/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.h
  * @author  MCD Application Team
  * @brief   ThreadX applicative header file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_THREADX_H
#define __APP_THREADX_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "tx_api.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <main.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <utils.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

// Thread with max priority
#define MAX_PRIO   0

// Thread with medium priority
#define MEDIUM_PRIO   5

// Thread with low priority
#define LOW_PRIO   10

// Thread with no priority (lowest)
#define NONE_PRIO   15

//Queue size (number of messages)
#define QUEUE_SIZE      8

/*
Number of threads
1 -> Speed sensor thread
2 -> CAN TX thread
3 -> CAN RX thread
4 -> dc_motors thread
5 -> servo thread
6 -> battery thread
7 -> emergency break thread
*/
#define THREAD_COUNT    7

// Thread structure
typedef struct s_threads {
  TX_THREAD thread;
  UINT      stack[1024];
} t_threads;

// CAN frames structure
typedef struct s_canFrames {
  FDCAN_TxHeaderTypeDef tx_header_speed;
  FDCAN_TxHeaderTypeDef tx_header_heart_beat;
  FDCAN_TxHeaderTypeDef tx_header_battery;
} t_canFrames;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern FDCAN_HandleTypeDef  hfdcan1;
extern UART_HandleTypeDef   huart1;
extern TIM_HandleTypeDef    htim1;
extern I2C_HandleTypeDef    hi2c3;

extern TX_QUEUE             can_tx_queue;
extern TX_QUEUE             can_rx_queue;
extern TX_QUEUE             i2c_dc_motors_queue;
extern TX_QUEUE             i2c_servo_queue;
extern TX_MUTEX             i2c_mutex;
extern t_threads            threads[THREAD_COUNT];
/* USER CODE END EC */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Main thread defines -------------------------------------------------------*/
/* USER CODE BEGIN MTD */

/* USER CODE END MTD */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
UINT App_ThreadX_Init(VOID *memory_ptr);
void MX_ThreadX_Init(void);

/* USER CODE BEGIN EFP */

//threads
VOID  thread_SensorSpeed(ULONG thread_input);
VOID  thread_tx_can(ULONG thread_input);
VOID  thread_rx_can(ULONG thread_input);
VOID  thread_dc_motors(ULONG thread_input);
VOID  thread_servo(ULONG thread_input);
VOID  thread_battery(ULONG thread_input);
VOID  thread_emergency_break(ULONG thread_input);

//init
void  initCanFrames(t_canFrames *canFrames);
UINT  init_threads(VOID);
UINT  init_queue(VOID);

//utils
VOID                uart_send(const char *msg);
VOID                uart_send_int(int32_t value);
VOID                rpm_debug_print(ULONG rpm, 
                        ULONG cr1_reg, ULONG cnt_reg);
HAL_StatusTypeDef   i2c_scan_bus(VOID);

/* USER CODE END EFP */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /* __APP_THREADX_H */
