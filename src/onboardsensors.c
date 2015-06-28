#include <math.h>
#include <ch.h>
#include "sensors/mpu60X0.h"
#include "sensors/ms5611.h"
#include "timestamp/timestamp.h"
#include "sensors.h"
#include "exti.h"

rate_gyro_sample_t gyro_sample;
accelerometer_sample_t acc_sample;
barometer_sample_t barometer_sample;

#define MPU_IRQ_EVENT_MASK 1

static THD_WORKING_AREA(imu_sensors_wa, 512);
static THD_FUNCTION(imu_sensors, arg)
{
    chRegSetThreadName("imu sensors");

    I2CDriver *driver = (I2CDriver *)arg;
    static mpu60X0_t mpu6050;

    static event_listener_t mpu_irq_listener;
    chEvtRegisterMaskWithFlags(&exti_events, &mpu_irq_listener,
                               (eventmask_t)MPU_IRQ_EVENT_MASK,
                               (eventflags_t)EXTI_EVENT_MPU6050_INT);

    i2cAcquireBus(driver);
    mpu60X0_init_using_i2c(&mpu6050, driver, 0);

    if (!mpu60X0_ping(&mpu6050)) {
        i2cReleaseBus(driver);
        error_set(ERROR_LEVEL_CRITICAL);
        return 0;
    }

    mpu60X0_setup(&mpu6050, MPU60X0_SAMPLE_RATE_DIV(0) | MPU60X0_ACC_FULL_RANGE_4G
        | MPU60X0_GYRO_FULL_RANGE_1000DPS | MPU60X0_LOW_PASS_FILTER_6);
    i2cReleaseBus(driver);

    static rate_gyro_t mpu_gyro = {
        .device = "MPU6050", .full_scale_range = 1000*M_PI/180, .update_rate = 1000};
    static accelerometer_t mpu_acc = {
        .device = "MPU6050", .full_scale_range = 4*9.81, .update_rate = 1000};;
    gyro_sample.sensor = &mpu_gyro;
    acc_sample.sensor = &mpu_acc;

    while (1) {
        float gyro[3], acc[3], temp;
        chEvtWaitAny(MPU_IRQ_EVENT_MASK);
        timestamp_t t = timestamp_get();
        i2cAcquireBus(driver);
        mpu60X0_read(&mpu6050, gyro, acc, &temp);
        i2cReleaseBus(driver);

        chSysLock();
        gyro_sample.timestamp = t;
        gyro_sample.rate[0] = gyro[0];
        gyro_sample.rate[1] = gyro[1];
        gyro_sample.rate[2] = gyro[2];
        acc_sample.timestamp = t;
        acc_sample.acceleration[0] = acc[0];
        acc_sample.acceleration[1] = acc[1];
        acc_sample.acceleration[2] = acc[2];
        chSysUnlock();
    }
    return 0;
}

static THD_WORKING_AREA(barometer_thread_wa, 512);
static THD_FUNCTION(barometer_thread, arg)
{
    I2CDriver *driver = (I2CDriver *)arg;
    static ms5611_t ms5611;

    chRegSetThreadName("barometer thread");

    i2cAcquireBus(driver);
    if (ms5611_i2c_init(&ms5611, driver, 0)) {
        i2cflags_t flags = i2cGetErrors(driver);
        i2cReleaseBus(driver);
        (void) flags;
        // error handling
        error_set(ERROR_LEVEL_CRITICAL);
        return 0;
    }
    i2cReleaseBus(driver);

    while (1) {
        uint32_t raw_t, raw_p, press;
        int32_t temp;
        int16_t t;

        i2cAcquireBus(driver);
        t = ms5611_adc_start(&ms5611, MS5611_ADC_TEMP, MS5611_OSR_4096);
        i2cReleaseBus(driver);
        if (t > 0) {
            chThdSleepMilliseconds((t - 1)/1000 + 1);
            i2cAcquireBus(driver);
            ms5611_adc_read(&ms5611, &raw_t);
            i2cReleaseBus(driver);
        } else {
            continue;
        }

        i2cAcquireBus(driver);
        t = ms5611_adc_start(&ms5611, MS5611_ADC_PRESS, MS5611_OSR_4096);
        i2cReleaseBus(driver);
        if (t > 0) {
            chThdSleepMilliseconds((t - 1)/1000 + 1);
            i2cAcquireBus(driver);
            ms5611_adc_read(&ms5611, &raw_p);
            i2cReleaseBus(driver);
        } else {
            continue;
        }
        press = ms5611_calc_press(&ms5611, raw_p, raw_t, &temp);

        chSysLock();
        barometer_sample.pressure = press;
        barometer_sample.temperature = (float)temp/100;
        chSysUnlock();

        chThdSleepMilliseconds(1);
    }

}

void onboard_sensors_start(void)
{
    /*
     * I2C setup
     */
    I2CDriver *driver = &I2CD2;
    static const I2CConfig i2c_cfg = {
        .op_mode = OPMODE_I2C,
        .clock_speed = 200000,
        .duty_cycle = FAST_DUTY_CYCLE_2
    };

    i2cStart(driver, &i2c_cfg);

    chThdCreateStatic(imu_sensors_wa, sizeof(imu_sensors_wa), NORMALPRIO, imu_sensors, driver);
    chThdCreateStatic(barometer_thread_wa, sizeof(barometer_thread_wa), LOWPRIO, barometer_thread, driver);
}
