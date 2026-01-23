#include "i2c_pca9685.h"

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

HAL_StatusTypeDef   pca9685_init(I2C_HandleTypeDef *hi2c, uint8_t addr) {

    HAL_StatusTypeDef   status = HAL_OK;
    uint8_t             data;

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);
    
    // Set to sleep mode to configure prescaler
    data = 0x10;
    status = HAL_I2C_Mem_Write(hi2c, addr << 1,
        MODE1, I2C_MEMADD_SIZE_8BIT,
        &data, 1, 100);
    if (status != HAL_OK) {
        uart_send("PCA9685: Failed to enter sleep mode\r\n");
        tx_mutex_put(&i2c_mutex);
        return (status);
    }

    // 50Hz
    data = 0x79;
    status = HAL_I2C_Mem_Write(hi2c, addr << 1,
        PRE_SCALE, I2C_MEMADD_SIZE_8BIT,
        &data, 1, 100);
    if (status != HAL_OK) {
        uart_send("PCA9685: Failed to set prescaler\r\n");
        tx_mutex_put(&i2c_mutex);
        return (status);
    }

    // Wake up & auto increment
    data = 0x20;
    status = HAL_I2C_Mem_Write(hi2c, addr << 1,
        MODE1, I2C_MEMADD_SIZE_8BIT,
        &data, 1, 100);
    if (status != HAL_OK) {
        uart_send("PCA9685: Failed to exit sleep mode\r\n");
        tx_mutex_put(&i2c_mutex);
        return (status);
    }

    tx_mutex_put(&i2c_mutex);
    return (status);
}

HAL_StatusTypeDef pca9685_set_servo_angle(I2C_HandleTypeDef *hi2c,
    uint8_t channel, uint8_t angle)
{
    uint16_t pulse;

    if (angle > SERVO_MAX_ANGLE)
        angle = SERVO_MAX_ANGLE;

    pulse = SERVO_MIN_PULSE +
        ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle)
        / SERVO_MAX_ANGLE;

    return pca9685_set_pwm(hi2c, channel, 0, pulse, PCA9685_ADDR_SERVO);
}

HAL_StatusTypeDef   pca9685_set_pwm(I2C_HandleTypeDef *hi2c, uint8_t channel, 
    uint16_t on, uint16_t off, uint32_t addr)
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

    status = HAL_I2C_Mem_Write(hi2c, addr << 1,
        reg, I2C_MEMADD_SIZE_8BIT,
        buf, 4, 500);

    if (status != HAL_OK) {
        uart_send("PCA9685: Failed to set PWM\r\n");
        tx_mutex_put(&i2c_mutex);
        return (status);
    }

    tx_mutex_put(&i2c_mutex);
    return (HAL_OK);
}

HAL_StatusTypeDef motor_set(I2C_HandleTypeDef *hi2c, t_motor_channel motor, int16_t speed)
{
    uint16_t pwm;

    // Speed limits
    if (speed > MOTOR_MAX_SPEED) speed = MOTOR_MAX_SPEED;
    if (speed < -MOTOR_MAX_SPEED) speed = -MOTOR_MAX_SPEED;

    // Stop motor
    if (speed == 0) {
        pca9685_set_pwm(hi2c, motor.in1_ch, 0, 0, PCA9685_ADDR_MOTOR);
        pca9685_set_pwm(hi2c, motor.in2_ch, 0, 0, PCA9685_ADDR_MOTOR);
        pca9685_set_pwm(hi2c, motor.pwm_ch, 0, 0, PCA9685_ADDR_MOTOR);
        return HAL_OK;
    }

    // Determine direction
    if (speed > 0) {
        // Front
        pca9685_set_pwm(hi2c, motor.in1_ch, 0, PCA9685_PWM_MAX, PCA9685_ADDR_MOTOR);
        pca9685_set_pwm(hi2c, motor.in2_ch, 0, MOTOR_PWM_MIN, PCA9685_ADDR_MOTOR);
    } else {
        // Back
        speed = -speed;
        pca9685_set_pwm(hi2c, motor.in1_ch, 0, MOTOR_PWM_MIN, PCA9685_ADDR_MOTOR);
        pca9685_set_pwm(hi2c, motor.in2_ch, 0, PCA9685_PWM_MAX, PCA9685_ADDR_MOTOR);
    }

    // Scale PWM proportional to speed
    pwm = (PCA9685_PWM_MAX * speed) / MOTOR_MAX_SPEED;
    pca9685_set_pwm(hi2c, motor.pwm_ch, 0, pwm, PCA9685_ADDR_MOTOR);

    return HAL_OK;
}