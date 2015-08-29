#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <nullstreams.h>
#include <string.h>
#include "usbcfg.h"
#include "onboardsensors.h"
#include "serial-datagram/serial_datagram.h"
#include "cmp/cmp.h"
#include "exti.h"
#include "shell_cmd.h"
#include "radio.h"
#include "stream.h"
#include <timestamp/timestamp_stm32.h>

SerialUSBDriver SDU1;
NullStream null_stream;
// stream settings
BaseSequentialStream *debug_stream = (BaseSequentialStream *)&UART_EXT;
// BaseSequentialStream *debug_stream = (BaseSequentialStream *)&null_stream;
// BaseSequentialStream *shell_stream = (BaseSequentialStream *)&SDU1;
BaseSequentialStream *shell_stream = (BaseSequentialStream *)&UART_CONN1;

/*
 *  Heartbeat, Error LED thread
 *
 * The Heartbeat-LED double-flashes every second in normal mode and continuously
 *  flashes in safemode. If the error level is above NORMAL, the Heartbeat-LED
 *  is off and the Error-LED indicates the error level.
 * If the error level is WARNING, the Error-LED blinks slowly (once per second)
 * If the error level is CRITICAL, the Error-LED blinks rapidly (10 times per second)
 * If a kernel panic occurs the Error-LED is on and all LEDs are off.
 */
static THD_WORKING_AREA(led_task_wa, 128);
static THD_FUNCTION(led_task, arg)
{
    (void)arg;
    chRegSetThreadName("led_task");
    while (1) {
        int err = error_level_get();
        if (err == ERROR_LEVEL_WARNING) {
            palClearPad(GPIOC, GPIOC_LED_STATUS);
            chThdSleepMilliseconds(500);
            palSetPad(GPIOC, GPIOC_LED_STATUS);
            chThdSleepMilliseconds(500);
        } else if (err == ERROR_LEVEL_CRITICAL) {
            palClearPad(GPIOC, GPIOC_LED_STATUS);
            chThdSleepMilliseconds(50);
            palSetPad(GPIOC, GPIOC_LED_STATUS);
            chThdSleepMilliseconds(50);
        } else {
            palClearPad(GPIOC, GPIOC_LED_STATUS);
            chThdSleepMilliseconds(80);
            palSetPad(GPIOC, GPIOC_LED_STATUS);
            chThdSleepMilliseconds(80);
            palClearPad(GPIOC, GPIOC_LED_STATUS);
            chThdSleepMilliseconds(80);
            palSetPad(GPIOC, GPIOC_LED_STATUS);
            if (safemode_active()) {
                chThdSleepMilliseconds(80);
            } else {
                chThdSleepMilliseconds(760);
            }
        }
    }
    return 0;
}

void panic_handler(const char *reason)
{
    (void)reason;

    palClearPad(GPIOC, GPIOC_LED_STATUS);

    charging_disable(); // safety

#ifdef DEBUG
    while (1) {
        // if (!palReadPad(GPIOB, GPIOB_PB_CTRL_KILL)) {
        //     power_down();
        // }
    }
#else
    NVIC_SystemReset();
#endif
}

int main(void)
{
    halInit();
    chSysInit();

    nullObjectInit(&null_stream);
    sdStart(&UART_CONN1, NULL);
    sdStart(&UART_EXT, NULL);
    chprintf(debug_stream, "\n> boot\n");

    chThdCreateStatic(led_task_wa, sizeof(led_task_wa), LOWPRIO, led_task, NULL);

    timestamp_stm32_init();
    exti_setup();

    // radio
    radio_start_tx();
    // sensors
    // onboard_sensors_start();
    // stream_thread_start();

    // // USB CDC serial device
    // sduObjectInit(&SDU1);
    // sduStart(&SDU1, &serusbcfg);
    // // USB start and connect
    // usbDisconnectBus(serusbcfg.usbp);
    // chThdSleepMilliseconds(1500);
    // usbStart(serusbcfg.usbp, &usbcfg);
    // usbConnectBus(serusbcfg.usbp);

    shellInit();
    static thread_t *shelltp = NULL;
    static ShellConfig shell_cfg;
    shell_cfg.sc_channel = shell_stream;
    shell_cfg.sc_commands = shell_commands;
    while (true) {
        if (!shelltp) {
            if (shell_stream != (BaseSequentialStream *) &SDU1 || SDU1.config->usbp->state == USB_ACTIVE) {
                shelltp = shellCreate(&shell_cfg, THD_WORKING_AREA_SIZE(2048), NORMALPRIO);
            }
        } else if (chThdTerminatedX(shelltp)) {
            chThdRelease(shelltp);
            shelltp = NULL;
        }
        chThdSleepMilliseconds(500);
    }
}
