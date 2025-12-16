#include "app_threadx.h"

void    uart_send(const char *msg) 
{
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);
}