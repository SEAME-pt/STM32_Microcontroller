#include "app_threadx.h"
#include "can_protocol.h"
#include "i2c_pca9685.h"

VOID  thread_emergency_brake(ULONG thread_input) 
{
    t_rx_can_msg    msg;
    t_rx_can_msg    msg_to_send;
    memset(&msg, 0, sizeof(t_rx_can_msg));
    memset(&msg_to_send, 0, sizeof(t_rx_can_msg));

    msg_to_send.data[0] = EMERGENCY_BRAKE;

    while (1)
    {
        // Wait for emergency brake message from Raspberry Pi
        if (tx_queue_receive(&emergency_brake_queue, &msg, TX_WAIT_FOREVER) == TX_SUCCESS) {
            //uart_send("Emergency message sent to i2c thread\r\n");
            tx_queue_send(&i2c_driving_queue, &msg_to_send, TX_NO_WAIT);
        }
    }
}
