#ifndef I2C_PCA9685_H
# define I2C_PCA9685_H

#include "app_threadx.h"

#define PCA9685_ADDR  0x40

// PWM resolution and channel range
#define PCA9685_PWM_RESOLUTION 4096
#define PCA9685_PWM_MAX        4095

// PCA9685 channel limits
#define PCA9685_CHANNEL_MIN  0
#define PCA9685_CHANNEL_MAX  15

// PCA9685 register addresses
#define MODE1       0x00
#define MODE2       0x01
#define PRE_SCALE   0xFE
#define LED0_ON_L   0x06
#define LED0_OFF_L  0x08

HAL_StatusTypeDef   pca9685_init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef   pca9685_set_pwm(I2C_HandleTypeDef *hi2c, uint8_t channel, uint16_t on, uint16_t off);

HAL_StatusTypeDef   pca9685_set_servo_angle(I2C_HandleTypeDef *hi2c, uint8_t channel, uint8_t angle);

#endif
