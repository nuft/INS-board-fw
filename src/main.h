#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch.h>
#include "parameter/parameter.h"

extern BaseSequentialStream* stdout;

extern parameter_namespace_t parameters;

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */