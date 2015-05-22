#ifndef COMMANDS_H
#define COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch.h>

void shell_run(BaseSequentialStream *stream);

#ifdef __cplusplus
}
#endif

#endif /* COMMANDS_H */