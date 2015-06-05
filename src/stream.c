#include <string.h>
#include <ch.h>
// #include <memstreams.h>
#include <cmp_mem_access/cmp_mem_access.h>
// #include <serial_datagram/serial_datagram.h>
#include "onboardsensors.h"
#include "radio.h"

#include "stream.h"

// MemoryStream mem_stream;

// static void _stream_send_fn(void *arg, const void *p, size_t len)
// {
//     if (len > 0) {
//         chSequentialStreamWrite((BaseSequentialStream*)arg, (const uint8_t*)p, len);
//     }
// }

static THD_WORKING_AREA(stream_thread_wa, 128);
static THD_FUNCTION(stream_thread, arg)
{
    (void)arg;
    chRegSetThreadName("stream_thread");

    // static uint8_t mem_stream_buffer[100];
    // static char dtgrm[100];
    static cmp_mem_access_t mem;
    static cmp_ctx_t cmp;
    while (1) {
        chThdSleepMilliseconds(100);
        struct radio_packet *p = radio_packet_alloc();
        if (p == NULL) {
            continue;
        }
        char *dtgrm = (char *) &p->data[0];

        float g[3];
        chSysLock();
        // gyro_sample.timestamp;
        g[0] = gyro_sample.rate[0];
        g[1] = gyro_sample.rate[1];
        g[2] = gyro_sample.rate[2];
        // acc_sample.timestamp;
        // acc_sample.acceleration[0];
        // acc_sample.acceleration[1];
        // acc_sample.acceleration[2];
        chSysUnlock();

        // msObjectInit(&mem_stream, mem_stream_buffer, sizeof(mem_stream_buffer), 0);
        cmp_mem_access_init(&cmp, &mem, dtgrm, sizeof(dtgrm));

        bool err = false;
        err = err || !cmp_write_map(&cmp, 1);
        const char *gyro_id = "gyro";
        err = err || !cmp_write_str(&cmp, gyro_id, strlen(gyro_id));
        err = err || !cmp_write_array(&cmp, 3);
        err = err || !cmp_write_float(&cmp, g[0]);
        err = err || !cmp_write_float(&cmp, g[1]);
        err = err || !cmp_write_float(&cmp, g[2]);

        // // encode as serial datagram
        // if (!err) {
        //     serial_datagram_send(dtgrm, cmp_mem_access_get_pos(&mem), _stream_send_fn, &mem_stream);
        // }

        // send
        p->length = cmp_mem_access_get_pos(&mem);
        radio_send(p);

        // while(1) {
        //     struct radio_packet *p = radio_packet_alloc();
        //     if (p == NULL) {
        //         break;
        //     }
        //     uint8_t len = chSequentialStreamRead(&mem_stream, p->data, 32);
        //     if (len == 0) {
        //         radio_packet_free(p);
        //         break;
        //     }
        //     p->length = len;
        //     radio_send(p);
        // }
    }
    return 0;
}

void stream_thread_start(void)
{
    chThdCreateStatic(stream_thread_wa, sizeof(stream_thread_wa), LOWPRIO, stream_thread, NULL);
}
