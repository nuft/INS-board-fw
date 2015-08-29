#ifndef RADIO_H
#define RADIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ch.h>

struct radio_packet {
    uint8_t length;
    uint8_t data[32];
};


// start the radio thread
void radio_start(void);


// get a packet buffer to send
struct radio_packet *radio_packet_alloc(void);

// free a packet buffer
void radio_packet_free(struct radio_packet *p);

void radio_send(struct radio_packet* packet);

#ifdef __cplusplus
}
#endif

#endif /* RADIO_H */
