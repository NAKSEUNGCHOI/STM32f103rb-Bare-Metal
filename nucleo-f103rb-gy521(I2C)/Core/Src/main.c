/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WHO_AM_I_REG			0x75
#define MPU_ADDR					0x68 << 1 // <---- 0xD0 8-bit address(Device Address) for I2C.
#define PWR_MAGT_1_REG		0x6B              // 	   Need to leave the last bit-field empty for "R/W bit: Write"
#define CONFIG_REG				0x1A          // 	   It is a common method.
#define GYRO_CONFIG_REG		0x1B
#define ACCEL_CONFIG_REG	0x1C
#define SMPLRT_DIV_REG			0x19
#define INT_STATUS_REG		0x3A
#define ACCEL_XOUT_H_REG			0x3B
#define TEMP_OUT_H_REG		0x41
#define GYRO_XOUT_H_REG				0x43
#define FIFO_EN_REG 			0x23
#define INT_ENABLE_REG 		0x38
#define I2CMACO_REG 			0x23
#define USER_CNT_REG			0x6A
#define FIFO_COUNTH_REG 	0x72
#define FIFO_R_W_REG 			0x74

int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;

int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;

float Ax, Ay, Az, Gx, Gy, Gz;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void MPU6050_Init(void){
	uint8_t check, Data;

	// Checking device ID WHO_AM_I
	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, WHO_AM_I_REG, 1, &check, 1, HAL_MAX_DELAY);

	if(check == 104){
		Data = 0;
		HAL_I2C_Mem_Write(&hi2c1, MPU_ADDR, PWR_MAGT_1_REG, 1, &Data, 1, HAL_MAX_DELAY);
		Data = 0x07; //SMPLRT_DIV_REG
		HAL_I2C_Mem_Write(&hi2c1, MPU_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, HAL_MAX_DELAY);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST = 0, YA_ST = 0, ZA_ST = 0, AFS_SEL = 0 -> ±2g
		Data = 0x00; //SMPLRT_DIV_REG
		HAL_I2C_Mem_Write(&hi2c1, MPU_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, HAL_MAX_DELAY);

		// Set Gyroscopic configuration in GYRO_CONFIG_REG
		// XG_ST = 0, YG_ST = 0, ZG_ST = 0, FS_SEL = 0 -> ±250 °/s
		Data = 0x0; //SMPLRT_DIV_REG
		HAL_I2C_Mem_Write(&hi2c1, MPU_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, HAL_MAX_DELAY);
	}
}

void MPU6050_READ_Accel(void){
	uint8_t Rec_data[6]; // read 6 BYTES of data starting from ACCEL_XOUT_H register.

	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, ACCEL_XOUT_H_REG, 1, Rec_data, 6, HAL_MAX_DELAY);

	Accel_X_RAW = (int16_t)(Rec_data[0] << 8 | Rec_data[1]);
	Accel_Y_RAW = (int16_t)(Rec_data[2] << 8 | Rec_data[3]);
	Accel_Z_RAW = (int16_t)(Rec_data[4] << 8 | Rec_data[5]);

	Ax = Accel_X_RAW/16384.0;
	Ay = Accel_Y_RAW/16384.0;
	Az = Accel_Z_RAW/16384.0;



}
int MPU6050_READ_Gyro(void){
	uint8_t Rec_data[6]; // read 6 BYTES of data starting from ACCEL_XOUT_H register.
						 // 6 BYTES, 2 BYTES for each axis.

	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, GYRO_XOUT_H_REG, 1, Rec_data, 6, HAL_MAX_DELAY);

	Gyro_X_RAW = (int16_t)(Rec_data[0] << 8 | Rec_data[1]);
	Gyro_Y_RAW = (int16_t)(Rec_data[2] << 8 | Rec_data[3]);
	Gyro_Z_RAW = (int16_t)(Rec_data[4] << 8 | Rec_data[5]);

	Gx = Gyro_X_RAW/131.0;
	Gy = Gyro_Y_RAW/131.0;
	Gz = Gyro_Z_RAW/131.0;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  float buf[4] = {'\0'};
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  MPU6050_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	MPU6050_READ_Accel();
	MPU6050_READ_Gyro();


	sprintf(buf, "Ax: %.2fg      Gx: %.2f\r\n", Ax, Gx);
	HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 100);
	sprintf(buf, "Ay: %.2fg      Gy: %.2f\r\n", Ay, Gy);
	HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 100);
	sprintf(buf, "Az: %.2fg      Gz: %.2f\r\n", Az, Gz);
	HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 100);
	HAL_Delay(200);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
