#include "can_protocol.h"

static const uint8_t dlc_to_len[16] = {0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64};

// CAN RX callback function
HAL_StatusTypeDef rx_receive(t_rx_can_msg *msg)
{
    FDCAN_RxHeaderTypeDef   rxHeader;
    uint8_t                 rx_data[8];

    if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) > 0)
    {
        if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rxHeader, rx_data) == HAL_OK)
        {
            msg->type = rxHeader.Identifier;
            msg->len = (rxHeader.DataLength < 16) ? dlc_to_len[rxHeader.DataLength] : 8;
            memcpy(&msg->data, rx_data, msg->len);
            return (HAL_OK); // Success
        }
    }
    return (HAL_ERROR); // Failure
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
                case 0x100: // Emergency break
                    tx_queue_send(&can_rx_queue, &msg, TX_NO_WAIT);
                    //uart_send("Received Emergency break msg\r\n");
                    break ;
                case 0x101: // throttle
                    tx_queue_send(&i2c_dc_motors_queue, &msg, TX_NO_WAIT);
                    //uart_send("Received CAN MSG THROTTLE\r\n");
                    break ;
                case 0x102: // Steering
                    tx_queue_send(&i2c_servo_queue, &msg, TX_NO_WAIT);
                    //uart_send("Received CAN MSG STEERING\r\n");
                    break ;
                default:
                    uart_send("Received UNKNOWN CAN MSG\r\n");
                    break ;
            }
        }
        tx_thread_sleep(1);
    }
}
