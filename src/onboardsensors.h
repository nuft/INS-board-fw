#ifndef ONBOARDSENSORS_H
#define ONBOARDSENSORS_H

#include "sensors.h"

extern rate_gyro_sample_t gyro_sample;
extern accelerometer_sample_t acc_sample;
extern barometer_sample_t barometer_sample;
extern float battery_voltage;
extern float battery_current;

void onboard_sensors_start(void);

#endif // ONBOARDSENSORS_H