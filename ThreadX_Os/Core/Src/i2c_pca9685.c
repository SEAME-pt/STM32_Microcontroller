#include "i2c_pca9685.h"

HAL_StatusTypeDef   pca9685_init(I2C_HandleTypeDef *hi2c, UINT addr) {

    HAL_StatusTypeDef   status = HAL_OK;
    uint8_t             data;

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);
    
    // Set to sleep mode to configure prescaler
    data = PCA9685_SLEEP_MODE;
    status = HAL_I2C_Mem_Write(hi2c, addr << 1,
        MODE1, I2C_MEMADD_SIZE_8BIT,
        &data, 1, 100);
    if (status != HAL_OK) {
        uart_send("PCA9685: Failed to enter sleep mode\r\n");
        tx_mutex_put(&i2c_mutex);
        return (status);
    }

    // 50Hz
    data = PCA9685_50HZ_PRESCALE;
    status = HAL_I2C_Mem_Write(hi2c, addr << 1,
        PRE_SCALE, I2C_MEMADD_SIZE_8BIT,
        &data, 1, 100);
    if (status != HAL_OK) {
        uart_send("PCA9685: Failed to set prescaler\r\n");
        tx_mutex_put(&i2c_mutex);
        return (status);
    }

    // Wake up & auto increment
    data = PCA9685_WAKE_AUTOINC;
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
    UINT channel, UINT angle)
{
    uint16_t pulse;

    if (angle > SERVO_MAX_ANGLE)
        angle = SERVO_MAX_ANGLE;

    pulse = SERVO_MIN_PULSE +
        ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle)
        / SERVO_MAX_ANGLE;

    return pca9685_set_pwm(hi2c, channel, 0, pulse, PCA9685_ADDR_SERVO);
}

HAL_StatusTypeDef   pca9685_set_pwm(I2C_HandleTypeDef *hi2c, UINT channel, 
    uint16_t on, uint16_t off, UINT addr)
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

HAL_StatusTypeDef motor_set(I2C_HandleTypeDef *hi2c, t_motor_channel motor, int8_t speed)
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
        return (HAL_OK);
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

    return (HAL_OK);
}