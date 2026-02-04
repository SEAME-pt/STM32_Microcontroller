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
                    if (tx_queue_send(&can_emergency_brake_queue, &msg, TX_NO_WAIT) != TX_SUCCESS)
                        uart_send("ERROR: Emergency brake queue FULL!\r\n");
                    break ;
                case 0x101: // throttle
                    if (tx_queue_send(&i2c_dc_motors_queue, &msg, TX_NO_WAIT) != TX_SUCCESS)
                        uart_send("ERROR: DC motors queue FULL!\r\n");
                    uart_send("DC motor CAN msg received\r\n");
                    break ;
                case 0x102: // Steering
                    if (tx_queue_send(&i2c_servo_queue, &msg, TX_NO_WAIT) != TX_SUCCESS)
                        uart_send("ERROR: Servo queue FULL!\r\n");
                    uart_send("Servo CAN msg received\r\n");
                    break ;
                case 0x103:
                    
                default:
                    uart_send("Received UNKNOWN CAN MSG\r\n");
                    break ;
            }
        }
        tx_thread_sleep(1); // Sleep for 1 tick to avoid busy waiting
    }
}
