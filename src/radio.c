#include <ch.h>
#include <hal.h>
#include <stdint.h>
#include <string.h>
#include "nrf24l01p.h"
#include "nrf24l01p_registers.h"
#include "radio.h"
#include "exti.h"
#include <chprintf.h>
#include <cmp_mem_access/cmp_mem_access.h>

#define NRF_INTERRUPT_EVENT 1

const uint8_t address[] = {0x2A, 0x2A, 0x2A};
const uint8_t channel = 42;
const uint8_t datarate = RF_DR_2M;

#define RADIO_BUF_SIZE 16
memory_pool_t radio_packet_pool;
struct radio_packet radio_packet_pool_buf[RADIO_BUF_SIZE];
msg_t radio_rx_mbox_buf[RADIO_BUF_SIZE];
mailbox_t radio_rx_mbox;
event_listener_t radio_event_listener;

SPIDriver *spi_init(void);
void nrf_ce_active(void);
void nrf_ce_inactive(void);

struct radio_packet *radio_packet_alloc(void)
{
    return (struct radio_packet *)chPoolAlloc(&radio_packet_pool);
}

void radio_packet_free(struct radio_packet *p)
{
    chPoolFree(&radio_packet_pool, p);
}

static void radio_rx_packet_pool_init(void)
{
    chMBObjectInit(&radio_rx_mbox, radio_rx_mbox_buf, RADIO_BUF_SIZE);
    chPoolObjectInit(&radio_packet_pool, sizeof(struct radio_packet), NULL);
    chPoolLoadArray(&radio_packet_pool, radio_packet_pool_buf, RADIO_BUF_SIZE);
}

static void nrf_event_register(void)
{
    chEvtRegisterMaskWithFlags(&exti_events, &radio_event_listener,
        NRF_INTERRUPT_EVENT, EXTI_EVENT_NRF_IRQ);
}

extern BaseSequentialStream *stdout;

// static void nrf_setup_rx_only(nrf24l01p_t *dev)
// {
//     nrf_ce_inactive();
//     // 2 byte CRC, enable RX_DR, mask MAX_RT and TX_DS IRQ
//     uint8_t config = PRIM_RX | PWR_UP | EN_CRC | MASK_TX_DS | MASK_MAX_RT;
//     // uint8_t config = PRIM_RX | PWR_UP | EN_CRC;
//     nrf24l01p_write_register(dev, CONFIG, config);
//     // frequency = 2400 + <channel> [MHz], maximum: 2525MHz
//     nrf24l01p_set_channel(dev, channel);
//     // 0dBm power, datarate 2M/1M/250K
//     nrf24l01p_write_register(dev, RF_SETUP, RF_PWR(3) | datarate);
//     // disable dynamic packet length (DPL)
//     nrf24l01p_write_register(dev, FEATURE, 0);
//     // 3 byte address length
//     nrf24l01p_write_register(dev, SETUP_AW, AW_3);
//     // RX address
//     nrf24l01p_write_register(dev, EN_RXADDR, ERX_P0);
//     nrf24l01p_set_addr(dev, RX_ADDR_P0, address, 3);
//     nrf24l01p_write_register(dev, RX_PW_P0, PACKET_STATIC_LENGTH);
//     // disable Enhanced ShockBurst Auto Acknowledgment
//     nrf24l01p_write_register(dev, EN_AA, 0);
//     // clear data fifo
//     nrf24l01p_flush_rx(dev);
//     // clear IRQ flags
//     nrf24l01p_write_register(dev, STATUS, RX_DR | TX_DS | MAX_RT);
// }

// static void nrf_setup_tx_only(nrf24l01p_t *dev)
// {
//     nrf_ce_inactive();
//     // 2 byte CRC, enable TX_DS, RX_DR and MAX_RT IRQ
//     uint8_t config = EN_CRC | MASK_RX_DR | MASK_MAX_RT;
//     // uint8_t config = EN_CRC;
//     nrf24l01p_write_register(dev, CONFIG, config);
//     // frequency = 2400 + <channel> [MHz], maximum: 2525MHz
//     nrf24l01p_set_channel(dev, channel);
//     // 0dBm power, datarate 2M/1M/250K
//     nrf24l01p_write_register(dev, RF_SETUP, RF_PWR(3) | datarate);
//     // Disable retransmission
//     nrf24l01p_write_register(dev, SETUP_RETR, ARD(0) | ARC(0));
//     // disable dynamic packet length (DPL)
//     nrf24l01p_write_register(dev, FEATURE, 0);
//     // disable Enhanced ShockBurst Auto Acknowledgment
//     nrf24l01p_write_register(dev, EN_AA, 0);
//     // 3 byte address length
//     nrf24l01p_write_register(dev, SETUP_AW, AW_3);
//     // TX address
//     nrf24l01p_set_addr(dev, TX_ADDR, address, 3);
//     // clear data fifo
//     nrf24l01p_flush_tx(dev);
//     // clear IRQ flags
//     nrf24l01p_write_register(dev, STATUS, RX_DR | TX_DS | MAX_RT);
//     nrf24l01p_write_register(dev, CONFIG, config | PWR_UP);
// }

static void nrf_setup_ptx(nrf24l01p_t *dev)
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

static void nrf_setup_prx(nrf24l01p_t *dev)
{
    nrf_ce_inactive();
    // 2 byte CRC, enable RX_DR, mask MAX_RT and TX_DS IRQ
    // uint8_t config = PRIM_RX | PWR_UP | EN_CRC | MASK_TX_DS | MASK_MAX_RT;
    uint8_t config = PRIM_RX | PWR_UP | EN_CRC | MASK_MAX_RT;
    nrf24l01p_write_register(dev, CONFIG, config);
    // frequency = 2400 + <channel> [MHz], maximum: 2525MHz
    nrf24l01p_set_channel(dev, channel);
    // 0dBm power, datarate 2M/1M/250K
    nrf24l01p_write_register(dev, RF_SETUP, RF_PWR(3) | datarate);
    // enable dynamic packet length (DPL)
    nrf24l01p_write_register(dev, FEATURE, EN_DPL | EN_ACK_PAY);
    // 3 byte address length
    nrf24l01p_write_register(dev, SETUP_AW, AW_3);
    // RX address
    nrf24l01p_write_register(dev, EN_RXADDR, ERX_P0);
    nrf24l01p_set_addr(dev, RX_ADDR_P0, address, 3);
    // enable Enhanced ShockBurst Auto Acknowledgment
    nrf24l01p_write_register(dev, EN_AA, ENAA_P0);
    nrf24l01p_write_register(dev, DYNPD, DPL_P0);
    // clear data fifo
    nrf24l01p_flush_tx(dev);
    nrf24l01p_flush_rx(dev);
    // default ack payload
    uint8_t ack[] = {0};
    nrf24l01p_write_ack_payload(dev, 0, &ack[0], sizeof(ack));
    // clear IRQ flags
    nrf24l01p_write_register(dev, STATUS, RX_DR | TX_DS | MAX_RT);
}

void encode_packet(struct radio_packet *p)
{
    static cmp_ctx_t cmp;
    static cmp_mem_access_t mem;

    // struct radio_packet *p = radio_packet_alloc();
    if (p == NULL) {
        return;
    }

    cmp_mem_access_init(&cmp, &mem, &p->data[0], 32);

/*
    float g[3];
    chSysLock();
    g[0] = l3gd20_gyro_rate[0];
    g[1] = l3gd20_gyro_rate[1];
    g[2] = l3gd20_gyro_rate[2];
    chSysUnlock();

    bool err = false;
    err = err || !cmp_write_map(&cmp, 1);
    const char *gyro_id = "gyro";
    err = err || !cmp_write_str(&cmp, gyro_id, strlen(gyro_id));
    err = err || !cmp_write_array(&cmp, 3);
    err = err || !cmp_write_float(&cmp, g[0]);
    err = err || !cmp_write_float(&cmp, g[1]);
    err = err || !cmp_write_float(&cmp, g[2]);*/

    static uint64_t count = 0;

    bool err = false;
    err = err || !cmp_write_map(&cmp, 1);
    const char *coutn_id = "count";
    err = err || !cmp_write_str(&cmp, coutn_id, strlen(coutn_id));
    err = err || !cmp_write_uint(&cmp, count++);

    p->length = 0;
    if (!err) {
        p->length = cmp_mem_access_get_pos(&mem);
        // radio_send(p);
    }
}


// void tx_only(nrf24l01p_t *nrf)
// {
//     nrf_setup_tx_only(nrf);
//     nrf_event_register();
//     static struct radio_packet tx_packet;
//     struct radio_packet *p = &tx_packet;
//     while (1) {
//         if (palReadPad(GPIOA, GPIOA_BUTTON)) {
//             NVIC_SystemReset();
//         }
//         set(0);
//         set(1);
//         set(2);
//         set(3);
//         // clear interrupts
//         nrf24l01p_write_register(nrf, STATUS, RX_DR | TX_DS | MAX_RT);

//         encode_packet(p);
//         // nrf24l01p_write_tx_payload_no_ack(nrf, p->data, p->length);
//         // nrf24l01p_write_tx_payload(nrf, p->data, p->length);
//         nrf24l01p_write_tx_payload(nrf, p->data, PACKET_STATIC_LENGTH);
//         clear(0);

//         nrf_ce_active();
//         eventmask_t ret = chEvtWaitAnyTimeout(NRF_INTERRUPT_EVENT, MS2ST(100));
//         nrf_ce_inactive();
//         clear(1);

//         if (ret == 0) {
//             toggle(4);
//             palClearPad(GPIOE, GPIOE_LED4_BLUE);
//             palTogglePad(GPIOE, GPIOE_LED3_RED);
//             nrf24l01p_flush_tx(nrf);
//             continue;
//         }
//         clear(2);

//         palTogglePad(GPIOE, GPIOE_LED4_BLUE);
//         chThdSleepMilliseconds(1);
//     }
// }

// void rx_only(nrf24l01p_t *nrf)
// {
//     nrf_setup_rx_only(nrf);
//     nrf_event_register();
//     nrf_ce_active();
//     while (1) {
//         eventmask_t ret = chEvtWaitAnyTimeout(NRF_INTERRUPT_EVENT, MS2ST(100));

//         if (ret == 0) {
//             nrf24l01p_flush_rx(nrf);
//             palClearPad(GPIOE, GPIOE_LED4_BLUE);
//             palTogglePad(GPIOE, GPIOE_LED3_RED);
//             continue;
//         }

//         uint8_t status = nrf24l01p_status(nrf);
//         if (status & RX_DR) {
//             // clear status flags
//             nrf24l01p_write_register(nrf, STATUS, RX_DR | TX_DS | MAX_RT);

//             uint8_t len = nrf24l01p_read_rx_payload_len(nrf);
//             if (len == 0 || len > 32) { // invalid length
//                 nrf24l01p_flush_rx(nrf);
//                 continue;
//             }

//             struct radio_packet *p = radio_packet_alloc();
//             if (p != NULL) {
//                 nrf24l01p_read_rx_payload(nrf, p->data, len);
//                 p->length = len;
//                 msg_t m = chMBPost(&radio_rx_mbox, (msg_t) p, TIME_IMMEDIATE);
//                 if (m != MSG_OK) {
//                     radio_packet_free(p);
//                 }
//             } else {
//                 palTogglePad(GPIOE, GPIOE_LED8_ORANGE);
//             }
//             palTogglePad(GPIOE, GPIOE_LED4_BLUE);
//         }
//         //  else {
//         //     nrf24l01p_write_register(nrf, STATUS, RX_DR | TX_DS | MAX_RT);
//         //     nrf24l01p_flush_rx(nrf);
//         // }
//     }
// }

static THD_WORKING_AREA(radio_thread_wa, 512);

static THD_FUNCTION(radio_thread_tx, arg)
{
    nrf24l01p_t *nrf = (nrf24l01p_t *)arg;
    nrf_setup_ptx(nrf);
    nrf_event_register();
    static struct radio_packet tx_packet;
    while (1) {
        // clear interrupts
        nrf24l01p_write_register(nrf, STATUS, RX_DR | TX_DS | MAX_RT);
        encode_packet(&tx_packet);
        nrf24l01p_write_tx_payload(nrf, tx_packet.data, tx_packet.length);

        nrf_ce_active();
        eventmask_t ret = chEvtWaitAnyTimeout(NRF_INTERRUPT_EVENT, MS2ST(100));
        nrf_ce_inactive();

        if (ret == 0) {
            palClearPad(GPIOC, GPIOC_LED_STATUS); /////// TMP
            nrf24l01p_flush_tx(nrf);
            continue;
        }

        uint8_t status = nrf24l01p_status(nrf);
        if (status & RX_DR) {
            struct radio_packet *p;
            p = radio_packet_alloc();
            uint8_t len = nrf24l01p_read_rx_payload_len(nrf);
            if (len == 0 || len > 32 || p == NULL) { // invalid length
                nrf24l01p_flush_rx(nrf);
                continue;
            }
            nrf24l01p_read_rx_payload(nrf, p->data, len);
            p->length = len;

            msg_t m = chMBPost(&radio_rx_mbox, (msg_t)p, TIME_IMMEDIATE);
            if (m != MSG_OK) {
                radio_packet_free(p);
            }
        } else if (status & MAX_RT) {
            // nrf24l01p_flush_tx(nrf);
            // nrf24l01p_flush_rx(nrf);
        }
        // chThdSleepMilliseconds(1);
    }
    return 0;
}

static THD_FUNCTION(radio_thread_rx, arg)
{
    nrf24l01p_t *nrf = (nrf24l01p_t *)arg;
    nrf_setup_prx(nrf);
    nrf_event_register();
    nrf_ce_active();
    struct radio_packet buf;
    buf.length = 1;
    buf.data[0] = 42;
    while (1) {
        // encode_packet(&buf);
        nrf24l01p_write_ack_payload(nrf, 0, buf.data, buf.length);

        nrf24l01p_write_register(nrf, STATUS, RX_DR | TX_DS | MAX_RT);

        eventmask_t ret = chEvtWaitAnyTimeout(NRF_INTERRUPT_EVENT, MS2ST(100));
        if (ret == 0) {
            nrf24l01p_flush_rx(nrf);
            palClearPad(GPIOC, GPIOC_LED_STATUS); /////// TMP
            continue;
        }

        uint8_t status = nrf24l01p_status(nrf);
        if (status & RX_DR) {
            struct radio_packet *packet;
            // packet = radio_packet_alloc();
            packet = &buf;
            uint8_t len = nrf24l01p_read_rx_payload_len(nrf);
            if (len == 0 || len > 32 || packet == NULL) {
                nrf24l01p_flush_rx(nrf);
                continue;
            }
            nrf24l01p_read_rx_payload(nrf, packet->data, len);
            packet->length = len;

            // // chPoolFree(&radio_packet_pool, packet);
            // msg_t m = chMBPost(&radio_rx_mbox, (msg_t)packet, TIME_IMMEDIATE);
            // if (m != MSG_OK) {
            //     chPoolFree(&radio_packet_pool, packet);
            // }
        }
        //  else {
        //     nrf24l01p_write_register(nrf, STATUS, RX_DR | TX_DS | MAX_RT);
        //     nrf24l01p_flush_rx(nrf);
        //     nrf24l01p_flush_tx(nrf);
        // }
    }
    return 0;
}

void radio_start_rx(void)
{
    static nrf24l01p_t nrf;
    SPIDriver *spi = spi_init();
    nrf24l01p_init(&nrf, spi);
    radio_rx_packet_pool_init();
    // rx
    chThdCreateStatic(radio_thread_wa, sizeof(radio_thread_wa), NORMALPRIO, radio_thread_rx, &nrf);
}

void radio_start_tx(void)
{
    static nrf24l01p_t nrf;
    SPIDriver *spi = spi_init();
    nrf24l01p_init(&nrf, spi);
    radio_rx_packet_pool_init();
    // tx
    chThdCreateStatic(radio_thread_wa, sizeof(radio_thread_wa), NORMALPRIO, radio_thread_tx, &nrf);
}
