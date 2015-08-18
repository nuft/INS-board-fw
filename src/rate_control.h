#ifndef RATE_CONTROL_H
#define RATE_CONTROL_H

#include <stdint.h>
#include <timestamp/timestamp.h>

struct controller_struct {
    float x; // [rad/s]
    float y; // [rad/s]
    float z; // [rad/s]
    float power; // [0.0, 1.0]
    timestamp_t last_update;
};
extern struct controller_struct controller;

#ifdef __cplusplus
extern "C" {
#endif

void rate_control(void);

#ifdef __cplusplus
}
#endif

#endif /* RATE_CONTROL_H */
