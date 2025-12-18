#include "app_threadx.h"

// Function responsible to transmit CAN messages.
VOID thread_tx_can(ULONG thread_input)
{
    //can message to send
    t_can_msg   msg;
    //can frames configuration
    t_canFrames canFrames;

    memset(&msg, 0, sizeof(t_can_msg));
    memset(&canFrames, 0, sizeof(t_canFrames));
    initCanFrames(&canFrames);
    if (canFrames.tx_header_speed.Identifier == 0 &&
            canFrames.tx_header_steering_throttle.Identifier == 0 &&
            canFrames.tx_header_battery.Identifier == 0) {
        uart_send("CAN frames not initialized!\r\n");
        return ;
    }
    while (1) 
    {
        //constant wait for messages from other threads
        tx_queue_receive(&can_tx_queue, &msg, TX_WAIT_FOREVER);

        switch (msg.type)
        {
            case CAN_MSG_SPEED:
                HAL_FDCAN_AddMessageToTxFifoQ(
                    &hfdcan1,
                    &canFrames.tx_header_speed,
                    msg.data
                );
                break;

            case CAN_MSG_STEERING_THROTTLE:
                HAL_FDCAN_AddMessageToTxFifoQ(
                    &hfdcan1,
                    &canFrames.tx_header_steering_throttle,
                    msg.data
                );
                break;

            case CAN_MSG_BATTERY:
                HAL_FDCAN_AddMessageToTxFifoQ(
                    &hfdcan1,
                    &canFrames.tx_header_battery,
                    msg.data
                );
                break;

            default:
                uart_send("Unknown CAN message type\r\n");
                break;
        }
        tx_thread_sleep(100);
    }
}
