#include "app_threadx.h"
#include "i2c_pca9685.h"

VOID thread_dc_motors(ULONG initial_input)
{
    uart_send("DC Motors Thread: Setting motor speeds\r\n");
    while(1)
    {
        pca9685_init(&hi2c3, PCA9685_ADDR_MOTOR);

        motor_set(&hi2c3, MOTOR_LEFT, 20);
        motor_set(&hi2c3, MOTOR_RIGHT, 50);

        tx_thread_sleep(1000);
    }
}
