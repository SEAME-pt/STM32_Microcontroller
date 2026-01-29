#ifndef I2C_INA219_H
# define I2C_INA219_H

#include "app_threadx.h"

#define INA219_ADDR  0x41

#define INA219_REG_CONFIG        0x00
#define INA219_REG_SHUNT_VOLT    0x01
#define INA219_REG_BUS_VOLT      0x02
#define INA219_REG_POWER         0x03
#define INA219_REG_CURRENT       0x04
#define INA219_REG_CALIBRATION   0x05

#define INA219_CALIBRATION  4096

HAL_StatusTypeDef   ina219_init(UINT addr);
HAL_StatusTypeDef   ina219_read_voltage(float* voltage);
HAL_StatusTypeDef   ina219_read_current(float* current);
HAL_StatusTypeDef   ina219_read_power(float* power);

#endif