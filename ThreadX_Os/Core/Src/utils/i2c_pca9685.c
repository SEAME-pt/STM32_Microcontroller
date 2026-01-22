#include "i2c_pca9685.h"

HAL_StatusTypeDef   pca9685_set_pwm(I2C_HandleTypeDef *hi2c, uint8_t channel, uint8_t addr7)
{
    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);
    // I2C communication to set PWM on PCA9685
    tx_mutex_put(&i2c_mutex);
    return HAL_OK;
}


void i2c_scan_bus(void) 
{
    uint8_t             i2c_address;
    HAL_StatusTypeDef   result;


    uart_send("Scanning I2C bus...\r\n");

    tx_thread_sleep(50);

    if (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY)
    {
        uart_send("I2C not ready\r\n");
    }


    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);
    for (i2c_address = 0x03; i2c_address < 0x78; i2c_address++) 
    {
        result = HAL_I2C_IsDeviceReady(&hi2c3, (i2c_address << 1), 1, 10);
        
        if (result == HAL_OK) 
        {
            char msg[30];
            snprintf(msg, sizeof(msg), "Found device at 0x%02X\r\n", i2c_address);
            uart_send(msg);
        }
    }
    tx_mutex_put(&i2c_mutex);

    uart_send("I2C bus scan complete.\r\n");
}