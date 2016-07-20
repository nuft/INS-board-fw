#include <hal.h>
#include <ch.h>
#include <chprintf.h>
#include "servo_pwm.h"

static const SerialConfig uart_config = {
    115200,
    0,
    USART_CR2_STOP1_BITS | USART_CR2_LINEN,
    0
};

BaseSequentialStream *stdout;

int main(void)
{
    halInit();
    chSysInit();

    sdStart(&SD1, &uart_config);
    stdout = (BaseSequentialStream *)&SD1;

    chprintf(stdout, "boot\n");

    servo_pwm_setup();

    while (1) {
        servo_pwm_set_pos(SERVO1, 0.5f);
        chThdSleepMilliseconds(500);
        palSetPad(GPIOC, GPIOC_LED_HEARTBEAT);
        servo_pwm_set_pos(SERVO1, 1.0f);
        chThdSleepMilliseconds(500);
        palClearPad(GPIOC, GPIOC_LED_HEARTBEAT);
    }
}
