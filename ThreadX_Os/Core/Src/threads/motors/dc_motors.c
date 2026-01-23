#include "app_threadx.h"
#include "i2c_pca9685.h"

VOID thread_dc_motors(ULONG initial_input)
{
    uart_send("DC Motors Thread: Setting motor speeds\r\n");
    while(1)
    {
        pca9685_init(&hi2c3, PCA9685_ADDR_MOTOR);
        // Exemplo: motor esquerdo a 50%, direito a -30%
        motor_set(&hi2c3, MOTOR_LEFT, 50);
        motor_set(&hi2c3, MOTOR_RIGHT, -30);

        tx_thread_sleep(100);  // 100 * 10ms = 1s aprox
    }
}
