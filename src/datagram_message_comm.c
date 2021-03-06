#include <ch.h>
#include "thread_prio.h"
#include "serial-datagram/serial_datagram.h"
#include "datagram-messages/msg_dispatcher.h"
#include "datagram-messages/service_call.h"
#include "parameter/parameter_msgpack.h"
#include <chprintf.h>
#include <memstreams.h>
#include "main.h"
#include "log.h"
#include "datagram_message_comm.h"


static void _serial_datagram_write_cb(void *arg, const void *p, size_t len)
{
    if (len > 0 && arg != NULL) {
        chSequentialStreamWrite((BaseSequentialStream*)arg, (const uint8_t*)p, len);
    }
}


static void datagram_send(const void *dtgrm, size_t len, void *arg)
{
    (void)arg;
    datagram_message_send(dtgrm, len);
}


static bool name_service(cmp_ctx_t *cmp_in, cmp_ctx_t *cmp_out, void *arg)
{
    (void)cmp_in;
    (void)arg;

    char buf[32];
    int len = parameter_string_get(&board_name, buf, sizeof(buf));
    return cmp_write_str(cmp_out, buf, len);
}


static bool ping_service(cmp_ctx_t *cmp_in, cmp_ctx_t *cmp_out, void *arg)
{
    (void)cmp_in;
    (void)arg;
    return cmp_write_str(cmp_out, "pong", 4);
}


static void parameter_set_err_cb(void *arg, const char *id, const char *err)
{
    MemoryStream *stream = (MemoryStream*)arg;
    chprintf((BaseSequentialStream*)stream, "parameter %s: %s\n", id, err);
}


static bool parameter_set_service(cmp_ctx_t *cmp_in, cmp_ctx_t *cmp_out, void *arg)
{
    (void)arg;
    const size_t buf_size = 512;
    uint8_t *parameter_set_err_buf = malloc(buf_size);
    if (parameter_set_err_buf == NULL) {
        bool err = false;
        err = err || !cmp_write_array(cmp_out, 2);
        err = err || !cmp_write_int(cmp_out, -1);
        err = err || !cmp_write_str(cmp_out,
                                    "malloc failed",
                                    strlen("malloc failed"));
        return !err;
    }

    MemoryStream parameter_set_err_buf_stream;
    msObjectInit(&parameter_set_err_buf_stream,
                 parameter_set_err_buf,
                 buf_size, 0);

    int ret = parameter_msgpack_read_cmp(&parameters,
                                         cmp_in,
                                         parameter_set_err_cb,
                                         &parameter_set_err_buf_stream);

    bool err = false;
    err = err || !cmp_write_array(cmp_out, 2);
    err = err || !cmp_write_int(cmp_out, ret);
    err = err || !cmp_write_str(cmp_out,
                                (char*)parameter_set_err_buf,
                                parameter_set_err_buf_stream.eos);

    free(parameter_set_err_buf);
    return !err;
}


void test_msg_cb(cmp_ctx_t *cmp, void *arg)
{
    (void)arg;
    int32_t i = 0;
    if (cmp_read_int(cmp, &i)) {
        log_info("test called with %d", i);
    } else {
        log_info("test called");
    }
}


static struct service_entry_s service_table[] = {
    {.id="name", .cb=name_service, .arg=NULL},
    {.id="parameter_set", .cb=parameter_set_service, .arg=NULL},
    {.id="ping", .cb=ping_service, .arg=NULL},
    {NULL, NULL, NULL}
};


static char resp_buffer[1024];
static struct service_call_handler_s service_call_handler = {
    .service_table = service_table,
    .response_buffer = &resp_buffer[0],
    .response_buffer_sz = sizeof(resp_buffer),
    .send_cb = datagram_send,
    .send_cb_arg = NULL,
};


static struct msg_dispatcher_entry_s dispatcher_table[] = {
    {.id="test", .cb=test_msg_cb, .arg=NULL},
    // don't modify these last two entries.
    {.id="req", .cb=service_call_msg_cb, .arg=&service_call_handler},
    {NULL, NULL, NULL}
};


static void *out_stream = NULL;


static THD_WORKING_AREA(stream_wa, 1024);
static THD_FUNCTION(stream, arg)
{
    chRegSetThreadName("datagram-message-handler");

    out_stream = arg;
    BaseSequentialStream *in = (BaseSequentialStream*)arg;

    static serial_datagram_rcv_handler_t rcv_handler;
    static char rcv_buffer[1024];
    serial_datagram_rcv_handler_init(&rcv_handler,
                                     rcv_buffer,
                                     sizeof(rcv_buffer),
                                     (void(*)(const void*, size_t, void*))msg_dispatcher,
                                     dispatcher_table);
    while (true) {
        char c = chSequentialStreamGet(in);
        int ret = serial_datagram_receive(&rcv_handler, &c, 1);
        switch (ret) {
        case SERIAL_DATAGRAM_RCV_CRC_MISMATCH:
            log_warning("SERIAL_DATAGRAM_RCV_CRC_MISMATCH");
            break;
        case SERIAL_DATAGRAM_RCV_PROTOCOL_ERROR:
            log_warning("SERIAL_DATAGRAM_RCV_PROTOCOL_ERROR");
            break;
        case SERIAL_DATAGRAM_RCV_DATAGRAM_TOO_LONG:
            log_warning("SERIAL_DATAGRAM_RCV_DATAGRAM_TOO_LONG");
            break;
        }
    }
}


static mutex_t send_lock;

void datagram_message_init(void)
{
    chMtxObjectInit(&send_lock);
}


void datagram_message_start(BaseSequentialStream *io)
{
    if (io == NULL) {
        return;
    }
    chThdCreateStatic(stream_wa, sizeof(stream_wa), THD_PRIO_DATAGRAM_MSG_HANDLER, stream, io);
}


void datagram_message_send(const char *buf, size_t size)
{
    chMtxLock(&send_lock);
    serial_datagram_send(buf, size, _serial_datagram_write_cb, out_stream);
    chMtxUnlock(&send_lock);
}
