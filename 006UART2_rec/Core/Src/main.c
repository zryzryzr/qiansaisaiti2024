/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


#include "oled.h"
#include "stdio.h"
#include "string.h"



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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */




uint64_t runtime;
char txbuff[10];
char rxbuff[20],flag=0;
char txlen=0,*txptr;
char rxlen=0,*rxptr;

char temp=43,humi=45,tutemp=34,hl=30;
char led1=1,pump1=1,hdift1=1,fan1=1,dpin1=1;


void putss(UART_HandleTypeDef huart, char ch[])
{
	txlen=strlen(ch);
	txptr=ch;
	HAL_UART_Transmit_DMA(&huart,(uint8_t *)txptr,txlen);
}

void getss(UART_HandleTypeDef huart, char ch[],unsigned char len)
{
	rxlen=len;
	rxptr=ch;
	HAL_UART_Receive_DMA(&huart,(uint8_t *)rxptr,rxlen);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		flag=0x55;
	}
	if(huart->Instance==USART2)
	{
		flag=0x66;
	}	
}










/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	unsigned char i = 0;


	
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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	char s[]="hello,world3.\n\r";

	putss(huart2,s);
	putss(huart2,s);
	putss(huart2,s);
	OLED_Init();
	OLED_Clear();
	

	OLED_P6x8Str(5, 2,"temp:");
	OLED_P6x8Str(60,2,"humi:");
	OLED_P6x8Str(5, 3,"tutemp:");
	OLED_P6x8Str(60,3,"hl:");
	OLED_P6x8Str(5, 4,"led1:");
	OLED_P6x8Str(60,4,"pump1:");
	OLED_P6x8Str(5, 5,"hdift1:");
	OLED_P6x8Str(60,5,"fan1:");
	OLED_P6x8Str(5, 6,"dpin1:");


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		txbuff[0]=temp;
		txbuff[1]=humi;
		txbuff[2]=tutemp;
		txbuff[3]=hl;
		txbuff[4]=led1;
		txbuff[5]=pump1;
		txbuff[6]=hdift1;
		txbuff[7]=fan1;
		txbuff[8]=dpin1;
		//测试下改变数值是否会影响另一方
//		wd++;
//		if(wd==99) 
//			wd=0;
		//测试下改变数值是否会影响另一方
		putss(huart2,txbuff);
		getss(huart2,rxbuff,9);

		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(1000);
		runtime++;
		OLED_time(70, 0);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, GPIO_PIN_SET);	
		
		if(flag==0x66)
		{
			temp=rxbuff[0];
			humi=rxbuff[1];
			tutemp=rxbuff[2];
			hl=rxbuff[3];
			led1=rxbuff[4];
			pump1=rxbuff[5];
			hdift1=rxbuff[6];
			fan1=rxbuff[7];
			dpin1=rxbuff[8];
			
			putss(huart1,"data is received.\r");
			putss(huart1,rxbuff);
			putss(huart1,"\r");
			flag=0x00;
		}
		OLED_int(30, 2, temp);
		OLED_int(90, 2, humi);
		OLED_int(30, 3, tutemp);
		OLED_int(90, 3, hl);
		OLED_int(30, 4, led1);
		OLED_int(90, 4, pump1);		
		OLED_int(30, 5, hdift1);
		OLED_int(90, 5, fan1);		
		OLED_int(30, 6, dpin1);	
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
