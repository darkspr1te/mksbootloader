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
  #define	LCD_CS_SET  GPIOC->BSRR=1<<8    //片选端口    PB9
  #define	LCD_RS_SET	GPIOD->BSRR=1<<13    //数据/命令 	PB8	   
  #define	LCD_WR_SET	GPIOB->BSRR=1<<14    //写数据			PB7
  #define	LCD_RD_SET	GPIOD->BSRR=1<<15    //读数据			PB6
                      
  #define	LCD_CS_CLR  GPIOC->BRR=1<<8     //片选端口  	PB9
  #define	LCD_RS_CLR	GPIOD->BRR=1<<13     //数据/命令		PB8	   
  #define	LCD_WR_CLR	GPIOB->BRR=1<<14     //写数据			PB7
  #define	LCD_RD_CLR	GPIOD->BRR=1<<15     //读数据			PB6   

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
