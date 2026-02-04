#ifndef I2C_PCA9685_H
# define I2C_PCA9685_H

#include "app_threadx.h"

#define PCA9685_ADDR_SERVO  0x40
#define PCA9685_ADDR_MOTOR  0x60

// PWM resolution and channel range
#define PCA9685_PWM_MAX        4095

// PCA9685 channel limits
#define PCA9685_CHANNEL_MIN  0
#define PCA9685_CHANNEL_MAX  15

// PCA9685 register addresses
#define MODE1       0x00
#define MODE2       0x01
#define PRE_SCALE   0xFE

// LED0 registers
#define LED0_ON_L   0x06
#define LED0_ON_H   0x07
#define LED0_OFF_L  0x08
#define LED0_OFF_H  0x09

// PCA9685 mode settings
#define PCA9685_SLEEP_MODE      0x10
#define PCA9685_50HZ_PRESCALE   0x79
#define PCA9685_1KHZ_PRESCALE   0x05
#define PCA9685_WAKE_AUTOINC    0x20

// Servo pulse limits
#define SERVO_MIN_PULSE 205
#define SERVO_MAX_PULSE 410
#define SERVO_MAX_ANGLE 180

// Motor speed limits
#define MOTOR_MAX_SPEED 100

#define MOTOR_PWM_MIN   0

typedef struct s_motor_channel {
    UINT pwm_ch;   // PWM motor channel
    UINT in1_ch;   // IN1 channel
    UINT in2_ch;   // IN2 channel
} t_motor_channel;

// Predefined motor channels
static const t_motor_channel MOTOR_LEFT  = { 0, 1, 2 };
static const t_motor_channel MOTOR_RIGHT = { 7, 5, 6 };

HAL_StatusTypeDef   pca9685_init(UINT addr);

HAL_StatusTypeDef   pca9685_set_pwm(UINT channel, 
    uint16_t on, uint16_t off, UINT addr);

HAL_StatusTypeDef   pca9685_set_servo_angle(UINT channel, UINT angle);

HAL_StatusTypeDef   motor_set(t_motor_channel motor, int8_t speed, uint8_t brake);

HAL_StatusTypeDef   stopMotors(VOID);

#endif
