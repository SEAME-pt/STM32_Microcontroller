#include "i2c_ina219.h"
#include "can_protocol.h"

VOID thread_battery(ULONG thread_input)
{
    float vbat;

    uart_send("Battery Thread started\r\n");

    if (ina219_init(INA219_ADDR) != HAL_OK) {
        uart_send("Battery Thread: INA219 init failed\r\n");
        return;
    }

    while (1) {
        if (ina219_read_voltage(&vbat) == HAL_OK) {
            uart_send("Battery Voltage: ");
            uart_send_int((int32_t)(vbat * 1000)); // in mV
            uart_send(" mV\r\n");
        } else {
            uart_send("Battery Thread: INA219 read voltage failed\r\n");
        }

        tx_thread_sleep(100); // Sleep for 100 ticks
    }
}
