#include "app_threadx.h"
#include "can_protocol.h"

VOID  thread_heartbeat(ULONG thread_input) {
    t_tx_can_msg    heartbeat_msg;
    t_tx_can_msg    msg;
    memset(&heartbeat_msg, 0, sizeof(t_tx_can_msg));
    memset(&msg, 0, sizeof(t_tx_can_msg));

    msg.data[0] = 0x01;
    msg.type = CAN_MSG_HEARTBEAT;

    while (1)
    {
        // Sends heartbeat message to CAN thread
        if (tx_queue_receive(&heartbeat_queue, &heartbeat_msg, TX_MS_TO_TICKS(1000)) != TX_SUCCESS)
            uart_send("Heartbeat timeout! No heartbeat received in the last second.\r\n");

        if (tx_queue_send(&can_tx_queue, &msg, TX_NO_WAIT) != TX_SUCCESS)
            uart_send("CAN TX could not add heartbeat message to queue!\r\n");
    }
}
