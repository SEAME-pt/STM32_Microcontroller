#include "can_protocol.h"

// CAN RX callback function
static HAL_StatusTypeDef    rx_receive(t_rx_can_msg *msg)
{
    FDCAN_RxHeaderTypeDef   rxHeader;
    uint8_t                 rx_data[8];

    if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) > 0)
    {
        if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rxHeader, rx_data) == HAL_OK)
        {
            msg->type = rxHeader.Identifier;
            msg->len = (rxHeader.DataLength < 8) ? rxHeader.DataLength : 8;
            memcpy(&msg->data, rx_data, msg->len);
            return (HAL_OK);
        }
    }
    return (HAL_ERROR);
}

// THREAD - responsible to receive CAN messages
VOID    thread_rx_can(ULONG thread_input)
{
    t_rx_can_msg    msg;
    memset(&msg, 0, sizeof(t_rx_can_msg));

    while (1)
    {
        if (rx_receive(&msg) == HAL_OK)
        {
            switch(msg.type) 
            {
                case 0x100: // Emergency brake
                    // Store timestamp in data bytes 4-7 (uint32_t)
                    {
                        ULONG timestamp = tx_time_get();
                        msg.data[4] = (timestamp >> 0) & 0xFF;
                        msg.data[5] = (timestamp >> 8) & 0xFF;
                        msg.data[6] = (timestamp >> 16) & 0xFF;
                        msg.data[7] = (timestamp >> 24) & 0xFF;
                    }
                    if (tx_queue_send(&emergency_brake_queue, &msg, TX_NO_WAIT) != TX_SUCCESS)
                        uart_send("ERROR: emergency brake queue FULL!\r\n");
                    uart_send("Emergency brake CAN msg received\r\n");
                    break ;

                case 0x101: // driving command
                    if (tx_queue_send(&i2c_driving_queue, &msg, TX_NO_WAIT) != TX_SUCCESS)
                        uart_send("ERROR: Driving command queue FULL!\r\n");
                    //uart_send("Driving command CAN msg received\r\n");
                    break ;

                default:
                    //uart_send("Received UNKNOWN CAN MSG\r\n");
                    break ;
            }
        }
        tx_thread_sleep(1);
    }
}
