/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
	#include "ringbuffer.h"
	#include <string.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

#define API_KEY "5ZRXVI7HIJQPGKID"

	extern DMA_HandleTypeDef hdma_usart2_rx;
	RingBuffer rx_buffer2;
	#define RX_BUFFER_SIZE 30
	uint8_t rx_circular_buffer2[RX_BUFFER_SIZE];

	extern DMA_HandleTypeDef hdma_usart3_rx;
	RingBuffer rx_buffer3;
	uint8_t rx_circular_buffer3[RX_BUFFER_SIZE];

	uint8_t cmd[500];
	uint32_t iCmd = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

uint8_t getCheckSum(uint8_t *);
uint32_t UART_Read(void);
void UART_3_Read(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

//	UART1 -> debug	PA_9(TX) 	PA10(RX)
//	UART2 -> CO2	PA_2(TX) 	PA3_(RX)
//	UART3 -> wi-fi	PB10(TX)	PB11(RX)

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

	char DataChar[100];
	uint8_t CO2_Packet_Read[9] = {0xff,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
	//uint8_t CO2_Packet_CZP[9] = {0xff,0x01,0x87,0x00,0x00,0x00,0x00,0x00,0x78};
	//uint8_t CO2_Packet_CSP[9] = {0xff,0x01,0x88,0x07,0xD0,0x00,0x00,0x00,0xA0};

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();

  /* USER CODE BEGIN 2 */
  RingBuffer_Init(&rx_buffer2, &hdma_usart2_rx, rx_circular_buffer2,RX_BUFFER_SIZE);
  HAL_UART_Receive_DMA(&huart2, rx_circular_buffer2, RX_BUFFER_SIZE);

  RingBuffer_Init(&rx_buffer3, &hdma_usart3_rx, rx_circular_buffer3,RX_BUFFER_SIZE);
  HAL_UART_Receive_DMA(&huart3, rx_circular_buffer3, RX_BUFFER_SIZE);

    sprintf(DataChar,"Start ESP8266\n\r");
  	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
  	HAL_Delay(10);

  	sprintf(DataChar, "AT+RST\r\n");
  	HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 100);
  	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
  	HAL_Delay(1000);
  	UART_3_Read();

  	sprintf(DataChar, "AT+CWMODE=1\r\n");
  	HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 100);
  	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
  	HAL_Delay(100);
  	UART_3_Read();

  	HAL_Delay(2000);
  	sprintf(DataChar, "AT+CWJAP=\"Kitchen\",\"Papanina36\"\r\n");
  	//sprintf(DataChar, "AT+CWJAP=\"Tapac\",\"27051329\"\r\n"); // Ok

  	HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 100);
  	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
  	HAL_Delay(1000);
  	UART_3_Read();

  	sprintf(DataChar,"ESP8266 - OK!\n\r");
  	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
 	HAL_Delay(100);

	sprintf(DataChar,"\nCO2 UART1 Start\n\r");
	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	HAL_UART_Transmit(&huart2, (uint8_t *)CO2_Packet_Read, 9, 100);
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
	HAL_Delay(10000);
	int co2 = UART_Read();
	sprintf(DataChar,"2CO2= %d\n\r",(int)co2);
	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);

	char http_req_1[200];
	sprintf(http_req_1,"GET /update?api_key=%s&field1=%d\r\n\r\n",	API_KEY,(int)co2);

	// Connect to server 1
	sprintf(DataChar, "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n");
	HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 1000);
	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
	HAL_Delay(2000);
	//HAL_Delay(1000);
	UART_3_Read();

	// Send data length (length of request)
	sprintf(DataChar, "AT+CIPSEND=%d\r\n", strlen(http_req_1));
	HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 1000);
	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
	HAL_Delay(500); // wait for ">"
	UART_3_Read();

	// SEND DATA
	HAL_UART_Transmit(&huart3, (uint8_t *)http_req_1, strlen(http_req_1), 1000);
	HAL_UART_Transmit(&huart1, (uint8_t *)http_req_1, strlen(http_req_1), 100);
	HAL_Delay(100);
	UART_3_Read();

	  HAL_Delay(20000);
	  HAL_Delay(20000);
	  HAL_Delay(20000);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
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
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

uint8_t getCheckSum(uint8_t *packet)
{
	uint8_t checksum = 0x00;
	for( uint8_t i = 1; i < 8; i++)
		{
		checksum += packet[i];
		}
	checksum = 0xff - checksum;
	checksum += 1;
	return checksum;
}
//=================================================================
void UART_3_Read(void)
 {
	char DataChar_UART[100];
   // if something go wrong with DMA - just restart receiving
 	//if (hdma_usart2_rx.State == HAL_DMA_STATE_ERROR) {
 		//HAL_UART_DMAStop(&huart2);
 		//HAL_UART_Receive_DMA(&huart2, rx_circular_buffer, RX_BUFFER_SIZE);
 	//}
   // how many bytes in buffer
 	uint32_t rx_count = RingBuffer_Count(&rx_buffer3);
 	while (rx_count--)
 		{
 		uint8_t c = RingBuffer_GetByte(&rx_buffer3);
 		if (c == '\n')
 			{
 			sprintf(DataChar_UART,"Answer: %s\n\r",cmd);
			HAL_UART_Transmit(&huart1, (uint8_t *)DataChar_UART, strlen(DataChar_UART), 100);
			HAL_Delay(10);
			cmd[iCmd++] = 0; // we received whole command, setting end of string
			iCmd = 0;
 			}
 		else if(c == '\r')
 			{
 			// skip '\r' symbol
 			}
 		else
 			{
 			cmd[iCmd++ % 500] = c;
 			}
 		}// while (rx_count--)
 }// void UART_Read(void)
//======================================================================

//=================================================================
uint32_t UART_Read(void)
 {
	uint32_t co2=0;
	char DataChar_UART[100];
   // if something go wrong with DMA - just restart receiving
// 	if (hdma_usart2_rx.State == HAL_DMA_STATE_ERROR) {
// 		HAL_UART_DMAStop(&huart2);
// 		HAL_UART_Receive_DMA(&huart2, rx_circular_buffer, RX_BUFFER_SIZE);
// 	}
   // how many bytes in buffer
 	uint32_t rx_count = RingBuffer_Count(&rx_buffer2);
 	while (rx_count--)
 		{
 		uint8_t c = RingBuffer_GetByte(&rx_buffer2);
 		cmd[iCmd++ % 500] = c;
 		HAL_Delay(10);
 		}// while (rx_count--)
	co2 = cmd[2] * 0xff +cmd[3];

	uint8_t CheckSum = getCheckSum(cmd);
	if (cmd[8] == CheckSum)
		sprintf(DataChar_UART,"CO2= %d\n\r",(int)co2);
	else
		sprintf(DataChar_UART,"CheckSum- ERROR\n");

	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar_UART, strlen(DataChar_UART), 100);

	cmd[iCmd++] = 0; // we received whole command, setting end of string
	iCmd = 0;
	return co2;
 }// void UART_Read(void)
//======================================================================
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
