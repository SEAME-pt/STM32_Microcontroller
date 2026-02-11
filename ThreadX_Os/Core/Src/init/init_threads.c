#include "app_threadx.h"

// Function to initialize and create threads
UINT    init_threads(VOID)
{
    UINT ret = TX_SUCCESS;

    // Sensor speed thread
    ret = tx_thread_create(&threads[0].thread, "CANThread", thread_SensorSpeed, 0,
                                  threads[0].stack, 1024,
                                  LOW_PRIO, LOW_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS) {
        uart_send("ERROR! Speed sensor thread creation failed!\r\n");
        exit(EXIT_FAILURE);
    }
    /* else
        uart_send("Speed Sensor Thread created successfully.\r\n"); */

    // CAN TX thread
    ret = tx_thread_create(&threads[1].thread, "TxCanThread", thread_tx_can, 0,
                                  threads[1].stack, 1024,
                                  MEDIUM_PRIO, MEDIUM_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS) {
        uart_send("ERROR! CAN TX thread creation failed!\r\n");
        exit(EXIT_FAILURE);
    }
    /* else
        uart_send("CAN TX Thread created successfully.\r\n"); */

    // CAN RX thread
    ret = tx_thread_create(&threads[2].thread, "RxCanThread", thread_rx_can, 0,
                                  threads[2].stack, 1024,
                                  MAX_PRIO, MAX_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);

    if (ret != TX_SUCCESS) {
        uart_send("ERROR! CAN RX thread creation failed!\r\n");
        exit(EXIT_FAILURE);
    }
    /* else
        uart_send("CAN RX Thread created successfully.\r\n"); */

     // I2C Driving Command thread
    ret = tx_thread_create(&threads[3].thread, "I2CDrivingThread", thread_driving_command, 0,
                                  threads[3].stack, 1024,
                                  MEDIUM_PRIO, MEDIUM_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS) {
        uart_send("ERROR! I2C Driving Command thread creation failed!\r\n");
        exit(EXIT_FAILURE);
    }
    /* else
        uart_send("I2C Driving Command Thread created successfully.\r\n"); */

    // Emergency brake thread - MAX PRIORITY
    ret = tx_thread_create(&threads[4].thread, "thread_emergency_brake", thread_emergency_brake, 0,
                                  threads[4].stack, 1024,
                                  MAX_PRIO, MAX_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS) {
        uart_send("ERROR! Emergency brake thread creation failed!\r\n");
        exit(EXIT_FAILURE);
    }
    /* else
        uart_send("Emergency brake Thread created successfully.\r\n"); */

    // Battery thread
    ret = tx_thread_create(&threads[5].thread, "BatteryThread", thread_battery, 0,
                                  threads[5].stack, 1024,
                                  NONE_PRIO, NONE_PRIO,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    if (ret != TX_SUCCESS) {
         uart_send("ERROR! Battery thread creation failed!\r\n");
         exit(EXIT_FAILURE);
    }
    /* else
        uart_send("Battery Thread created successfully.\r\n"); */

    return (ret);
}
