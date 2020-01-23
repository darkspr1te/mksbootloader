#include "lcd_init_gpio.h"

/**
 * #define LCD_nWR_Pin             GPIO_PIN_14
 #define LCD_nWR_GPIO_Port       GPIOB
 #define LCD_RS_Pin              GPIO_PIN_13
 #define LCD_RS_GPIO_Port        GPIOD
 #define LCD_BACKLIGHT_Pin       GPIO_PIN_14
 #define LCD_BACKLIGHT_GPIO_Port GPIOD
 #define LCD_nRD_Pin             GPIO_PIN_15
 #define LCD_nRD_GPIO_Port       GPIOD
 #define LCD_nCS_Pin             GPIO_PIN_8
 #define LCD_nCS_GPIO_Port       GPIOC
 */

void LCD_Init_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, 0xffff, GPIO_PIN_RESET);

  /*Configure GPIO pins : Pin1_Pin Pin2_Pin */
  GPIO_InitStruct.Pin = 0xffff;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); 
	GPIO_InitStruct.Pin = TOUCH_DI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(TOUCH_DI_GPIO_Port, &GPIO_InitStruct);

#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(GPIOB, LCD_nWR_Pin|FLASH_nCS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, SDCARD_nCS_Pin|LCD_RS_Pin|LCD_BACKLIGHT_Pin|LCD_nRD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, LCD_nCS_Pin|TOUCH_nCS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : FILAMENT_DI_Pin POWER_DI_Pin */
	GPIO_InitStruct.Pin = FILAMENT_DI_Pin|POWER_DI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : LCD_nWR_Pin FLASH_nCS_Pin */
	GPIO_InitStruct.Pin = LCD_nWR_Pin|FLASH_nCS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : SDCARD_nCS_Pin LCD_RS_Pin LCD_BACKLIGHT_Pin LCD_nRD_Pin */
	GPIO_InitStruct.Pin = SDCARD_nCS_Pin|LCD_RS_Pin|LCD_BACKLIGHT_Pin|LCD_nRD_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : LCD_nCS_Pin */
	GPIO_InitStruct.Pin = LCD_nCS_Pin|TOUCH_nCS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : WIFI_DI_Pin */
	GPIO_InitStruct.Pin = WIFI_DI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(WIFI_DI_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : SDCARD_DETECT_Pin */
	GPIO_InitStruct.Pin = SDCARD_DETECT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init */
	HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);	// sdcard detect irq
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);	// touch irq
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

#endif

}
void LCD_INIT_Panel(void)
{

}
void LCD_DATA(void)
{

}
void LCD_Writ_Bus(char VH,char VL, uint8_t mode)
{
    if (VH==0) 
	{
		GPIOE->ODR = (uint16_t)VL;
		HAL_GPIO_WritePin(LCD_nWR_GPIO_Port,LCD_nWR_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LCD_nWR_GPIO_Port,LCD_nWR_Pin, GPIO_PIN_SET);
	}
	else
	{

		GPIOE->ODR=(uint16_t)((VH<<8)+VL);
		HAL_GPIO_WritePin(LCD_nWR_GPIO_Port,LCD_nWR_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LCD_nWR_GPIO_Port,LCD_nWR_Pin, GPIO_PIN_SET);
	}
}
void LCD_Write_Bus_8(char com1)
{   
    GPIOE->ODR = (uint16_t)com1;
	HAL_GPIO_WritePin(LCD_nWR_GPIO_Port,LCD_nWR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_nWR_GPIO_Port,LCD_nWR_Pin, GPIO_PIN_SET);
 
}
void LCD_COM(void)
{}