#ifndef I2C_PCA9685_H
# define I2C_PCA9685_H

#include "app_threadx.h"

#define PCA9685_ADDR_SERVO  0x40
#define PCA9685_ADDR_MOTOR  0x60

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

#define SERVO_MIN_PULSE 205
#define SERVO_MAX_PULSE 410
#define SERVO_MAX_ANGLE 180

// Motor speed limits
#define MOTOR_MAX_SPEED 100

#define MOTOR_PWM_MIN   0

typedef struct s_motor_channel {
    uint8_t pwm_ch;   // Canal PWM do motor
    uint8_t in1_ch;   // Canal IN1
    uint8_t in2_ch;   // Canal IN2
} t_motor_channel;

HAL_StatusTypeDef   i2c_scan_bus(VOID);

HAL_StatusTypeDef   pca9685_init(I2C_HandleTypeDef *hi2c, uint8_t addr);

HAL_StatusTypeDef   pca9685_set_pwm(I2C_HandleTypeDef *hi2c, uint8_t channel, 
    uint16_t on, uint16_t off, uint32_t addr);

HAL_StatusTypeDef   pca9685_set_servo_angle(I2C_HandleTypeDef *hi2c, 
    uint8_t channel, uint8_t angle);

// Exemplo de mapeamento: Motor esquerdo e direito
static const t_motor_channel MOTOR_LEFT  = { 0, 1, 2 };
static const t_motor_channel MOTOR_RIGHT = { 3, 4, 5 };

HAL_StatusTypeDef motor_set(I2C_HandleTypeDef *hi2c, t_motor_channel motor, int16_t speed);

#endif
