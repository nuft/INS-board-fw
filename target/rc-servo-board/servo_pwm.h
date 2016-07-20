#ifndef SERVO_PWM_H
#define SERVO_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#define SERVO1  (0)
#define SERVO2  (1)
#define SERVO3  (2)
#define SERVO4  (3)
#define SERVO5  (4)
#define SERVO6  (5)
#define SERVO7  (6)
#define SERVO8  (7)

void servo_pwm_setup(void);
void servo_pwm_set_pos(unsigned int servo, float pos);

#ifdef __cplusplus
}
#endif

#endif /* SERVO_PWM_H */
