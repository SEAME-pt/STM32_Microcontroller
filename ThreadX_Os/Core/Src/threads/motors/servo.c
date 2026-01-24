#include "i2c_pca9685.h"
#include "can_protocol.h"

VOID    thread_servo(ULONG initial_input)
{
    if (pca9685_init(PCA9685_ADDR_SERVO) != HAL_OK) {
        uart_send("Servo Thread: PCA9685 initialization failed\r\n");
        return ;
    }

    while (1)
    {
        pca9685_set_servo_angle(0, 0);
        tx_thread_sleep(100);

        pca9685_set_servo_angle(0, 90);
        tx_thread_sleep(100);

        pca9685_set_servo_angle(0, 180);
        tx_thread_sleep(100);
    }
}