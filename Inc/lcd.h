#ifndef _LCD_H_
#define _LCD_H_

#include "stdint.h"
//#include "variants.h"
#include "boot_conf.h"


  /* 配置控制线
  * PB6   :LCD-RD
  * PB7   :LCD-WR
  * PB9   :LCD-CS
  * PB8   :LCD-RS   LCD-RS发送的数据代表是命令还是显示数据
  */

/**
 * 
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
  #define	LCD_CS_SET  GPIOC->BSRR=1<<8   
  #define	LCD_RS_SET	GPIOD->BSRR=1<<13      
  #define	LCD_WR_SET	GPIOB->BSRR=1<<14   
  #define	LCD_RD_SET	GPIOD->BSRR=1<<15   

#if defined(STM32F107xC)
  #define	LCD_CS_CLR  GPIOC->BRR=1<<8    
  #define	LCD_RS_CLR	GPIOD->BRR=1<<13      
  #define	LCD_WR_CLR	GPIOB->BRR=1<<14   
  #define	LCD_RD_CLR	GPIOD->BRR=1<<15     

#elif  defined(STM32F407xx)
 #define	LCD_CS_CLR  GPIOD->BSRR=0<<7
  #define	LCD_RS_CLR	GPIOD->BSRR=0<<12      
  #define	LCD_WR_CLR	GPIOD->BSRR=0<<5   
  #define	LCD_RD_CLR	GPIOD->BSRR=1<<4    
  /*
  
typedef struct
{
  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      
  __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04     
  __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      
  __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      
  __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      
  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      
  __IO uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      
  __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      
  __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 
} GPIO_TypeDef;
*/
#endif 

  //PB0~15,作为数据线
  #define DATAOUT(x) do{GPIOE->ODR = x;}while(0) //数据输出
  #define DATAIN()     GPIOE->IDR                //数据输入	

  void LCD_WR_REG(uint16_t data);
  void LCD_WR_DATA(uint16_t data);
  



//
#define WHITE                 0xFFFF
#define BLACK                 0x0000
#define BLUE                  0x001F
#define GBLUE                 0X07FF
#define RED                   0xF800
#define MAGENTA               0xF81F
#define GREEN                 0x07E0
#define CYAN                  0x7FFF
#define YELLOW                0xFFE0
#define BROWN                 0XBC40
#define BRRED                 0XFC07
#define GRAY                  0X8430

void LCD_HardwareConfig(void);
uint16_t LCD_RD_DATA(void);

#endif
