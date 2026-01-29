#include "i2c_ina219.h"

HAL_StatusTypeDef   ina219_init(UINT addr) {

    HAL_StatusTypeDef status;
    uint8_t data[2];

    // Config:
    // Bus voltage range = 32V
    // Gain = /8 (320mV shunt)
    // Bus ADC = 12bit
    // Shunt ADC = 12bit
    // Mode = Shunt + Bus continuous
    uint16_t config = 0x019F;

    data[0] = (config >> 8) & 0xFF;
    data[1] = config & 0xFF;

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);

    status = HAL_I2C_Mem_Write(&hi2c3,
        addr << 1,
        INA219_REG_CONFIG,
        I2C_MEMADD_SIZE_8BIT,
        data, 2, 100);

    if (status != HAL_OK) {
        tx_mutex_put(&i2c_mutex);
        return status;
    }

    // Calibration
    data[0] = (INA219_CALIBRATION >> 8) & 0xFF;
    data[1] = INA219_CALIBRATION & 0xFF;

    status = HAL_I2C_Mem_Write(&hi2c3,
        addr << 1,
        INA219_REG_CALIBRATION,
        I2C_MEMADD_SIZE_8BIT,
        data, 2, 100);

    tx_mutex_put(&i2c_mutex);
    return status;
}

HAL_StatusTypeDef ina219_read_voltage(float *voltage)
{
    uint8_t buf[2];
    uint16_t raw;

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);

    if (HAL_I2C_Mem_Read(&hi2c3,
        INA219_ADDR << 1,
        INA219_REG_BUS_VOLT,
        I2C_MEMADD_SIZE_8BIT,
        buf, 2, 100) != HAL_OK) {
        tx_mutex_put(&i2c_mutex);
        return HAL_ERROR;
    }

    raw = (buf[0] << 8) | buf[1];
    raw >>= 3; // remove flags

    *voltage = raw * 0.004f; // 4mV per bit

    return HAL_OK;
}

HAL_StatusTypeDef ina219_read_current(float *current)
{
    uint8_t buf[2];
    int16_t raw;

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);

    if (HAL_I2C_Mem_Read(&hi2c3,
        INA219_ADDR << 1,
        INA219_REG_CURRENT,
        I2C_MEMADD_SIZE_8BIT,
        buf, 2, 100) != HAL_OK) {
        tx_mutex_put(&i2c_mutex);
        return HAL_ERROR;
    }
    tx_mutex_put(&i2c_mutex);

    raw = (buf[0] << 8) | buf[1];

    *current = raw * 0.1f; // 0.1mA per bit

    return HAL_OK;
}

HAL_StatusTypeDef ina219_read_power(float *power)
{
    uint8_t buf[2];
    uint16_t raw;

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);

    if (HAL_I2C_Mem_Read(&hi2c3,
        INA219_ADDR << 1,
        INA219_REG_POWER,
        I2C_MEMADD_SIZE_8BIT,
        buf, 2, 100) != HAL_OK) {
        tx_mutex_put(&i2c_mutex);
        return HAL_ERROR;
    }
    tx_mutex_put(&i2c_mutex);
    raw = (buf[0] << 8) | buf[1];

    *power = raw * 2.0f; // 2mW per bit

    return HAL_OK;
}
