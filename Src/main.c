
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          :Boot loader for the following displays 
  *                      MKS TFT 28/32" STM32f107vc displays for Marlin + ramps board with AUX-1 connector
  *                       MKSTFT 24" STM32F105 based 
  ******************************************************************************
  * @attention
  *
 
  ******************************************************************************
  */

#include "main.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


#include "boot_conf.h"
#include <stdio.h>
#include <string.h>
#include "LCD_Init.h"


#if defined(STM32F107xC)

SPI_HandleTypeDef hspi1;	// SD-card
TIM_HandleTypeDef htim2;	// Buzzer


char SPISD_Path[4];     /* USER logical drive path */


#elif defined(STM32F103xE)
# include "bsp_driver_sd.h"

SD_HandleTypeDef hsd;
HAL_SD_CardInfoTypedef SDCardInfo;
char SD_Path[4];        /* SD logical drive path */

static void MX_SDIO_SD_Init(void);
static inline void ShortBeep() {}
#elif defined(STM32F407xx)
#endif

static void ShortBeep();
static void Jump_To_App(void);
 static uint8_t go_to(uint32_t myfunc);
static void ErrorBeep(int count);
FATFS sdFileSystem;		// 0:/


void osDelay(__IO uint32_t Delay)
{
    HAL_Delay(Delay);
}

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

inline void moveVectorTable(uint32_t Offset)
{
    // __disable_irq();
    SCB->VTOR = FLASH_BASE | Offset;
}


/**
  * @brief  The application entry point.
  * @retval int
  */
//#include "inline_font.h"

int main(void)
{
 
  HAL_Init();
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  MX_TIM2_Init();
  MX_FATFS_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* Initialize interrupts */
  MX_NVIC_Init();
  MX_FATFS_Init();
  
  /* Start System - start output*/
  
 
  #ifdef DEBUG
  printf("\n\r\n\r\n\rBooting\n\r");
  printf("Software version: %s\r\n",SOFTWARE_VERSION);
  printf("Board Build: \"%s\"\r\n",HARDWARE);
  printf("Build epoch %d\n\r",LAST_BUILD_TIME);
  #endif

  ErrorBeep(1);
  #ifdef DEBUG
  printf("Mounting Filesystem...\n\r");
  printf("looking for \"%s\" on sdcard\r\n",FIRMWARE);
  #endif
    

  unsigned char result;
  result = f_mount(&sdFileSystem, SPISD_Path, 1);
  #ifdef GFX_UI
  LCD_Init();
  LCD_RefreshDirection(1);
  W25Qxx_Init();
  GUI_Clear(BLACK);
  GUI_SetColor(GREEN);
  GUI_DispString(110, 30, (uint8_t*)"Booting");
  
  #endif

  if (result == FR_OK)
  {
    #ifdef DEBUG
	  printf("SD Card Open Success\r\n");
    #endif
  } else 
  {
    #ifdef DEBUG
	  printf("FatFs Init Failed Code: %d\r\n", (int)result);
    printf("is sd-card present ?");
    #endif
  }
//scanUpdates();
  //Now we look for sdcard file and if found we write it to flash
  if (FR_OK == f_mount(&sdFileSystem, SPISD_Path, 1) && FR_OK == flash(FIRMWARE))
  {
     #ifdef DEBUG
     #ifndef DONT_RENAME
      if (FR_OK !=f_rename(FIRMWARE,RENAME_FILE))
      {
        printf("rename error");
        printf("old File name %s\r\n",FIRMWARE);
        printf("new File name %s\r\n",RENAME_FILE);       
      }
      else
      {
        printf("old File name %s\r\n",FIRMWARE);
        printf("new File name %s\r\n",RENAME_FILE); 
      }
      #endif
      #endif
      //We flashed a new firmware app now lets jump to it's vectors at addres stored in MAIN_PR_OFFSET(boot_conf.h)
      //Beep 3 for we flashed ok 
      ErrorBeep(3);
      Jump_To_App();
  }

  
  while (1)
  {
    
    //Assume theres a app there to boot too
    Jump_To_App();
    //we can run a crc or check for valid reset vectors (guess work) to get to here too
    #ifdef DEBUG
    printf("app failed to boot at %#010x\n\r",MAIN_PR_OFFSET);
    printf("we should not be here!\n\r");
    printf("no app to boot too!\n\r");
    #endif
    ErrorBeep(5);
    HAL_Delay(5000);
    
  }
 

 //End of main, we should not get to this point 
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
 #if defined(STM32F107xC)

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV5;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the Systick interrupt time 
  */
  __HAL_RCC_PLLI2S_ENABLE();
}
#elif  defined(STM32F407xx)

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
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

#endif
/**
  * @brief NVIC Configuration.
  * @retval None
  */
 static uint8_t go_to(uint32_t myfunc)
{
	uint8_t ret = 0;
	void(*ptr)(void);
	if((*(volatile uint32_t *)myfunc & 0x2ffe0000) == 0x20000000)
	{

		__set_MSP((*(volatile uint32_t *)myfunc));
		ptr = (void(*)(void))(*(__IO uint32_t*)(myfunc+4));

		ptr();
	}
	return ret;      
}
/**
 * We shutdown system ready to load vector table for app and jump to start address
 * 
 */

 void Jump_To_App(void)
 {
      f_mount(NULL, SPISD_Path, 1);
      HAL_SPI_MspDeInit(&hspi1);
      HAL_TIM_Base_MspDeInit(&htim2);

      __HAL_RCC_GPIOA_CLK_DISABLE();
      __HAL_RCC_GPIOB_CLK_DISABLE();
      __HAL_RCC_GPIOC_CLK_DISABLE();
      __HAL_RCC_GPIOD_CLK_DISABLE();
      __HAL_RCC_GPIOE_CLK_DISABLE();

      HAL_DeInit();

      // Disabling SysTick interrupt
      SysTick->CTRL = 0;
      moveVectorTable(MAIN_PR_OFFSET);
      // Setting initial value to stack pointer
      __set_MSP(*mcuFirstPageAddr);
      // booting really

      Callable resetHandler = (Callable) (*(mcuFirstPageAddr + 1) );
      resetHandler();
 }
 
static void MX_NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* SPI1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SPI1_IRQn);
}

//old replaced by ErrorBeep(count)
void ShortBeep()
{
	HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_3);
	HAL_Delay(40);
	HAL_TIM_OC_Stop_IT(&htim2, TIM_CHANNEL_3);
}

/**
 * Error beeps, 1=OK, 3=Flashing complete,5=Boot error
 * 
 */

static void ErrorBeep(int count)
{
  //blocking, need to add non-blocking 
  for (int i=0;i<count;i++)
  {
	  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_3);
	  HAL_Delay(30);
	  HAL_TIM_OC_Stop_IT(&htim2, TIM_CHANNEL_3);
    HAL_Delay(150);
  }

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


