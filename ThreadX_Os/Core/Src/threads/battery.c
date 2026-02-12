#include "i2c_ina219.h"
#include "can_protocol.h"

VOID thread_battery(ULONG thread_input)
{
    float           vbat;
    t_tx_can_msg    msg;

    memset(&msg, 0, sizeof(t_tx_can_msg));
    if (ina219_init(INA219_ADDR) != HAL_OK) 
    {
        uart_send("Battery Thread: INA219 init failed\r\n");
        return ;
    } 
    else 
        uart_send("Battery Thread started\r\n");
    msg.type = CAN_MSG_BATTERY;

    while (1) 
    {
        if (ina219_read_voltage(&vbat) == HAL_OK) 
        {
            //battery_debug_print(vbat);
            msg.data[0] = (uint8_t)vbat;
            msg.data[1] = (uint8_t)((vbat - msg.data[0]) * 100);
            msg.data[2] = (uint8_t)((vbat / 12.456f) * 100);
            if (tx_queue_send(&can_tx_queue, &msg, TX_NO_WAIT) != TX_SUCCESS)
                uart_send("Battery Thread: Failed to send CAN message\r\n");
        } 
        else
            uart_send("Battery Thread: INA219 read voltage failed\r\n");
        tx_thread_sleep(1500);
    }
}
