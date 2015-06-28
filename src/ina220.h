#ifndef INA220_H
#define INA220_H

#include <stdint.h>
#include <stdbool.h>
#include <hal.h>

#define INA220_ADDRESS (0x8A>>1)

void ina220_setup(I2CDriver *driver, uint8_t address);
// [10uV]
bool ina220_read_shunt_voltage(I2CDriver *driver, uint8_t address, int16_t *voltage);
// [4mV]
bool ina220_read_bus_voltage(I2CDriver *driver, uint8_t address, int16_t *voltage);

#endif // INA220_H
