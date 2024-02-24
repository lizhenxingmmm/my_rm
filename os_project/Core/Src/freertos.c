/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gimbal_task.h"
#include "voltage_task.h"
#include "spring_motor_task.h"
#include "NRF24L01.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId gimbalHandle;
osThreadId voltageHandle;
osThreadId RGBHandle;
osThreadId spring_motorHandle;
osThreadId messageHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartGimbalTask(void const * argument);
void StartVoltageTask(void const * argument);
void StartRGBTask(void const * argument);
void StartSpringMotorTask(void const * argument);
void StartMessageTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of gimbal */
  osThreadDef(gimbal, StartGimbalTask, osPriorityNormal, 0, 128);
  gimbalHandle = osThreadCreate(osThread(gimbal), NULL);

  /* definition and creation of voltage */
  osThreadDef(voltage, StartVoltageTask, osPriorityLow, 0, 128);
  voltageHandle = osThreadCreate(osThread(voltage), NULL);

  /* definition and creation of RGB */
  osThreadDef(RGB, StartRGBTask, osPriorityIdle, 0, 128);
  RGBHandle = osThreadCreate(osThread(RGB), NULL);

  /* definition and creation of spring_motor */
  osThreadDef(spring_motor, StartSpringMotorTask, osPriorityAboveNormal, 0, 128);
  spring_motorHandle = osThreadCreate(osThread(spring_motor), NULL);

  /* definition and creation of message */
  osThreadDef(message, StartMessageTask, osPriorityHigh, 0, 128);
  messageHandle = osThreadCreate(osThread(message), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartGimbalTask */
/**
  * @brief  Function implementing the gimbal thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartGimbalTask */
void StartGimbalTask(void const * argument)
{
  /* USER CODE BEGIN StartGimbalTask */
  /* Infinite loop */
  gimbal_task_init();
  for(;;)
  {
    gimbal_task();
    osDelay(1);
  }
  /* USER CODE END StartGimbalTask */
}

/* USER CODE BEGIN Header_StartVoltageTask */
/**
* @brief Function implementing the voltage thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartVoltageTask */
void StartVoltageTask(void const * argument)
{
  /* USER CODE BEGIN StartVoltageTask */
  /* Infinite loop */
  voltage_task_init();
  for(;;)
  {
    voltage_task();
    osDelay(1);
  }
  /* USER CODE END StartVoltageTask */
}

/* USER CODE BEGIN Header_StartRGBTask */
/**
* @brief Function implementing the RGB thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRGBTask */
void StartRGBTask(void const * argument)
{
  /* USER CODE BEGIN StartRGBTask */
  /* Infinite loop */
  HAL_GPIO_WritePin(GPIOH,GPIO_PIN_10,GPIO_PIN_SET);
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOH,GPIO_PIN_10);
    HAL_GPIO_TogglePin(GPIOH,GPIO_PIN_11);
    HAL_GPIO_TogglePin(GPIOH,GPIO_PIN_12);
    osDelay(100);
  }
  /* USER CODE END StartRGBTask */
}

/* USER CODE BEGIN Header_StartSpringMotorTask */
/**
* @brief Function implementing the spring_motor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSpringMotorTask */
void StartSpringMotorTask(void const * argument)
{
  /* USER CODE BEGIN StartSpringMotorTask */
  /* Infinite loop */
  spring_motor_task_init();
  for(;;)
  {
    spring_motor_task();
    osDelay(1);
  }
  /* USER CODE END StartSpringMotorTask */
}

/* USER CODE BEGIN Header_StartMessageTask */
/**
* @brief Function implementing the message thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMessageTask */
void StartMessageTask(void const * argument)
{
  /* USER CODE BEGIN StartMessageTask */
  /* Infinite loop */
  message_task_init();
  while(1)
		{
  message_task();
		osDelay(100);
  }
  /* USER CODE END StartMessageTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
