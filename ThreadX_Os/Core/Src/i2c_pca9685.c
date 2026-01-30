#include "i2c_pca9685.h"

static HAL_StatusTypeDef    error_return(const char *msg) {
    if (!msg)
        uart_send("PCA9685: I2C communication error\r\n");
    else
        uart_send(msg);
    return (HAL_ERROR);
}

HAL_StatusTypeDef   pca9685_init(UINT addr) {

    HAL_StatusTypeDef   status;
    uint8_t             data;

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);
    
    i2c_scan_bus();
    // Set to sleep mode to configure prescaler
    data = PCA9685_SLEEP_MODE;
    status = HAL_I2C_Mem_Write(&hi2c3, addr << 1,
        MODE1, I2C_MEMADD_SIZE_8BIT,
        &data, 1, 100);
    if (status != HAL_OK) {
        tx_mutex_put(&i2c_mutex);
        return (error_return("PCA9685: Failed to enter sleep mode\r\n"));
    }

    if (addr == PCA9685_ADDR_SERVO) 
    {
        // 50Hz for servos
        data = PCA9685_50HZ_PRESCALE;
        status = HAL_I2C_Mem_Write(&hi2c3, addr << 1,
            PRE_SCALE, I2C_MEMADD_SIZE_8BIT,
            &data, 1, 100);
        if (status != HAL_OK) {
            tx_mutex_put(&i2c_mutex);
            return (error_return("PCA9685: Failed to set prescaler\r\n"));
        }
    }
    else if (addr == PCA9685_ADDR_MOTOR) 
    {
        // 1KHz for DC motors
        data = PCA9685_1KHZ_PRESCALE;
        status = HAL_I2C_Mem_Write(&hi2c3, addr << 1,
            PRE_SCALE, I2C_MEMADD_SIZE_8BIT,
            &data, 1, 100);
        if (status != HAL_OK) {
            tx_mutex_put(&i2c_mutex);
            return (error_return("PCA9685: Failed to set prescaler\r\n"));
        }
    }

    // Wake up & auto increment
    data = PCA9685_WAKE_AUTOINC;
    status = HAL_I2C_Mem_Write(&hi2c3, addr << 1,
        MODE1, I2C_MEMADD_SIZE_8BIT,
        &data, 1, 100);
    if (status != HAL_OK) {
        tx_mutex_put(&i2c_mutex);
        return (error_return("PCA9685: Failed to exit sleep mode\r\n"));
    }

    tx_mutex_put(&i2c_mutex);
    return (status);
}

HAL_StatusTypeDef pca9685_set_servo_angle(UINT channel, UINT angle)
{
    uint16_t    pulse;

    if (angle > SERVO_MAX_ANGLE)
        angle = SERVO_MAX_ANGLE;

    pulse = SERVO_MIN_PULSE +
        ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle)
        / SERVO_MAX_ANGLE;

    return (pca9685_set_pwm(channel, 0, pulse, PCA9685_ADDR_SERVO));
}

HAL_StatusTypeDef   pca9685_set_pwm(UINT channel, 
    uint16_t on, uint16_t off, UINT addr)
{
    HAL_StatusTypeDef   status;
    uint8_t             buf[4];
    uint8_t             reg;

    // Buffer preparation with LSB / MSB
    buf[0] = on & 0xFF;         // ON_L
    buf[1] = (on >> 8) & 0x0F;  // ON_H
    buf[2] = off & 0xFF;        // OFF_L
    buf[3] = (off >> 8) & 0x0F; // OFF_H

    reg = LED0_ON_L + 4 * channel;

    tx_mutex_get(&i2c_mutex, TX_WAIT_FOREVER);

    status = HAL_I2C_Mem_Write(&hi2c3, addr << 1,
        reg, I2C_MEMADD_SIZE_8BIT,
        buf, 4, 500);

    if (status != HAL_OK) {
        tx_mutex_put(&i2c_mutex);
        return (error_return("PCA9685: Failed to set PWM\r\n"));
    }

    tx_mutex_put(&i2c_mutex);
    return (HAL_OK);
}

HAL_StatusTypeDef motor_set(t_motor_channel motor, int8_t speed, uint8_t brake)
{
    uint16_t pwm;

    // Fast stop (active brake)
    if (brake) {
        // Both IN1 and IN2 high, PWM 0
        if (pca9685_set_pwm(motor.in1_ch, 0, PCA9685_PWM_MAX, PCA9685_ADDR_MOTOR) != HAL_OK) {
            return (error_return("PCA9685: Failed to brake motor (in1)\r\n"));
        }
        if (pca9685_set_pwm(motor.in2_ch, 0, PCA9685_PWM_MAX, PCA9685_ADDR_MOTOR) != HAL_OK) {
            return (error_return("PCA9685: Failed to brake motor (in2)\r\n"));
        }
        if (pca9685_set_pwm(motor.pwm_ch, 0, 0, PCA9685_ADDR_MOTOR) != HAL_OK) {
            return (error_return("PCA9685: Failed to brake motor (pwm)\r\n"));
        }
        return (HAL_OK);
    }

    // Speed limits
    if (speed > MOTOR_MAX_SPEED) speed = MOTOR_MAX_SPEED;
    if (speed < -MOTOR_MAX_SPEED) speed = -MOTOR_MAX_SPEED;

    // Stop motor (coast)
    if (speed == 0) {
        if (pca9685_set_pwm(motor.in1_ch, 0, 0, PCA9685_ADDR_MOTOR) != HAL_OK) {
            return (error_return("PCA9685: Failed to stop motor (in1)\r\n"));
        }
        if (pca9685_set_pwm(motor.in2_ch, 0, 0, PCA9685_ADDR_MOTOR) != HAL_OK) {
            return (error_return("PCA9685: Failed to stop motor (in2)\r\n"));
        }
        if (pca9685_set_pwm(motor.pwm_ch, 0, 0, PCA9685_ADDR_MOTOR) != HAL_OK) {
            return (error_return("PCA9685: Failed to stop motor (pwm)\r\n"));
        }
        return (HAL_OK);
    }

    // Determine direction
    if (speed > 0) {
        // Forward
        if (pca9685_set_pwm(motor.in1_ch, 0, PCA9685_PWM_MAX, PCA9685_ADDR_MOTOR) != HAL_OK) {
            return (error_return("PCA9685: Failed to set motor direction (in1)\r\n"));
        }
        if (pca9685_set_pwm(motor.in2_ch, 0, MOTOR_PWM_MIN, PCA9685_ADDR_MOTOR) != HAL_OK) {
            return (error_return("PCA9685: Failed to set motor direction (in2)\r\n"));
        }
    } else {
        // Reverse
        speed = -speed;
        if (pca9685_set_pwm(motor.in1_ch, 0, MOTOR_PWM_MIN, PCA9685_ADDR_MOTOR) != HAL_OK) {
            return (error_return("PCA9685: Failed to set motor direction (in1)\r\n"));
        }
        if (pca9685_set_pwm(motor.in2_ch, 0, PCA9685_PWM_MAX, PCA9685_ADDR_MOTOR) != HAL_OK) {
            return (error_return("PCA9685: Failed to set motor direction (in2)\r\n"));
        }
    }

    // PWM proportional to speed
    pwm = (PCA9685_PWM_MAX * speed) / MOTOR_MAX_SPEED;

    if (pca9685_set_pwm(motor.pwm_ch, 0, pwm, PCA9685_ADDR_MOTOR) != HAL_OK)
        return (error_return("PCA9685: Failed to set motor PWM\r\n"));

    return (HAL_OK);
}

HAL_StatusTypeDef stopMotors(VOID)
{
    HAL_StatusTypeDef status;

    status = motor_set(MOTOR_LEFT, 0, 1);
    if (status != HAL_OK)
        return (status);

    status = motor_set(MOTOR_RIGHT, 0, 1);
    if (status != HAL_OK)
        return (status);

    return (HAL_OK);
}
