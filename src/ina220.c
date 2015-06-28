#include "ina220.h"

#define INA220_CONF_REG      0x00
#define INA220_SHUNT_VOLTAGE 0x01
#define INA220_BUS_VOLTAGE   0x02
#define INA220_POWER         0x03
#define INA220_CURRENT       0x04
#define INA220_CALIBRATION   0x05

#define INA220_CONF_RST     (1<<15)
#define INA220_CONF_BRNG    (1<<13)
#define INA220_CONF_PG0     (1<<12)
#define INA220_CONF_PG1     (1<<11)
#define INA220_CONF_BADC4   (1<<10)
#define INA220_CONF_BADC3   (1<<9)
#define INA220_CONF_BADC2   (1<<8)
#define INA220_CONF_BADC1   (1<<7)
#define INA220_CONF_SADC4   (1<<6)
#define INA220_CONF_SADC3   (1<<5)
#define INA220_CONF_SADC2   (1<<4)
#define INA220_CONF_SADC1   (1<<3)
#define INA220_CONF_MODE3   (1<<2)
#define INA220_CONF_MODE2   (1<<1)
#define INA220_CONF_MODE1   (1<<0)

#define INA220_BUS_V_CNVR   (1<<1)
#define INA220_BUS_V_OVF    (1<<0)

// ARM: little endian (lowest address = least significant)
// INA220 16bit registers are transmitted MSByte first

void ina220_setup(I2CDriver *driver, uint8_t address)
{
    // config:
    // bus voltage range: 16V : BRNG = 0
    // Gain 1x: full scale = 40mV : PG = 0
    // 8.51ms sampling time (16x oversampling) : ADC4 | ADC3 (1100)
    // Mode: Shunt and Bus, Continuous : Mode1 | Mode2 | Mode3 (111)
    uint16_t config = (INA220_CONF_BADC4 | INA220_CONF_BADC3
        | INA220_CONF_SADC4 | INA220_CONF_SADC3
        | INA220_CONF_MODE3 | INA220_CONF_MODE2 | INA220_CONF_MODE1);
    uint8_t transm[] = {INA220_CONF_REG, (config>>8) & 0xff, config & 0xff};
    i2cMasterTransmit(driver, address, transm, sizeof(transm), NULL, 0);

    // calibration register:
    // current = U / R = shunt voltage * calibration / 4096
    // R = 4096 / calibration , calibration = 4096 / R
    // TODO
}

// [10uV]
bool ina220_read_shunt_voltage(I2CDriver *driver, uint8_t address, int16_t *voltage)
{
    // shunt voltage: LSB = 10uV (16 bit 2complement)
    uint8_t reg = INA220_SHUNT_VOLTAGE;
    uint8_t ret[2];
    if (i2cMasterTransmit(driver, address, &reg, 1, &ret[0], 1) != MSG_OK) {
        return false;
    }
    *voltage = ((int16_t)ret[0]<<8) | ret[1];
    return true;
}

// (>>3) [4mV]
bool ina220_read_bus_voltage(I2CDriver *driver, uint8_t address, int16_t *voltage)
{
    // bus voltage: LSB = 4mV (12 bit, shift register right by 3 bits)
    uint8_t reg = INA220_BUS_VOLTAGE;
    uint8_t ret[2];
    if (i2cMasterTransmit(driver, address, &reg, 1, &ret[0], 1) != MSG_OK) {
        return false;
    }
    *voltage = ((int16_t)ret[0]<<8) | ret[1];
    return true;
}
