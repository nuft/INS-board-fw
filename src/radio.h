#ifndef RADIO_H
#define RADIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ch.h>

/*
#define RADIO_PACKET_BUFFER_DELETE  0
#define RADIO_PACKET_BUFFER_KEEP    1
#define RADIO_PACKET_REFERENCE      2

struct radio_packet_header {
     uint8_t length  :6;
     uint8_t type    :2;
}

struct radio_packet_reference {
    struct radio_packet_header header;
    uint8_t *reference;
}

struct radio_packet {
    struct radio_packet_header header;
    uint8_t data[32];
};
*/

struct radio_packet {
    uint8_t length;
    uint8_t data[32];
};

// struct radio_port {
//     mailbox_t tx_mbox;
//     mailbox_t rx_mbox;
//     binary_semaphore_t bsem;
//     uint8_t number;
// };

// start the radio thread
void radio_start(void);

// // register a port for reception and sending
// // notes: - the mailboxes of the port have to be initialized
// //        - can be called before radio_start()
// void radio_port_register(struct radio_port *port, uint8_t number);

// get a packet buffer to send
struct radio_packet *radio_packet_alloc(void);

// free a packet buffer
void radio_packet_free(struct radio_packet *p);

void radio_send(struct radio_packet* packet);

// // send a packet
// void radio_send(struct radio_port *port, struct radio_packet* packet);

#ifdef __cplusplus
}
#endif

#endif /* RADIO_H */
