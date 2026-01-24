#include "i2c_pca9685.h"
#include "can_protocol.h"

VOID thread_dc_motors(ULONG initial_input)
{
    t_i2c_msg    msg;
    memset(&msg, 0, sizeof(t_i2c_msg));

    //uart_send("DC Motors Thread: Setting motor speeds\r\n");
    if (pca9685_init(PCA9685_ADDR_MOTOR) != HAL_OK) {
        uart_send("DC Motors Thread: PCA9685 initialization failed\r\n");
        return ;
    }
    while (1)
    {
        if (tx_queue_receive(&i2c_dc_motors_queue, &msg, TX_WAIT_FOREVER) == TX_SUCCESS)
        {
            if (motor_set(MOTOR_LEFT, msg.throttle) != HAL_OK) {
                uart_send("DC Motors Thread: Failed to set LEFT motor speed\r\n");
            }
            if (motor_set(MOTOR_RIGHT, msg.throttle) != HAL_OK) {
                uart_send("DC Motors Thread: Failed to set RIGHT motor speed\r\n");
            }
        }
    }
    //test motors
    motor_set(MOTOR_LEFT, -100);
    motor_set(MOTOR_RIGHT, -100);
}
