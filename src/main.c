#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <string.h>
#include "usbcfg.h"
#include "onboardsensors.h"
#include "serial-datagram/serial_datagram.h"
#include "cmp/cmp.h"
#include "exti.h"
#include "shell_commands.h"
#include "radio.h"

SerialUSBDriver SDU1;

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

int main(void)
{
    // check_bootloader();
    halInit();
    chSysInit();

    // debug UART
    sdStart(&UART_CONN1, NULL);
    chprintf((BaseSequentialStream*)&UART_CONN1, "\n> start\n");

    chThdCreateStatic(led_task_wa, sizeof(led_task_wa), LOWPRIO, led_task, NULL);

    exti_setup();

    charging_enable();

    // sensors
    onboard_sensors_start();

    // radio
    // radio_start();

    // // USB CDC
    // sduObjectInit(&SDU1);
    // sduStart(&SDU1, &serusbcfg);
    // usbDisconnectBus(serusbcfg.usbp);
    // chThdSleepMilliseconds(1000);
    // usbStart(serusbcfg.usbp, &usbcfg);
    // usbConnectBus(serusbcfg.usbp);
    // while (SDU1.config->usbp->state != USB_ACTIVE) {
    //     chThdSleepMilliseconds(10);
    // }

    while (true) {
        shell_run((BaseSequentialStream*)&UART_CONN1);
        chThdSleepMilliseconds(500);
    }
}
