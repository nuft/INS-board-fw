#include <hal.h>
#include <ch.h>
#include <chprintf.h>
#include "servo_pwm.h"

#define BROADCAST_ADDRESS   0x3f
#define NETWORK_ADDRESS     10

static const CANConfig can1_config = {
    .mcr = (1 << 6)  /* Automatic bus-off management enabled. */
         | (1 << 2), /* Message are prioritized by order of arrival. */
    /* APB1 Clock is 36 Mhz
       Data rate: 36MHz / 2 / (1tq + 10tq + 7tq) = 1MHz => 1Mbit */
    .btr = ((2 - 1) << 0)  /* Baudrate prescaler (10 bits) */
         | ((10 - 1) << 16) /* Time segment 1 (4 bits) */
         | ((7 - 1) << 20) /* Time segment 2 (3 bits) */
         | (0 << 24) /* Resync jump width (2 bits) */
};

static THD_WORKING_AREA(can_thread, 1000);
static THD_FUNCTION(can_thread_main, arg)
{
    (void)arg;
    while (1) {
        static CANRxFrame rxf;
        msg_t m = canReceive(&CAND1, CAN_ANY_MAILBOX, &rxf, TIME_INFINITE);
        if (m != MSG_OK || rxf.IDE || rxf.RTR) {
            continue;
        }
        // mask out priority & source address
        // check for single frame transfer
        if ((rxf.SID & 0x403) != (1<<10 | 0<<1 | 1<<0)) {
            continue;
        }
        // are we addressed?
        if (rxf.data8[0] != NETWORK_ADDRESS ||rxf.DLC != 1 + 1 + 4) {
            continue;
        }
        uint8_t servo = rxf.data8[1];
        float position = (float)((uint32_t)rxf.data8[2] | rxf.data8[3]<<8
                                 | rxf.data8[4]<<16 | rxf.data8[5]<<24);
        servo_pwm_set_pos(servo, position);
    }
}

void can_send_dummy_frame(void)
{
    CANTxFrame txf;
    txf.SID = 1<<10 | NETWORK_ADDRESS<<2 | 0<<1 | 1<<0;
    txf.IDE = 0;
    txf.RTR = 0;
    txf.DLC = 2;
    txf.data8[0] = BROADCAST_ADDRESS;
    txf.data8[1] = 42;
    canTransmit(&CAND1, CAN_ANY_MAILBOX, &txf, TIME_INFINITE);
}

void can_setup(void)
{
    // enable CAN transceiver
    palClearPad(GPIOA, GPIOA_CAN_EN);
    // start CAN driver
    canStart(&CAND1, &can1_config);
    // start receive thread
    chThdCreateStatic(can_thread, sizeof(can_thread), NORMALPRIO, can_thread_main, NULL);
}

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
    can_setup();

    while (1) {
        can_send_dummy_frame();
        chThdSleepMilliseconds(500);
        palSetPad(GPIOC, GPIOC_LED_HEARTBEAT);
        chThdSleepMilliseconds(500);
        palClearPad(GPIOC, GPIOC_LED_HEARTBEAT);
    }
}
