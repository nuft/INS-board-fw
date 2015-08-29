#include <ch.h>
#include <hal.h>
#include <stdint.h>
#include <string.h>
#include "nrf24l01p.h"
#include "nrf24l01p_registers.h"
#include "radio.h"
#include "exti.h"

#define NRF_INTERRUPT_EVENT 1

#define RADIO_BUF_SIZE 32
memory_pool_t radio_packet_pool;
struct radio_packet radio_packet_pool_buf[RADIO_BUF_SIZE];
mailbox_t radio_tx_mbox;

static void nrf_ce_active(void)
{
    palSetPad(GPIOC, GPIOC_NRF_CE);
}

static void nrf_ce_inactive(void)
{
    palClearPad(GPIOC, GPIOC_NRF_CE);
}

const uint8_t address[] = {0x2A, 0x2A, 0x2A};
const uint8_t channel = 42;
const uint8_t datarate = RF_DR_2M;

void nrf_setup_ptx(nrf24l01p_t *dev)
{
    nrf_ce_inactive();
    // 2 byte CRC, enable TX_DS, RX_DR and MAX_RT IRQ
    uint8_t config = EN_CRC;
    nrf24l01p_write_register(dev, CONFIG, config);
    // frequency = 2400 + <channel> [MHz], maximum: 2525MHz
    nrf24l01p_set_channel(dev, channel);
    // 0dBm power, datarate 2M/1M/250K
    nrf24l01p_write_register(dev, RF_SETUP, RF_PWR(3) | datarate);
    // Disable retransmission, 1500us delay
    nrf24l01p_write_register(dev, SETUP_RETR, ARD(5) | ARC(1));
    // enable dynamic packet length (DPL)
    nrf24l01p_write_register(dev, FEATURE, EN_DPL | EN_ACK_PAY);
    // enable Enhanced ShockBurst Auto Acknowledgment
    nrf24l01p_write_register(dev, EN_AA, ENAA_P0);
    // 3 byte address length
    nrf24l01p_write_register(dev, SETUP_AW, AW_3);
    // TX address
    nrf24l01p_set_addr(dev, TX_ADDR, address, 3);
    // RX address
    nrf24l01p_set_addr(dev, RX_ADDR_P0, address, 3);
    nrf24l01p_write_register(dev, DYNPD, DPL_P0);
    // clear data fifo
    nrf24l01p_flush_tx(dev);
    nrf24l01p_flush_rx(dev);
    // clear IRQ flags
    nrf24l01p_write_register(dev, STATUS, RX_DR | TX_DS | MAX_RT);
    nrf24l01p_write_register(dev, CONFIG, config | PWR_UP);
}

struct radio_packet *radio_get_next_tx_packet(void)
{
    struct radio_packet *packet;
    msg_t m;
    // m = chMBFetch(&radio_tx_mbox, (msg_t *)&packet, TIME_INFINITE);
    // m = chMBFetch(&radio_tx_mbox, (msg_t *)&packet, TIME_IMMEDIATE);
    m = chMBFetch(&radio_tx_mbox, (msg_t *)&packet, MS2ST(100));
    if (m == MSG_OK) {
        return packet;
    }
    return NULL;

    // int i;
    // for (i = 0; i < 16; i++) {
    //     if (radio_ports[i] == NULL) {
    //         continue;
    //     }
    //     msg_t m = chMBFetch(&radio_ports[i]->tx_mbox, (msg_t *)&packet, TIME_IMMEDIATE);
    //     if (m == MSG_OK) {
    //         uint8_t seq = (radio_ports[i]->_tx_seq++ << 4);
    //         // set port number and sequence number
    //         packet->data[0] = i | seq;
    //         return packet;
    //     }
    // }
    // return NULL;
}

// static bool radio_forward_rx_packet(struct radio_packet *packet)
// {
//     uint8_t i = packet->data[0] & 0x0f;
//     if (radio_ports[i] == NULL) {
//         return false;
//     }
//     if ((packet->data[0] & 0xf0) == (radio_ports[i]->_rx_seq << 4)) {
//         // same packet as last time
//         return false;
//     }
//     msg_t m = chMBPost(&radio_ports[i]->rx_mbox, (msg_t)packet, TIME_IMMEDIATE);
//     if (m != MSG_OK) {
//         return false;
//     }
//     return true;
// }

static THD_WORKING_AREA(radio_thread_wa, 256);
static THD_FUNCTION(radio_thread, arg)
{
    nrf24l01p_t *nrf = (nrf24l01p_t *)arg;
    nrf_setup_ptx(nrf);

    static event_listener_t radio_event_listener;
    chEvtRegisterMaskWithFlags(&exti_events, &radio_event_listener,
        NRF_INTERRUPT_EVENT, EXTI_EVENT_NRF_IRQ);

    static struct radio_packet *tx;
    while (1) {
        nrf24l01p_write_register(nrf, STATUS, RX_DR | TX_DS | MAX_RT);
        // tx = radio_get_next_tx_packet();

        // if (tx != NULL) {
        //     nrf24l01p_write_tx_payload(nrf, tx->data, tx->length);
        //     chPoolFree(&radio_packet_pool, (void *)tx);
        // } else {
        //     // chThdSleepMilliseconds(1);
        //     // continue;
        //     nrf24l01p_write_tx_payload(nrf, "cvra", 5);
        // }
        nrf24l01p_write_tx_payload(nrf, "cvra", 5);


        nrf_ce_active();
        eventmask_t ret = chEvtWaitAnyTimeout(NRF_INTERRUPT_EVENT, MS2ST(100));
        nrf_ce_inactive();
        if (ret == 0) {
            nrf24l01p_flush_tx(nrf);
            continue;
        }

        uint8_t status = nrf24l01p_status(nrf);
        if (status & RX_DR) {
            struct radio_packet *p;
            p = radio_packet_alloc();
            uint8_t len = nrf24l01p_read_rx_payload_len(nrf);
            if (len == 0 || len > 32 || p == NULL) { // invalid length
                if (p != NULL) {
                    radio_packet_free(p);
                }
                nrf24l01p_flush_rx(nrf);
                continue;
            }
            nrf24l01p_read_rx_payload(nrf, p->data, len);
            p->length = len;

            radio_packet_free(p);
            // msg_t m = chMBPost(&radio_rx_mbox, (msg_t)p, TIME_IMMEDIATE);
            // if (m != MSG_OK) {
            //     radio_packet_free(p);
            // }
        } else if (status & MAX_RT) {
            // nrf24l01p_flush_tx(nrf);
            // nrf24l01p_flush_rx(nrf);
        }
        chThdSleepMilliseconds(1);
    }
    return 0;
}

void radio_start(void)
{
    static msg_t radio_tx_mbox_buf[10];
    chMBObjectInit(&radio_tx_mbox, radio_tx_mbox_buf, 10);

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

    static nrf24l01p_t nrf;
    nrf24l01p_init(&nrf, &SPID2);

    chThdCreateStatic(radio_thread_wa, sizeof(radio_thread_wa), NORMALPRIO, radio_thread, &nrf);
}

void radio_send(struct radio_packet* packet)
{
    msg_t m = chMBPost(&radio_tx_mbox, (msg_t)packet, TIME_IMMEDIATE);
    // msg_t m = chMBPost(&radio_tx_mbox, (msg_t)packet, MS2ST(100));
    if (m != MSG_OK) {
        panic_handler("packet send");
        chPoolFree(&radio_packet_pool, packet);
    }
}

struct radio_packet *radio_packet_alloc(void)
{
    return (struct radio_packet *)chPoolAlloc(&radio_packet_pool);
}

void radio_packet_free(struct radio_packet *p)
{
    chPoolFree(&radio_packet_pool, p);
}
