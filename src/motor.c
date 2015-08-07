#include <ch.h>
#include <hal.h>
#include "board.h"
#include "motor.h"

// Motor 1: TIM3 CH1 PC6
// Motor 2: TIM3 CH2 PC7
// Motor 3: TIM3 CH3 PC8
// Motor 4: TIM3 CH4 PC9
// Motor 5: TIM4 CH3 PB8
// Motor 6: TIM4 CH4 PB9
// Timer 3&4: Alternate Function AF2

#define MOTOR_PWM_FREQ      1000000 // 1MHz
#define MOTOR_PWM_PERIOD    1000    // 1 ms period

static const PWMConfig pwmcfg = {
    MOTOR_PWM_FREQ,
    MOTOR_PWM_PERIOD,
    NULL,   // no callback
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}
    },
    0,      // TIMx_CR2 value
    0       // TIMx_DIER value
};

void motor_init(void)
{
    pwmStart(&PWMD3, &pwmcfg);
    pwmStart(&PWMD4, &pwmcfg);
}

static pwmcnt_t thrust_to_pwmcnt(float th)
{
    if (th > 1) {
        th = 1;
    } else if (th < 0) {
        th = 0;
    }
    return (pwmcnt_t)(th * MOTOR_PWM_PERIOD);
}

static void _thrust_set(const float *m)
{
    // TIM3, Motor 1-4
    pwmEnableChannel(&PWMD3, 0, thrust_to_pwmcnt(m[0]));
    pwmEnableChannel(&PWMD3, 1, thrust_to_pwmcnt(m[1]));
    pwmEnableChannel(&PWMD3, 2, thrust_to_pwmcnt(m[2]));
    pwmEnableChannel(&PWMD3, 3, thrust_to_pwmcnt(m[3]));
    // TIM4, Motor 5-6
    pwmEnableChannel(&PWMD4, 2, thrust_to_pwmcnt(m[4]));
    pwmEnableChannel(&PWMD4, 3, thrust_to_pwmcnt(m[5]));
}

void motor_thrust_set(const float *m)
{
    if (m != NULL && error_level_get() == ERROR_LEVEL_NORMAL) {
        _thrust_set(m);
    } else {
        motor_off();
    }
}

void motor_off(void)
{
    static const float no_thrust[6] = {0,0,0,0,0,0};
    _thrust_set(no_thrust);
}
