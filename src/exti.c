#include <ch.h>
#include <hal.h>
#include "exti.h"

event_source_t exti_events;

static void gpio_exti_callback(EXTDriver *extp, expchannel_t channel) {
    (void)extp;
    if (channel == GPIOC_HMC5883L_DRDY) {
        chSysLockFromISR();
        chEvtBroadcastFlagsI(&exti_events, EXTI_EVENT_HMC5883L_DRDY);
        chSysUnlockFromISR();
    } else if (channel == GPIOC_MPU6050_INT) {
        chSysLockFromISR();
        chEvtBroadcastFlagsI(&exti_events, EXTI_EVENT_MPU6050_INT);
        chSysUnlockFromISR();
    }
}

static const EXTConfig extcfg = {{
    {EXT_CH_MODE_DISABLED, NULL}, // 0
    {EXT_CH_MODE_DISABLED, NULL}, // 1
    {EXT_CH_MODE_DISABLED, NULL}, // 2
    {EXT_CH_MODE_DISABLED, NULL}, // 3
    {EXT_CH_MODE_DISABLED, NULL}, // 4
    {EXT_CH_MODE_DISABLED, NULL}, // 5
    {EXT_CH_MODE_DISABLED, NULL}, // 6
    {EXT_CH_MODE_DISABLED, NULL}, // 7
    {EXT_CH_MODE_DISABLED, NULL}, // 8
    {EXT_CH_MODE_DISABLED, NULL}, // 9
    // GPIOC_MPU6050_INT, C10
    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, gpio_exti_callback},
    {EXT_CH_MODE_DISABLED, NULL}, // 10
    {EXT_CH_MODE_DISABLED, NULL}, // 11
    {EXT_CH_MODE_DISABLED, NULL}, // 12
    // GPIOB_HMC5883L_DRDY, C13
    {EXT_CH_MODE_DISABLED, NULL}, // {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, gpio_exti_callback},
    {EXT_CH_MODE_DISABLED, NULL}, // 14
    {EXT_CH_MODE_DISABLED, NULL}, // 15
    {EXT_CH_MODE_DISABLED, NULL}, // 16
    {EXT_CH_MODE_DISABLED, NULL}, // 17
    {EXT_CH_MODE_DISABLED, NULL}, // 18
    {EXT_CH_MODE_DISABLED, NULL}, // 19
    {EXT_CH_MODE_DISABLED, NULL}, // 20
    {EXT_CH_MODE_DISABLED, NULL}, // 21
    {EXT_CH_MODE_DISABLED, NULL}  // 22
}};

void exti_setup(void)
{
    chEvtObjectInit(&exti_events);
    extStart(&EXTD1, &extcfg);
}
