#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch.h>

void shell_run(BaseSequentialStream *stream);

#ifdef __cplusplus
}
#endif

#endif /* SHELL_COMMANDS_H */