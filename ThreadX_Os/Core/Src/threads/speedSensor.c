#include "app_threadx.h"
#include "speed_rpm.h"

VOID    rpm_debug_print(ULONG rpm, ULONG cr1_reg, ULONG cnt_reg) {

    char debug[32];

    int len = snprintf(debug, sizeof(debug),
            "RPM=%lu | CR1=%lu | CNT=%lu\r\n",
            rpm, cr1_reg, cnt_reg );

    if (len > 0 && (size_t)len < sizeof(debug))
        HAL_UART_Transmit(&huart1, (uint8_t *)debug, len, 100);
}

// Thread responsible for reading speed sensor and sending RPM via CAN
VOID thread_SensorSpeed(ULONG thread_input)
{
    uint16_t        rpm;
    t_tx_can_msg    msg;
    UINT            ret;
    ULONG           period;
    t_rpm_state     state = {0, 0, 1};

    memset(&msg, 0, sizeof(t_tx_can_msg));
    msg.type = CAN_MSG_SPEED;

    // Reset/start timer, responsible to control STM32 timer
    HAL_TIM_Base_Stop(&htim1);
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    HAL_TIM_Base_Start(&htim1);
     period = htim1.Init.Period;

    while (1)
    {
        // Hardware timer values
        ULONG count = htim1.Instance->CNT;
        ULONG cr1_reg = htim1.Instance->CR1;
        ULONG ticks = tx_time_get();
        rpm = convertValuesRPM(count, ticks, period, TX_TIMER_TICKS_PER_SECOND, &state);

        // Debug
        rpm_debug_print(rpm, cr1_reg, count);

        // Division of RPM into two data bytes *(big-endian)*
        msg.data[0] = (rpm >> 8) & 0xFF;
        msg.data[1] = rpm & 0xFF;

        // Sends data to CAN thread
        ret = tx_queue_send(&can_tx_queue, &msg, TX_NO_WAIT);
        if (ret != TX_SUCCESS)
        {
            uart_send("CAN TX could not add message to queue!\r\n");
            tx_thread_sleep(500);
            continue ;
        }

        tx_thread_sleep(500);
    }
}

/*
Visual representation on how RPM is divided into two data bytes for CAN transmission:

rpm = 500 (decimal) = 0x01F4 (hex) = 0000 0001 1111 0100 (binary)
                                     ├── high byte ──┤├── low byte ──┤
                                         0x01              0xF4

rpm         = 0000 0001 1111 0100  (0x01F4 = 500)
rpm >> 8    = 0000 0000 0000 0001  (shift 8 bits to the right)
& 0xFF      = 0000 0000 0000 0001  (mask to ensure only 8 bits)
ret         = 0x01

rpm         = 0000 0001 1111 0100  (0x01F4 = 500)
& 0xFF      = 0000 0000 1111 0100  (mask the lower 8 bits)
ret         = 0xF4

msg.data[0] = 0x01  ← high byte
msg.data[1] = 0xF4  ← low byte
*/