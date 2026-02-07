#include "can_protocol.h"

// Function responsible to transmit CAN messages.
VOID thread_tx_can(ULONG thread_input)
{
    t_tx_can_msg    msg;
    t_canFrames     canFrames;

    memset(&msg, 0, sizeof(t_tx_can_msg));
    memset(&canFrames, 0, sizeof(t_canFrames));
    initCanFrames(&canFrames);
    if (!canFrames.tx_header_speed.Identifier ||
            !canFrames.tx_header_battery.Identifier) {
        uart_send("CAN frames not initialized!\r\n");
        exit(EXIT_FAILURE);
    }
    while (1) 
    {
        //constant wait for messages from other threads
        if (tx_queue_receive(&can_tx_queue, &msg, TX_WAIT_FOREVER) == TX_SUCCESS)
        {
            switch (msg.type)
            {
                case CAN_MSG_SPEED:
                    HAL_FDCAN_AddMessageToTxFifoQ(
                    &hfdcan1,
                    &canFrames.tx_header_speed,
                    msg.data);
                    uart_send("Speed CAN message sent\r\n");
                    break ;

                case CAN_MSG_BATTERY:
                    HAL_FDCAN_AddMessageToTxFifoQ(
                        &hfdcan1,
                        &canFrames.tx_header_battery,
                        msg.data);
                    uart_send("Battery CAN message sent\r\n");
                    break ;

                default:
                    uart_send("Impossible to send this message, unknown ID...\r\n");
                    break ;
            }
        }
    }
}
