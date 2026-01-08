#include "app_threadx.h"

// CALLBACK - automatically called by the interrupt when a CAN message arrives
void    HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    // Variáveis locais estáticas - só visíveis dentro desta função
    static FDCAN_RxHeaderTypeDef rxHeader;
    static t_rx_can_msg          msg;
    uart_send("CAN RX Callback triggered\r\n");
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0)
    {
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, msg.data) == HAL_OK)
        {
            msg.type = rxHeader.Identifier;
            tx_queue_send(&can_rx_queue, &msg, TX_NO_WAIT);
        }
    }
}

// THREAD - espera mensagens da queue e processa-as
VOID    thread_rx_can(ULONG thread_input)
{
    t_rx_can_msg    msg;
    while (1)
    {
        //constant wait for incoming messages
        if (tx_queue_receive(&can_rx_queue, &msg, TX_WAIT_FOREVER) == TX_SUCCESS)
        {
            uart_send("CAN RX loop\r\n");
            // Processa a mensagem recebida baseado no ID
            switch (msg.type)
            {
                case 0x100:  // emergency break
                    uart_send("Emergency break received!\r\n");
                    break;

                case 0x101: // steering throttle
                    uart_send("Steering and throttle message received!\r\n");
                    break;

                case 0x102: // mode
                    uart_send("Mode change message received!\r\n");
                    break;

                default:
                    uart_send("Unknown message type received!\r\n");
                    break;
            }
        }
    }
}
