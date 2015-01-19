#include <math.h>
#include <ch.h>
#include "mpu60X0.h"
#include "ms5611.h"

#include "imu.h"
#include "exti.h"

rate_gyro_sample_t mpu_gyro_sample;
accelerometer_sample_t mpu_acc_sample;

uint32_t barometer_pressure;
int32_t barometer_temperature;

#define SENSOR_INTERRUPT_EVENT 1

static THD_WORKING_AREA(i2c_sensors_wa, 512);
static THD_FUNCTION(i2c_sensors, arg)
{
    (void)arg;
    chRegSetThreadName("onboard-sensors-i2c");
    static event_listener_t sensor_int;
    chEvtRegisterMaskWithFlags(&exti_events, &sensor_int,
                               (eventmask_t)SENSOR_INTERRUPT_EVENT,
                               (eventflags_t)EXTI_EVENT_MPU6050_INT);
    /*
     * I2C
     */
    static const I2CConfig i2c_cfg = {
        .op_mode = OPMODE_I2C,
        .clock_speed = 200000,
        .duty_cycle = FAST_DUTY_CYCLE_2
    };

    I2CDriver *driver = &I2CD2;

    i2cStart(driver, &i2c_cfg);
    i2cAcquireBus(driver);


    ms5611_t ms5611;
    int init;
    if (init = ms5611_i2c_init(&ms5611, driver, 0)) {
        i2cflags_t flags = i2cGetErrors(driver);

        // i2cReleaseBus(driver);
        // i2cStop(driver);
        error_set(ERROR_LEVEL_CRITICAL);
        return 0;
    }

    chThdSleepMilliseconds(100);

    static mpu60X0_t mpu6050;
    mpu60X0_init_using_i2c(&mpu6050, driver, 0);

    if (!mpu60X0_ping(&mpu6050)) {
        error_set(ERROR_LEVEL_CRITICAL);
        return 0;
    }

    mpu60X0_setup(&mpu6050, MPU60X0_SAMPLE_RATE_DIV(0) | MPU60X0_ACC_FULL_RANGE_2G
        | MPU60X0_GYRO_FULL_RANGE_500DPS | MPU60X0_LOW_PASS_FILTER_6);

    static rate_gyro_t mpu_gyro = {
        .device = "MPU6050", .full_scale_range = {500*M_PI/360, 500*M_PI/360, 500*M_PI/360},
        .noise_stddev = {NAN, NAN, NAN}, .update_rate = 8000, .health = SENSOR_HEALTH_OK };
    static accelerometer_t mpu_acc = {
        .device = "MPU6050", .full_scale_range = {2*9.81, 2*9.81, 2*9.81},
        .noise_stddev = {NAN, NAN, NAN}, .update_rate = 8000, .health = SENSOR_HEALTH_OK };;
    mpu_gyro_sample.sensor = &mpu_gyro;
    mpu_acc_sample.sensor = &mpu_acc;


    unsigned int baro_counter = 0;
    while (1) {
        // timestamp_t timestamp;
        float gyro[3], acc[3], temp;
        chEvtWaitAny(SENSOR_INTERRUPT_EVENT);
        mpu60X0_read(&mpu6050, gyro, acc, &temp);

        chSysLock();
        mpu_gyro_sample.rate[0] = gyro[0];
        mpu_gyro_sample.rate[1] = gyro[1];
        mpu_gyro_sample.rate[2] = gyro[2];
        mpu_acc_sample.acceleration[0] = acc[0];
        mpu_acc_sample.acceleration[1] = acc[1];
        mpu_acc_sample.acceleration[2] = acc[2];
        chSysUnlock();

        if (baro_counter-- == 0) {
            uint32_t raw_t, raw_p, press;
            int32_t temp;
            int16_t t;
            baro_counter = 10;

            t = ms5611_adc_start(&ms5611, MS5611_ADC_TEMP, MS5611_OSR_4096);
            if (t < 0) {
                continue;
            }

            chThdSleepMilliseconds((t - 1)/1000 + 1);

            ms5611_adc_read(&ms5611, &raw_t);
            t = ms5611_adc_start(&ms5611, MS5611_ADC_PRESS, MS5611_OSR_4096);
            if (t < 0) {
                continue;
            }

            chThdSleepMilliseconds((t - 1)/1000 + 1);

            ms5611_adc_read(&ms5611, &raw_p);
            press = ms5611_calc_press(&ms5611, raw_p, raw_t, &temp);

            chSysLock();
            barometer_pressure = press;
            barometer_temperature = temp;
            chSysUnlock();
        }
    }
    return 0;
}

void onboard_sensors_start(void)
{
    exti_setup();
    chThdCreateStatic(i2c_sensors_wa, sizeof(i2c_sensors_wa), LOWPRIO, i2c_sensors, NULL);
}
