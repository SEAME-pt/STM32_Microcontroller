#include "app_threadx.h"

// Function to initialize and create threads
UINT    init_threads(VOID)
{
    UINT ret = TX_SUCCESS;

    // Sensor speed thread
    ret = tx_thread_create(&threads[0].thread, "CANThread", thread_SensorSpeed, 0,
                                  threads[0].stack, 1024,
                                  THREAD_LOW_PRIO, THREAD_LOW_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS)
        uart_send("ERROR! Speed sensor thread creation failed!\r\n");
    else
        uart_send("Speed Sensor Thread created successfully.\r\n");

    // CAN TX thread
    ret = tx_thread_create(&threads[1].thread, "TxCanThread", thread_tx_can, 0,
                                  threads[1].stack, 1024,
                                  THREAD_LOW_PRIO, THREAD_LOW_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS)
        uart_send("ERROR! CAN TX thread creation failed!\r\n");
    else
        uart_send("CAN TX Thread created successfully.\r\n");

    // CAN RX thread
    ret = tx_thread_create(&threads[2].thread, "RxCanThread", thread_rx_can, 0,
                                  threads[2].stack, 1024,
                                  THREAD_MAX_PRIO, THREAD_MAX_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);

    if (ret != TX_SUCCESS)
        uart_send("ERROR! CAN RX thread creation failed!\r\n");
    else
        uart_send("CAN RX Thread created successfully.\r\n");

     // DC Motors thread
    ret = tx_thread_create(&threads[3].thread, "DCMotorsThread", thread_dc_motors, 0,
                                  threads[3].stack, 1024,
                                  THREAD_MAX_PRIO, THREAD_MAX_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS)
        uart_send("ERROR! DC Motors thread creation failed!\r\n");
    else
        uart_send("DC Motors Thread created successfully.\r\n");

    // Servo thread
    ret = tx_thread_create(&threads[4].thread, "ServoThread", thread_servo, 0,
                                  threads[4].stack, 1024,
                                  THREAD_MEDIUM_PRIO, THREAD_MEDIUM_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS)
        uart_send("ERROR! Servo thread creation failed!\r\n");
    else
        uart_send("Servo Thread created successfully.\r\n");

/*     // Battery thread
    ret = tx_thread_create(&threads[5].thread, "BatteryThread", thread_battery, 0,
                                  threads[5].stack, 1024,
                                  THREAD_NONE_PRIO, THREAD_NONE_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS)
        uart_send("ERROR! Battery thread creation failed!\r\n");
    else
        uart_send("Battery Thread created successfully.\r\n"); */

    return (ret);
}
