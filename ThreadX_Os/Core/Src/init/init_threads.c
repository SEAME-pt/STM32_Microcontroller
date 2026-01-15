#include "app_threadx.h"

// Function to initialize and create threads
UINT    init_threads(VOID) 
{
    UINT ret =  TX_SUCCESS;

    // Sensor speed thread
    ret = tx_thread_create(&threads[0].thread, "CANThread", thread_SensorSpeed, 0,
                                  threads[0].stack, 1024,
                                  THREAD_0_PRIO, THREAD_0_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS)
        uart_send("ERROR! Speed sensor thread creation failed!\r\n");

    // CAN TX thread
    ret = tx_thread_create(&threads[1].thread, "TxCanThread", thread_tx_can, 0,
                                  threads[1].stack, 1024,
                                  THREAD_0_PRIO, THREAD_0_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS)
        uart_send("ERROR! CAN TX thread creation failed!\r\n");

    // CAN RX thread
    ret = tx_thread_create(&threads[2].thread, "RxCanThread", thread_rx_can, 0,
                                  threads[2].stack, 1024,
                                  THREAD_0_PRIO, THREAD_0_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    uart_send("Creating RX CAN thread...\r\n");
    if (ret != TX_SUCCESS)
        uart_send("ERROR! CAN RX thread creation failed!\r\n");

    return (ret);
}
