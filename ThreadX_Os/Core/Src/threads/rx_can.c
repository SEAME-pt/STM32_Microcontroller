#include "app_threadx.h"

// CAN RX callback function
uint8_t rx_receive(t_rx_can_msg *msg)
{
    FDCAN_RxHeaderTypeDef   rxHeader;
    uint8_t                 rx_data[8];

    if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) > 0)
    {
        if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rxHeader, rx_data) == HAL_OK)
        {
            msg->type = rxHeader.Identifier;
            msg->len = (rxHeader.DataLength <= 8) ? rxHeader.DataLength : 8;
            memcpy(msg->data, rx_data, msg->len);
            return (1); // Success
        }
    }
    return (0); // Failure
}

// THREAD - responsible to receive CAN messages
VOID    thread_rx_can(ULONG thread_input)
{
    t_rx_can_msg    msg;
    memset(&msg, 0, sizeof(t_rx_can_msg));

    uart_send("CAN RX thread started\r\n");
    while (1)
    {
        if (rx_receive(&msg)) 
        {
            switch(msg.type) 
            {
                case 0x100: // Emergency break
                    tx_queue_send(&can_rx_queue, &msg, TX_NO_WAIT);
                    uart_send("Received Emergency break msg\r\n");
                    break ;
                case 0x101: // Steering and throttle
                    tx_queue_send(&can_rx_queue, &msg, TX_NO_WAIT);
                    uart_send("Received CAN MSG STEERING THROTTLE\r\n");
                    break ;
                default:
                    uart_send("Received UNKNOWN CAN MSG\r\n");
                    break ;
            }
        }
        tx_thread_sleep(1);
    }
}
