#include "app_threadx.h"

// RPM = pulse / PPR * (60 / dt_seconds)
// RPM calculation from timer values
static UINT convertValuesRPM(void)
{
    static ULONG    last_time_ticks = 0;
    static ULONG    last_count      = 0;
    static UINT     first_run       = 1;

    // Read current timer and system tick
    ULONG current_count = htim1.Instance->CNT;
    ULONG current_time_ticks = tx_time_get();

    if (first_run)
    {
        last_count = current_count;
        last_time_ticks = current_time_ticks;
        first_run = 0;
        return (0);
    }

    // Calculate time in seconds
    ULONG delta_ticks = current_time_ticks - last_time_ticks;
    if (delta_ticks == 0)
        return (0);

    // Calculate pulses
    ULONG pulses = (current_count >= last_count) 
                    ? (current_count - last_count) 
                    : (htim1.Init.Period - last_count + current_count + 1);

    // Update vars for next call
    last_count = current_count;
    last_time_ticks = current_time_ticks;

    // Converts to RPM
    UINT rpm = (UINT)pulses * 60 * TX_TIMER_TICKS_PER_SECOND / (PPR * delta_ticks);

    if (rpm > htim1.Init.Period)
        rpm = htim1.Init.Period;

    return rpm;
}

// Thread responsible for reading speed sensor and sending RPM via CAN
VOID thread_SensorSpeed(ULONG thread_input)
{
    char            debug[32];
    uint16_t        rpm;
    t_tx_can_msg    msg;
    UINT            ret;

    memset(&msg, 0, sizeof(t_tx_can_msg));
    msg.type = CAN_MSG_SPEED;

    // Reset / start timer, responsible to control STM32 timer
    HAL_TIM_Base_Stop(&htim1);
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    HAL_TIM_Base_Start(&htim1);

    while (1)
    {
        rpm = convertValuesRPM();

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

        // Debug
        ULONG cr1_reg = htim1.Instance->CR1;
        ULONG cnt_reg = htim1.Instance->CNT;

        int len = snprintf(debug, sizeof(debug),
            "RPM=%u | CR1=%lu | CNT=%lu\r\n",
            rpm, cr1_reg, cnt_reg );

        if (len > 0 && (size_t)len < sizeof(debug))
            HAL_UART_Transmit(&huart1, (uint8_t *)debug, len, 100);

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