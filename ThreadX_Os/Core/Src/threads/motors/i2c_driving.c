#include "i2c_pca9685.h"
#include "can_protocol.h"

VOID thread_driving_command(ULONG initial_input)
{
    t_rx_can_msg    msg;
    t_rx_can_msg    last_msg;

    if (pca9685_init(PCA9685_ADDR_MOTOR) != HAL_OK ||
        pca9685_init(PCA9685_ADDR_SERVO) != HAL_OK)
    {
        uart_send("Driving Command Thread: PCA9685 initialization failed\r\n");
        return ;
    } else 
        uart_send("Driving Command Thread: PCA9685 initialized successfully\r\n");

    while (1)
    {
        // waits permanently for a new message in the queue
        if (tx_queue_receive(&i2c_driving_queue, &msg, TX_WAIT_FOREVER) == TX_SUCCESS)
        {
            // Empties the queue and always takes the last available message
            last_msg = msg;
            while (tx_queue_receive(&i2c_driving_queue, &last_msg, TX_NO_WAIT) == TX_SUCCESS)
                msg = last_msg;

            if (msg.data[0] == EMERGENCY_BRAKE)
            {
                uart_send("EMERGENCY BREAK ACTIVATED!\r\n");
                if (motor_set(MOTOR_LEFT, 0, 1) != HAL_OK)
                    uart_send("Driving Command Thread: Failed to set emergency break in LEFT motor\r\n");
                if (motor_set(MOTOR_RIGHT, 0, 1) != HAL_OK)
                    uart_send("Driving Command Thread: Failed to set emergency break in RIGHT motor\r\n");
                tx_thread_sleep(TX_MS_TO_TICKS(100));
            }
            else if (msg.len >= 4)
            {
                int16_t throttle = (int16_t)(msg.data[0] | (msg.data[1] << 8));
                int16_t steering = (int16_t)(msg.data[2] | (msg.data[3] << 8));

                // Control motors
                if (motor_set(MOTOR_LEFT, -throttle, 0) != HAL_OK)
                    uart_send("Driving Command Thread: Failed to set LEFT motor speed\r\n");
                if (motor_set(MOTOR_RIGHT, throttle, 0) != HAL_OK)
                    uart_send("Driving Command Thread: Failed to set RIGHT motor speed\r\n");

                // Control servo
                if (pca9685_set_servo_angle(0, steering) != HAL_OK)
                    uart_send("Driving Command Thread: Failed to set servo angle\r\n");
            }
        }
    }
}
