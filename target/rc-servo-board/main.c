#include <hal.h>
#include <ch.h>
#include <chprintf.h>

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

    while (1) {
        chprintf(stdout, "hello world\n");
        chThdSleepMilliseconds(500);
        palSetPad(GPIOC, GPIOC_LED_HEARTBEAT);
        chThdSleepMilliseconds(500);
        palClearPad(GPIOC, GPIOC_LED_HEARTBEAT);
    }
}
