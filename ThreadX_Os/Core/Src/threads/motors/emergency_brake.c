#include "i2c_pca9685.h"
#include "can_protocol.h"

VOID  thread_emergency_brake(ULONG thread_input) 
{
    t_rx_can_msg    msg;
    //t_rx_can_msg    last_msg;

    tx_thread_sleep(100); // Allow other threads to initialize
    while (1) 
    {
        if (tx_queue_receive(&can_emergency_brake_queue, &msg, TX_WAIT_FOREVER) == TX_SUCCESS) 
        {
            motor_set(MOTOR_LEFT, 0, 1);
            motor_set(MOTOR_RIGHT, 0, 1);
            tx_thread_sleep(500);
        }
    }
}