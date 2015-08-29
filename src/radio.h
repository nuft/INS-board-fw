#ifndef RADIO_H
#define RADIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch.h>

struct radio_packet {
    uint8_t length;
    uint8_t _reserved[3];
    uint8_t data[32];
};

extern mailbox_t radio_rx_mbox;

// start the radio thread
void radio_start(void);

// get a packet buffer to send
struct radio_packet *radio_packet_alloc(void);

// free a packet buffer
void radio_packet_free(struct radio_packet *p);

void radio_start_rx(void);
void radio_start_tx(void);

#ifdef __cplusplus
}
#endif

#endif /* RADIO_H */
