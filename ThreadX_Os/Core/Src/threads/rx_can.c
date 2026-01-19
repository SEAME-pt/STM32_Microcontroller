#include "app_threadx.h"

// CALLBACK - automatically called by the interrupt when a CAN message arrives
void    rx_receive(t_rx_can_msg msg)
{
    FDCAN_RxHeaderTypeDef   rxHeader;
    t_rx_can_msg            msg;

    memset(&msg, 0, sizeof(t_rx_can_msg));
    uart_send("CAN RX Callback triggered\r\n");
    if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) > 0)
    {
        if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rxHeader, msg.data) == HAL_OK)
        {
            msg.type = rxHeader.Identifier;
            tx_queue_send(&can_rx_queue, &msg, TX_NO_WAIT);
        }
    }
}

// THREAD - responsible to receive CAN messages from the RX queue and process them
VOID    thread_rx_can(ULONG thread_input)
{
    t_rx_can_msg    msg;
    uart_send("CAN RX thread started\r\n");
    while (1)
    {
        // constant wait for incoming messages
        if (tx_queue_receive(&can_rx_queue, &msg, TX_WAIT_FOREVER) == TX_SUCCESS)
        {
            if (rx_handler(&msg) != TX_SUCCESS)
                uart_send("Error processing CAN RX message\r\n");
            else
                uart_send("CAN RX message processed successfully\r\n");
        }
    }
}
