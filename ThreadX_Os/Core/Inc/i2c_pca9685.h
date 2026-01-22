#ifndef I2C_PCA9685_H
# define I2C_PCA9685_H

#include "app_threadx.h"

#define PCA9685_I2C_ADDR  0x40

#define PCA9685_PWM_RESOLUTION 4096
#define PCA9685_PWM_MAX        4095

#define PCA9685_CHANNEL_MIN  0
#define PCA9685_CHANNEL_MAX  15

#define MODE1 0x00
#define LED0_ON_L 0x06
#define PRE_SCALE 0xFE


HAL_StatusTypeDef   pca9685_init(void);
HAL_StatusTypeDef   pca9685_set_pwm(I2C_HandleTypeDef *hi2c, uint8_t channel, uint8_t addr7);

HAL_StatusTypeDef   pca9685_set_servo_angle(I2C_HandleTypeDef *hi2c, uint8_t channel, uint8_t angle);

void                i2c_scan_bus(void);

#endif
