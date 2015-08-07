#include <ch.h>
#include <chprintf.h>
#include "board.h"
#include "shell.h"
#include "onboardsensors.h"

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[])
{
    size_t n, size;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: mem\r\n");
        return;
    }
    n = chHeapStatus(NULL, &size);
    chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
    chprintf(chp, "heap fragments   : %u\r\n", n);
    chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[])
{
    static const char *states[] = {CH_STATE_NAMES};
    thread_t *tp;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: threads\r\n");
        return;
    }
    chprintf(chp, "    addr    stack prio refs     state\r\n");
    tp = chRegFirstThread();
    do {
        chprintf(chp, "%08lx %08lx %4lu %4lu %9s\r\n",
                 (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
                 (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
                 states[tp->p_state]);
        tp = chRegNextThread(tp);
    } while (tp != NULL);
}

static void cmd_sensors(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    int count = 100;
    while (count-- > 0) {
        float g[3], a[3], p;

        chSysLock();
        g[0] = gyro_sample.rate[0];
        g[1] = gyro_sample.rate[1];
        g[2] = gyro_sample.rate[2];
        a[0] = acc_sample.acceleration[0];
        a[1] = acc_sample.acceleration[1];
        a[2] = acc_sample.acceleration[2];
        p = barometer_sample.pressure;
        chSysUnlock();

        chprintf(chp, "gyro: %d %d %d, acc: %d %d %d, baro: %u\n",
            (int)(1000*g[0]),
            (int)(1000*g[1]),
            (int)(1000*g[2]),
            (int)(1000*a[0]),
            (int)(1000*a[1]),
            (int)(1000*a[2]),
            (unsigned int)(p));
        chThdSleepMilliseconds(100);
    }
}

static void cmd_dfu(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    chprintf(chp, "rebooting in 3 seconds...\n");
    chThdSleepMilliseconds(3000);
    reboot_and_run_bootloader();
}

const ShellCommand shell_commands[] = {
    {"mem", cmd_mem},
    {"threads", cmd_threads},
    {"sensors", cmd_sensors},
    {"dfu", cmd_dfu},
    {NULL, NULL}
};
