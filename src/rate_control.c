
#include <ch.h>
#include <pid.h>
#include <timestamp/timestamp.h>
#include <onboardsensors.h>
#include <motor.h>
#include "rate_control.h"


#include <hal.h>
#include <chprintf.h>

struct controller_struct controller = {0,0,0,0,0};

//  3[o]   [x]1    ^ X
//       X         |
//  2[x]   [o]4   Z+----> Y

static float motors[6] = {0,0,0,0,0,0};
enum {motor1 = 0, motor2 = 1, motor3 = 2, motor4 = 3, motor5 = 4, motor6 = 5};

float thrust_saturate(float pwr)
{
    if (pwr > 1.0f) {
        return 1.0f;
    } else if (pwr < 0.0f) {
        return 0.0f;
    }
    return pwr;
}

void motor_control(float rx, float ry, float rz, float power)
{
    motors[motor1] = thrust_saturate(power - rx + ry - rz);
    motors[motor2] = thrust_saturate(power + rx - ry - rz);
    motors[motor3] = thrust_saturate(power + rx + ry + rz);
    motors[motor4] = thrust_saturate(power - rx - ry + rz);
    motors[motor5] = thrust_saturate(0);
    motors[motor6] = thrust_saturate(0);
    motor_thrust_set(motors);
}

struct pid pidx, pidy, pidz;

/*
successful flight-test PID parameters:
xy:
p: 10/10000
i: 80/10000
d: 1/10000

z:
p: 20/10000
i: 16/10000
d: 1/10000
*/

void kpid_init(void)
{
    // pidx.kp = 0.001;
    // pidx.ki = 0.008;
    // pidx.kd = 0.0001;
    // pidx.integration_bound = 8;

    // pidy.kp = 0.001;
    // pidy.ki = 0.008;
    // pidy.kd = 0.0001;
    // pidy.integration_bound = 8;

    // pidz.kp = 0.003;
    // pidz.ki = 0.0020;
    // pidz.kd = 0.0001;
    // pidz.integration_bound = 40;

    pidx.kp = 0.057;
    pidx.ki = 0.0456;
    pidx.kd = 0.0057;
    pidx.integration_bound = 60;

    pidy.kp = 0.057;
    pidy.ki = 0.0456;
    pidy.kd = 0.0057;
    pidy.integration_bound = 60;

    pidz.kp = 0.018;
    pidz.ki = 0.016;
    pidz.kd = 0.0057;
    pidz.integration_bound = 300;
}

bool motors_active(void)
{
    return true;
}

void rate_control(void)
{
    pid_init(&pidx);
    pid_init(&pidy);
    pid_init(&pidz);
    kpid_init();
    timestamp_t last = timestamp_get();
    while (1) {
        timestamp_t now = timestamp_get();
        float delta_t = timestamp_duration_s(last, now);
        last = now;
        if (!motors_active()) {
            pid_reset(&pidx);
            pid_reset(&pidy);
            pid_reset(&pidz);
            motor_off();
        } else {
            float gx, gy, gz;
            chSysLock();
            gx = gyro_sample.rate[0];
            gy = gyro_sample.rate[1];
            gz = gyro_sample.rate[2];
            chSysUnlock();
            motor_control(pid_control(&pidx, gx - controller.x, delta_t),
                          pid_control(&pidy, gy - controller.y, delta_t),
                          pid_control(&pidz, gz - controller.z, delta_t),
                          controller.power);
        }
        chThdSleepMilliseconds(10);
    }
}

