#include "app_threadx.h"

HAL_StatusTypeDef   i2c_scan_bus(VOID) 
{
    uint8_t             i2c_address;
    HAL_StatusTypeDef   result;

    tx_thread_sleep(50);

    if (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY) 
    {
        uart_send("I2C not ready\r\n");
        return HAL_BUSY;
    }
    else
        uart_send("Scanning I2C bus...\r\n");

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);
    for (i2c_address = 0x03; i2c_address < 0x78; i2c_address++) 
    {
        result = HAL_I2C_IsDeviceReady(&hi2c3, (i2c_address << 1), 1, 10);

        if (result == HAL_OK)
        {
            char msg[32];
            snprintf(msg, sizeof(msg), "Found device at 0x%02X\r\n", i2c_address);
            uart_send(msg);
        }
    }
    tx_mutex_put(&i2c_mutex);
    uart_send("I2C bus scan complete.\r\n");
    return (HAL_OK);
}