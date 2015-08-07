#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

void motor_init(void);

void motor_thrust_set(const float *m);

void motor_off(void);

#ifdef __cplusplus
}
#endif

#endif /* MOTOR_H */
