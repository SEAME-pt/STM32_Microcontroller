#include "i2c_pca9685.h"
#include "can_protocol.h"

VOID thread_dc_motors(ULONG initial_input)
{
    t_rx_can_msg    msg;
    t_rx_can_msg    last_msg;

    if (pca9685_init(PCA9685_ADDR_MOTOR) != HAL_OK) 
    {
        uart_send("DC Motors Thread: PCA9685 initialization failed\r\n");
        return ;
    } else 
        uart_send("DC Motors Thread: PCA9685 initialized successfully\r\n");

    while (1)
    {
        // waits permanently for a new message in the queue
        if (tx_queue_receive(&i2c_dc_motors_queue, &msg, TX_WAIT_FOREVER) == TX_SUCCESS)
        {
            // Empties the queue and always takes the last available message
            last_msg = msg;
            while (tx_queue_receive(&i2c_dc_motors_queue, &last_msg, TX_NO_WAIT) == TX_SUCCESS)
                msg = last_msg;

            if (msg.len >= 2)
            {
                int16_t throttle = (int16_t)(msg.data[0] | (msg.data[1] << 8));
                if (motor_set(MOTOR_LEFT, -throttle, 0) != HAL_OK)
                    uart_send("DC Motors Thread: Failed to set LEFT motor speed\r\n");

                if (motor_set(MOTOR_RIGHT, throttle, 0) != HAL_OK)
                    uart_send("DC Motors Thread: Failed to set RIGHT motor speed\r\n");
            }
        }
    }
}
