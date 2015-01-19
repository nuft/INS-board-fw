#ifndef EXTI_H
#define EXTI_H

#include <ch.h>

#define EXTI_EVENT_HMC5883L_DRDY   1
#define EXTI_EVENT_MPU6050_INT     2

extern event_source_t exti_events;

void exti_setup(void);

#endif // EXTI_H
