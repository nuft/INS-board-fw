#include <string.h>
#include <ch.h>
#include <hal.h>
#include <cmp_mem_access/cmp_mem_access.h>
#include "onboardsensors.h"
#include "radio.h"
#include "stream.h"

/*
packet size in bytes:
1       fixmap
4+1     string id e.g. 'gyro'
1       fixarray
3*(4+1) floats values
1+1     string id time 't'
(4+1)   timestamp u32
--------
29 bytes
*/

static cmp_ctx_t cmp;
static cmp_mem_access_t mem;

static void stream_gyro(void)
{
    struct radio_packet *p = radio_packet_alloc();
    if (p == NULL) {
        return;
    }

    float g[3];
    timestamp_t t;
    chSysLock();
    g[0] = gyro_sample.rate[0];
    g[1] = gyro_sample.rate[1];
    g[2] = gyro_sample.rate[2];
    t = gyro_sample.timestamp;
    chSysUnlock();

    cmp_mem_access_init(&cmp, &mem, &p->data[0], 32);

    bool err = false;
    err = err || !cmp_write_map(&cmp, 2);
    const char *gyro_id = "gyro";
    err = err || !cmp_write_str(&cmp, gyro_id, strlen(gyro_id));
    err = err || !cmp_write_array(&cmp, 3);
    err = err || !cmp_write_float(&cmp, g[0]);
    err = err || !cmp_write_float(&cmp, g[1]);
    err = err || !cmp_write_float(&cmp, g[2]);
    const char *time_id = "t";
    err = err || !cmp_write_str(&cmp, time_id, strlen(time_id));
    err = err || !cmp_write_uint(&cmp, t);

    if (!err) {
        p->length = cmp_mem_access_get_pos(&mem);
        radio_send(p);
    }
}

static void stream_acc(void)
{
    struct radio_packet *p = radio_packet_alloc();
    if (p == NULL) {
        return;
    }

    float a[3];
    timestamp_t t;
    chSysLock();
    a[0] = acc_sample.acceleration[0];
    a[1] = acc_sample.acceleration[1];
    a[2] = acc_sample.acceleration[2];
    t = acc_sample.timestamp;
    chSysUnlock();

    cmp_mem_access_init(&cmp, &mem, &p->data[0], 32);

    bool err = false;
    err = err || !cmp_write_map(&cmp, 2);
    const char *acc_id = "acc";
    err = err || !cmp_write_str(&cmp, acc_id, strlen(acc_id));
    err = err || !cmp_write_array(&cmp, 3);
    err = err || !cmp_write_float(&cmp, a[0]);
    err = err || !cmp_write_float(&cmp, a[1]);
    err = err || !cmp_write_float(&cmp, a[2]);
    const char *time_id = "t";
    err = err || !cmp_write_str(&cmp, time_id, strlen(time_id));
    err = err || !cmp_write_uint(&cmp, t);

    if (!err) {
        p->length = cmp_mem_access_get_pos(&mem);
        radio_send(p);
    }
}

static void stream_baro(void)
{
    struct radio_packet *p = radio_packet_alloc();
    if (p == NULL) {
        return;
    }

    float press, temp;
    chSysLock();
    press = barometer_sample.pressure;
    temp = barometer_sample.temperature;
    chSysUnlock();

    cmp_mem_access_init(&cmp, &mem, &p->data[0], 32);

    bool err = false;
    err = err || !cmp_write_map(&cmp, 2);
    const char *baro_id = "baro";
    err = err || !cmp_write_str(&cmp, baro_id, strlen(baro_id));
    err = err || !cmp_write_float(&cmp, press);
    const char *temp_id = "temp";
    err = err || !cmp_write_str(&cmp, temp_id, strlen(temp_id));
    err = err || !cmp_write_float(&cmp, temp);

    if (!err) {
        p->length = cmp_mem_access_get_pos(&mem);
        radio_send(p);
    }
}

static void stream_battery(void)
{
    struct radio_packet *p = radio_packet_alloc();
    if (p == NULL) {
        return;
    }

    float volt, cur;
    chSysLock();
    volt = battery_voltage;
    cur = battery_current;
    chSysUnlock();

    cmp_mem_access_init(&cmp, &mem, &p->data[0], 32);

    bool err = false;
    err = err || !cmp_write_map(&cmp, 2);
    const char *volt_id = "V_Bat";
    err = err || !cmp_write_str(&cmp, volt_id, strlen(volt_id));
    err = err || !cmp_write_float(&cmp, volt);
    const char *cur_id = "I_Bat";
    err = err || !cmp_write_str(&cmp, cur_id, strlen(cur_id));
    err = err || !cmp_write_float(&cmp, cur);

    if (!err) {
        p->length = cmp_mem_access_get_pos(&mem);
        radio_send(p);
    }
}

static THD_WORKING_AREA(stream_thread_wa, 256);
static THD_FUNCTION(stream_thread, arg)
{
    (void)arg;
    chRegSetThreadName("stream_thread");

    // apparently packets don't arrive if sent too frequently.
    // need to wait between sending.
    while (1) {
        chThdSleepMilliseconds(5);
        stream_gyro();
        // chThdSleepMilliseconds(5);
        // stream_acc();
        // chThdSleepMilliseconds(5);
        // stream_baro();
        // chThdSleepMilliseconds(5);
        // stream_battery();
    }
    return 0;
}

void stream_thread_start(void)
{
    chThdCreateStatic(stream_thread_wa, sizeof(stream_thread_wa), NORMALPRIO+1, stream_thread, NULL);
}
