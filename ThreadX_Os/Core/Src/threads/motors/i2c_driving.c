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
                // Extract timestamp and calculate elapsed time
                ULONG start_time = (ULONG)msg.data[4] | 
                                   ((ULONG)msg.data[5] << 8) | 
                                   ((ULONG)msg.data[6] << 16) | 
                                   ((ULONG)msg.data[7] << 24);
                ULONG current_time = tx_time_get();
                ULONG elapsed_ticks = current_time - start_time;
                
                char time_msg[100];
                sprintf(time_msg, "EMERGENCY BRAKE ACTIVATED! Latency: %lu ticks (%.2f ms)\r\n", 
                        elapsed_ticks, (float)elapsed_ticks * 1000.0f / TX_TIMER_TICKS_PER_SECOND);
                uart_send(time_msg);
                
                if (motor_set(MOTOR_LEFT, 0, 1) != HAL_OK)
                    uart_send("Driving Command Thread: Failed to set emergency break in LEFT motor\r\n");
                if (motor_set(MOTOR_RIGHT, 0, 1) != HAL_OK)
                    uart_send("Driving Command Thread: Failed to set emergency break in RIGHT motor\r\n");
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
