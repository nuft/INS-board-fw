#include <ch.h>
#include <hal.h>
#include "servo_pwm.h"

struct servo_list {
    PWMDriver *driver;
    pwmchannel_t channel;
};

static const struct servo_list servo_list[] =
{
    {.driver = &PWMD3, .channel = 0},   // PA6, TIM3_CH1, SERVO1
    {.driver = &PWMD3, .channel = 1},   // PA7, TIM3_CH2, SERVO2

    {.driver = &PWMD3, .channel = 2},   // PB0, TIM3_CH3, SERVO3
    {.driver = &PWMD3, .channel = 3},   // PB1, TIM3_CH4, SERVO4
    {.driver = &PWMD4, .channel = 0},   // PB6, TIM4_CH1, SERVO5
    {.driver = &PWMD4, .channel = 1},   // PB7, TIM4_CH2, SERVO6

    // todo: TIM15 is not supported by ChibiOS PWM driver
    // PB14, TIM15_CH1, SERVO7
    // PB15, TIM15_CH2, SERVO8
};

#define SERVO_COUNT (sizeof(servo_list)/sizeof(struct servo_list))

#define SERVO_PWM_FREQ      1000000 // 1MHz
#define SERVO_PWM_PERIOD    20000   // 20 ms period
static const PWMConfig pwmcfg = {
    SERVO_PWM_FREQ,
    SERVO_PWM_PERIOD,
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

void servo_pwm_setup(void)
{
    pwmStart(&PWMD3, &pwmcfg);
    pwmStart(&PWMD4, &pwmcfg);
}

// returns counter corresponding to a period of 1 to 2 ms.
static pwmcnt_t pos_to_pwmcnt(float pos)
{
    if (pos > 1) {
        pos = 1;
    } else if (pos < 0) {
        pos = 0;
    }
    return (pwmcnt_t)(pos * SERVO_PWM_FREQ / 1000) + 1000;
}

void servo_pwm_set_pos(unsigned int servo, float pos)
{
    if (servo >= SERVO_COUNT) {
        return;
    }
    pwmcnt_t cnt = pos_to_pwmcnt(pos);
    pwmEnableChannel(servo_list[servo].driver, servo_list[servo].channel, cnt);
}

