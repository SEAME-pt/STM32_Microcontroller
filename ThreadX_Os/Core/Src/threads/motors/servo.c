#include "i2c_pca9685.h"

VOID    thread_servo(ULONG initial_input)
{
    i2c_scan_bus();
    if (pca9685_init(&hi2c3, PCA9685_ADDR_SERVO) != HAL_OK) {
        uart_send("Servo Thread: PCA9685 initialization failed\r\n");
    } else {
        uart_send("Servo Thread: PCA9685 initialized successfully\r\n");
    }
    while (1)
    {
        pca9685_set_servo_angle(&hi2c3, 0, 0);
        tx_thread_sleep(100);

        pca9685_set_servo_angle(&hi2c3, 0, 90);
        tx_thread_sleep(100);

        pca9685_set_servo_angle(&hi2c3, 0, 180);
        tx_thread_sleep(100);
    }
}