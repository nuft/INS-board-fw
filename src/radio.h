#ifndef RADIO_H
#define RADIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ch.h>

#define RADIO_DATARATE_250K 0
#define RADIO_DATARATE_1M   1
#define RADIO_DATARATE_2M   2

struct radio_packet {
    uint8_t length;
    uint8_t data[32];
};

struct radio_port {
    mailbox_t tx_mbox;
    mailbox_t rx_mbox;
    uint8_t _tx_seq;
    uint8_t _rx_seq;
};

// start the radio thread
void radio_start(uint8_t channel, uint8_t *addr, uint8_t datarate);

// register a port for reception and sending
// notes: - the mailboxes of the port have to be initialized
//        - can be called before radio_start()
void radio_port_register(struct radio_port *port, uint8_t number);

// get a packet buffer to send
struct radio_packet *radio_get_packet_buffer(void);

// send a packet
void radio_send(struct radio_port *port, struct radio_packet* packet);

#ifdef __cplusplus
}
#endif

#endif /* RADIO_H */
