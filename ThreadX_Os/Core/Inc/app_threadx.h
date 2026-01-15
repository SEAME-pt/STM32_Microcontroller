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

// Thread structure
typedef struct s_threads {
  TX_THREAD thread;
  uint8_t   stack[1024];
} t_threads;

// CAN frames structure
typedef struct s_canFrames {
  FDCAN_TxHeaderTypeDef tx_header_speed;
  FDCAN_TxHeaderTypeDef tx_header_steering_throttle;
  FDCAN_TxHeaderTypeDef tx_header_battery;
} t_canFrames;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern FDCAN_HandleTypeDef  hfdcan1;
extern UART_HandleTypeDef   huart1;
extern TIM_HandleTypeDef    htim1;

extern TX_QUEUE             can_tx_queue;
extern TX_QUEUE             can_rx_queue;
extern t_threads            threads[3];
/* USER CODE END EC */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Main thread defines -------------------------------------------------------*/
/* USER CODE BEGIN MTD */

//Thread 0 (Speed Sensor) max priority
#define THREAD_0_PRIO   1

//Queue size (number of messages)
#define QUEUE_SIZE      8

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

//init
void  initCanFrames(t_canFrames *canFrames);
UINT  init_threads(VOID);
UINT  init_queue(VOID);

//utils
VOID  uart_send(const char *msg);
UINT  rx_handler(const t_rx_can_msg *msg);

/* USER CODE END EFP */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /* __APP_THREADX_H */
