#ifndef ONBOARDSENSORS_H
#define ONBOARDSENSORS_H

#include "imu.h"

extern rate_gyro_sample_t mpu_gyro_sample;
extern accelerometer_sample_t mpu_acc_sample;

extern uint32_t barometer_pressure;
extern int32_t barometer_temperature;

void onboard_sensors_start(void);

#endif // ONBOARDSENSORS_H