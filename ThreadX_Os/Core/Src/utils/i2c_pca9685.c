#include "i2c_pca9685.h"

HAL_StatusTypeDef   pca9685_init(I2C_HandleTypeDef *hi2c) {

    HAL_StatusTypeDef   status;
    uint8_t             data;

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);
    
    // Set to sleep mode to configure prescaler
    data = 0x10;
    status = HAL_I2C_Mem_Write(hi2c, PCA9685_ADDR << 1,
        MODE1, I2C_MEMADD_SIZE_8BIT,
        &data, 1, 100);
    if (status != HAL_OK) {
        uart_send("PCA9685: Failed to enter sleep mode\r\n");
        return (status);
    }

    // 50Hz
    data = 0x79;
    status = HAL_I2C_Mem_Write(hi2c, PCA9685_ADDR << 1,
        PRE_SCALE, I2C_MEMADD_SIZE_8BIT,
        &data, 1, 100);
    if (status != HAL_OK) {
        uart_send("PCA9685: Failed to set prescaler\r\n");
        return (status);
    }

    // Wake up & auto increment
    data = 0x20;
    status = HAL_I2C_Mem_Write(hi2c, PCA9685_ADDR << 1,
        MODE1, I2C_MEMADD_SIZE_8BIT,
        &data, 1, 100);
    if (status != HAL_OK) {
        uart_send("PCA9685: Failed to exit sleep mode\r\n");
        return (status);
    }

    tx_mutex_put(&i2c_mutex);
    return (HAL_OK);
}

HAL_StatusTypeDef   pca9685_set_pwm(I2C_HandleTypeDef *hi2c, uint8_t channel, 
                                uint16_t on, uint16_t off)
{
    HAL_StatusTypeDef   status;
    uint8_t             buf[4];
    uint8_t             reg;

    if (channel > PCA9685_CHANNEL_MAX) {
        uart_send("PCA9685: Invalid channel\r\n");
        return (HAL_ERROR);
    }

    // Buffer preparation with LSB / MSB
    buf[0] = on & 0xFF;         // ON_L
    buf[1] = (on >> 8) & 0x0F;  // ON_H
    buf[2] = off & 0xFF;        // OFF_L
    buf[3] = (off >> 8) & 0x0F; // OFF_H

    reg = LED0_ON_L + 4 * channel;

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);
    
    status = HAL_I2C_Mem_Write(hi2c, PCA9685_ADDR << 1,
        reg, I2C_MEMADD_SIZE_8BIT,
        buf, 4, 100);

    tx_mutex_put(&i2c_mutex);
    return (HAL_OK);
}
