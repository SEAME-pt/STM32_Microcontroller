#include "i2c_pca9685.h"
#include "can_protocol.h"

VOID    thread_servo(ULONG initial_input)
{
    
    t_i2c_msg    msg;
    memset(&msg, 0, sizeof(t_i2c_msg));
    
    if (pca9685_init(PCA9685_ADDR_SERVO) != HAL_OK) {
        uart_send("Servo Thread: PCA9685 initialization failed\r\n");
        return ;
    }

    while (1)
    {
        if (tx_queue_receive(&i2c_servo_queue, &msg, TX_WAIT_FOREVER) == TX_SUCCESS)
        {
            if (pca9685_set_servo_angle(0, msg.steering) != HAL_OK)
                uart_send("Servo Thread: Failed to set servo angle\r\n");
        }
    }
}
