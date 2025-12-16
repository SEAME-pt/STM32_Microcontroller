#include "app_threadx.h"

void thread_SensorSpeed(ULONG input) 
{
    ULONG counter = 0;
    char buffer[32];
    uint8_t speedData[8] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
  
    uart_send("\r\n=== ThreadX Started! ===\r\n");
    uart_send("CAN Thread running...\r\n");

    while(1) {

        int value = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15);
        // Send CAN message with Speed header
        if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &canFrames.TxHeader_Speed, speedData) == HAL_OK) {
            snprintf(buffer, sizeof(buffer), "CAN Msg sent: %lu\r\n", counter++);
            uart_send(buffer);
        } else {
            uart_send("CAN send failed!\r\n");
            HAL_FDCAN_Stop(&hfdcan1);
            HAL_FDCAN_Start(&hfdcan1);
            tx_thread_sleep(100);
            continue ;
        }

        if (value == 1) {
            uart_send("PIN HIGH\r\n");
        } else {
            uart_send("PIN LOW\r\n");
        }
        
        // Sleep for 1.5 seconds
        tx_thread_sleep(500);
    }
}