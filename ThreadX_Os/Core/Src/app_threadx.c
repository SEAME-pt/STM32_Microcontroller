/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "can_protocol.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PV */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_QUEUE    can_tx_queue;
TX_QUEUE    can_emergency_break_queue;
TX_QUEUE    i2c_dc_motors_queue;
TX_QUEUE    i2c_servo_queue;
TX_MUTEX    i2c_mutex;
t_threads   threads[THREAD_COUNT];

/* USER CODE BEGIN PD */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  
  // Create TX queue
  ret = tx_queue_create(&can_tx_queue, "CAN TX Queue", 
                        sizeof(t_tx_can_msg) / sizeof(ULONG),
                        memory_ptr, QUEUE_SIZE * sizeof(t_tx_can_msg));
  if (ret != TX_SUCCESS)
    uart_send("ERROR! Failed TX queue creation.\r\n");

  // Create RX queue
  UCHAR *can_emergency_break_queue = (UCHAR *)memory_ptr + QUEUE_SIZE * sizeof(t_tx_can_msg);
  ret = tx_queue_create(&can_emergency_break_queue, "CAN RX Queue", 
                        sizeof(t_rx_can_msg) / sizeof(ULONG),
                        can_emergency_break_queue, QUEUE_SIZE * sizeof(t_rx_can_msg));
  if (ret != TX_SUCCESS)
    uart_send("ERROR! Failed RX queue creation.\r\n");

  // Create I2C DC Motors queue
  UCHAR *i2c_motors_queue_memory = can_emergency_break_queue + QUEUE_SIZE * sizeof(t_rx_can_msg);
  ret = tx_queue_create(&i2c_dc_motors_queue, "I2C DC Motors Queue", 
                        sizeof(t_rx_can_msg) / sizeof(ULONG),
                        i2c_motors_queue_memory, QUEUE_SIZE * sizeof(t_rx_can_msg));
  if (ret != TX_SUCCESS)
    uart_send("ERROR! Failed I2C DC Motors queue creation.\r\n");

  // Create I2C Servo queue
  UCHAR *i2c_servo_queue_memory = i2c_motors_queue_memory + QUEUE_SIZE * sizeof(t_rx_can_msg);
  ret = tx_queue_create(&i2c_servo_queue, "I2C Servo Queue", 
                        sizeof(t_rx_can_msg) / sizeof(ULONG),
                        i2c_servo_queue_memory, QUEUE_SIZE * sizeof(t_rx_can_msg));
  if (ret != TX_SUCCESS)
    uart_send("ERROR! Failed I2C Servo queue creation.\r\n");

  if (init_threads() != TX_SUCCESS)
    exit(EXIT_FAILURE);
  else
    uart_send("\r\n=== ThreadX Initialized ===\r\n");

  /* USER CODE END App_ThreadX_MEM_POOL */
  /* USER CODE BEGIN App_ThreadX_Init */
  
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN Before_Kernel_Start */
  /* USER CODE END Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN Kernel_Start_Error */

  /* USER CODE END Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
