#include <ch.h>
#include <hal.h>
#include <stdint.h>
#include <string.h>
#include "nrf24l01p.h"
#include "nrf24l01p_registers.h"
#include "radio.h"
#include "exti.h"

#define NRF_INTERRUPT_EVENT 1

struct radio_config {
    uint8_t address[5];
    uint8_t address_len;
    uint8_t channel;
    uint8_t datarate;
    nrf24l01p_t dev;
};

#define RADIO_BUF_SIZE 32
memory_pool_t radio_packet_pool;
struct radio_packet radio_packet_pool_buf[RADIO_BUF_SIZE];
struct radio_port *radio_ports[16] = {NULL};

static void nrf_ce_active(void)
{
    palSetPad(GPIOC, GPIOC_NRF_CE);
}

static void nrf_ce_inactive(void)
{
    palClearPad(GPIOC, GPIOC_NRF_CE);
}

static void nrf_setup_ptx(struct radio_config *cfg)
{
    nrf24l01p_t *dev = &cfg->dev;

    nrf_ce_inactive();
    // 2 byte CRC, enable TX_DS, RX_DR and MAX_RT IRQ
    nrf24l01p_write_register(dev, CONFIG, PWR_UP | EN_CRC | CRCO);
    // frequency = 2400 + <channel> [MHz], maximum: 2525MHz
    nrf24l01p_set_channel(dev, cfg->channel);
    // 0dBm power, datarate 2M/1M/250K
    nrf24l01p_write_register(dev, RF_SETUP, RF_PWR(3) | cfg->datarate);
    // Disable retransmission, 1500us delay
    nrf24l01p_write_register(dev, SETUP_RETR, ARD(5) | ARC(0));
    // enable dynamic packet length (DPL)
    nrf24l01p_write_register(dev, FEATURE, EN_DPL | EN_ACK_PAY);
    // 3 byte address length
    nrf24l01p_write_register(dev, SETUP_AW, AW_3);
    // TX address
    nrf24l01p_set_addr(dev, TX_ADDR, cfg->address, 3);
    // RX address
    nrf24l01p_set_addr(dev, RX_ADDR_P0, cfg->address, 3);
    nrf24l01p_write_register(dev, DYNPD, DPL_P0);
    // clear data fifo
    nrf24l01p_flush_tx(dev);
    nrf24l01p_flush_rx(dev);
    // clear IRQ flags
    nrf24l01p_write_register(dev, STATUS, RX_DR | TX_DS | MAX_RT);
}

static struct radio_packet *radio_get_next_tx_packet(void)
{
    struct radio_packet *packet;
    int i;
    for (i = 0; i < 16; i++) {
        if (radio_ports[i] == NULL) {
            continue;
        }
        msg_t m = chMBFetch(&radio_ports[i]->tx_mbox, (msg_t *)&packet, TIME_IMMEDIATE);
        if (m == MSG_OK) {
            uint8_t seq = (radio_ports[i]->_tx_seq++ << 4);
            // set port number and sequence number
            packet->data[0] = i | seq;
            return packet;
        }
    }
    return NULL;
}

static bool radio_forward_rx_packet(struct radio_packet *packet)
{
    uint8_t i = packet->data[0] & 0x0f;
    if (radio_ports[i] == NULL) {
        return false;
    }
    if ((packet->data[0] & 0xf0) == (radio_ports[i]->_rx_seq << 4)) {
        // same packet as last time
        return false;
    }
    msg_t m = chMBPost(&radio_ports[i]->rx_mbox, (msg_t)packet, TIME_IMMEDIATE);
    if (m != MSG_OK) {
        return false;
    }
    return true;
}

static THD_WORKING_AREA(radio_thread_wa, 256);
static THD_FUNCTION(radio_thread, arg)
{
    struct radio_config *config = (struct radio_config *)arg;
    nrf_setup_ptx(config);

    nrf24l01p_t *nrf = &config->dev;

    event_listener_t radio_event_listener;
    chEvtRegisterMaskWithFlags(&exti_events, &radio_event_listener,
        NRF_INTERRUPT_EVENT, EXTI_EVENT_NRF_IRQ);

    const uint8_t null_packet[] = {0xff};

    struct radio_packet *packet;
    while (1) {
        // clear interrupts
        nrf24l01p_write_register(nrf, STATUS, RX_DR | TX_DS | MAX_RT);

        packet = radio_get_next_tx_packet();
        if (packet != NULL) {
            nrf24l01p_write_tx_payload(nrf, packet->data, packet->length);
            chPoolFree(&radio_packet_pool, (void *)packet);
        } else {
            // nothing to send
            nrf24l01p_write_tx_payload(nrf, null_packet, sizeof(null_packet));
        }

        nrf_ce_active();
        eventmask_t ret = chEvtWaitAnyTimeout(NRF_INTERRUPT_EVENT, MS2ST(10));
        nrf_ce_inactive();
        if (ret == 0) {
            // timeout
            nrf24l01p_flush_rx(nrf);
            nrf24l01p_flush_tx(nrf);
            continue;
        }

        uint8_t status = nrf24l01p_status(nrf);
        if (status & RX_DR) {
            uint8_t len = nrf24l01p_read_rx_payload_len(nrf);
            if (len == 0 || len > 32) { // invalid length
                nrf24l01p_flush_rx(nrf);
                continue;
            }
            packet = (struct radio_packet *) chPoolAlloc(&radio_packet_pool);
            if (packet == NULL) {
                continue;
            }
            nrf24l01p_read_rx_payload(nrf, packet->data, len);
            packet->length = len;
            if (!radio_forward_rx_packet(packet)) {
                chPoolFree(&radio_packet_pool, (void *)packet);
            }
        } else if (status & MAX_RT) {
            nrf24l01p_flush_rx(nrf);
            nrf24l01p_flush_tx(nrf);
        }
    }
    return 0;
}

void radio_start(void)
{
    static struct radio_config config = {
        .channel = 42,
        .datarate = RF_DR_2M,
        .address = {1,2,3},
        .address_len = 3
    };

    chPoolObjectInit(&radio_packet_pool, sizeof(struct radio_packet), NULL);
    chPoolLoadArray(&radio_packet_pool, radio_packet_pool_buf, RADIO_BUF_SIZE);

    /*
     * SPI2 configuration structure.
     * SPI2 is on APB1 @ 42MHz / 4 = 10.5 MHz
     * CPHA=0, CPOL=0, 8bits frames, MSb transmitted first.
     */
    static SPIConfig spi_cfg = {
        .end_cb = NULL,
        .ssport = GPIOB,
        .sspad = GPIOB_NRF_CSN,
        .cr1 =  SPI_CR1_BR_0
    };

    spiStart(&SPID2, &spi_cfg);
    nrf24l01p_init(&config.dev, &SPID2);

    chThdCreateStatic(radio_thread_wa, sizeof(radio_thread_wa), NORMALPRIO, radio_thread, &config);
}

void radio_port_register(struct radio_port *port, uint8_t number)
{
    if (number > 15) {
        return;
    }
    port->_tx_seq = 0;
    port->_rx_seq = 0xff;
    radio_ports[number] = port;
}

void radio_send(struct radio_port *port, struct radio_packet* packet)
{
    msg_t m = chMBPost(&port->tx_mbox, (msg_t)packet, TIME_IMMEDIATE);
    if (m != MSG_OK) {
        chPoolFree(&radio_packet_pool, packet);
    }
}

struct radio_packet *radio_get_packet_buffer(void)
{
    return (struct radio_packet *)chPoolAlloc(&radio_packet_pool);
}

void radio_free_packet_buffer(struct radio_packet *p)
{
    chPoolFree(&radio_packet_pool, p);
}

/*
void nrf_setup_prx(nrf24l01p_t *dev)
{
    nrf_ce_inactive();
    // 2 byte CRC, enable RX_DR, mask MAX_RT and TX_DS IRQ
    nrf24l01p_write_register(dev, CONFIG, PRIM_RX | PWR_UP | EN_CRC | CRCO
        | MASK_TX_DS | MASK_MAX_RT);
    // frequency = 2400 + <channel> [MHz], maximum: 2525MHz
    nrf24l01p_set_channel(dev, channel);
    // 0dBm power, datarate 2M/1M/250K
    nrf24l01p_write_register(dev, RF_SETUP, RF_PWR(3) | RF_DR_250K);
    // enable dynamic packet length (DPL)
    nrf24l01p_write_register(dev, FEATURE, EN_DPL | EN_ACK_PAY);
    // enable DPL for pipe 0
    nrf24l01p_write_register(dev, DYNPD, DPL_P0);
    // nrf24l01p_write_register(dev, RX_PW_P0, 32);
    // 3 byte address length
    nrf24l01p_write_register(dev, SETUP_AW, AW_3);
    // RX address
    nrf24l01p_write_register(dev, EN_RXADDR, ERX_P0);
    nrf24l01p_set_addr(dev, RX_ADDR_P0, address, 3);
    // enable Enhanced ShockBurst Auto Acknowledgment
    nrf24l01p_write_register(dev, EN_AA, ENAA_P0);
    // clear data fifo
    nrf24l01p_flush_tx(dev);
    nrf24l01p_flush_rx(dev);
    // default ack payload
    uint8_t ack[] = {0};
    nrf24l01p_write_ack_payload(dev, 0, &ack[0], sizeof(ack));
    // clear IRQ flags
    nrf24l01p_write_register(dev, STATUS, RX_DR | TX_DS | MAX_RT);
}

void cmd_radio_rx(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    static nrf24l01p_t nrf24l01p;
    nrf24l01p_t *nrf = &nrf24l01p;
    spi_init();
    nrf24l01p_init(nrf, &SPID2);
    nrf_setup_prx(nrf);

    event_listener_t radio_event_listener;
    chEvtRegisterMaskWithFlags(&exti_events, &radio_event_listener,
        NRF_INTERRUPT_EVENT, EXTI_EVENT_NRF_IRQ);

    static uint8_t ack[32];
    static uint8_t rx_buf[32];

    nrf_ce_active();

    while (1) {
        nrf24l01p_write_register(nrf, STATUS, RX_DR | TX_DS | MAX_RT);
        eventmask_t ret = chEvtWaitAnyTimeout(NRF_INTERRUPT_EVENT, MS2ST(100));

        if (ret == 0) {
            nrf24l01p_flush_rx(nrf);
            continue;
        }

        uint8_t status = nrf24l01p_status(nrf);
        if (status & RX_DR) {
            uint8_t len = nrf24l01p_read_rx_payload_len(nrf);
            if (len == 0 || len > 32) { // invalid length
                nrf24l01p_flush_rx(nrf);
                continue;
            }
            nrf24l01p_read_rx_payload(nrf, rx_buf, len);
        }

        nrf24l01p_write_ack_payload(nrf, 0, ack, 32);
    }
}
*/