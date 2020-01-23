
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOOT_CONF_H
#define __BOOT_CONF_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"
#include "fatfs.h"
#include "lcd.h"
#include "boot.h"

#define MKS_TFT
#define aCount(n) (sizeof(n)/sizeof(n[0]))
#if defined(STM32F107xC) && defined(MKS_TFT)

/**
 * Makerbase MKS-TFT32
 */
#define SPEAKER_Pin             GPIO_PIN_2
 #define SPEAKER_GPIO_Port       GPIOA
 #define FILAMENT_DI_Pin         GPIO_PIN_0
 #define FILAMENT_DI_GPIO_Port   GPIOB
 #define POWER_DI_Pin            GPIO_PIN_1
 #define POWER_DI_GPIO_Port      GPIOB
 #define LCD_nWR_Pin             GPIO_PIN_14
 #define LCD_nWR_GPIO_Port       GPIOB
 #define LCD_RS_Pin              GPIO_PIN_13
 #define LCD_RS_GPIO_Port        GPIOD
 #define LCD_BACKLIGHT_Pin       GPIO_PIN_14
 #define LCD_BACKLIGHT_GPIO_Port GPIOD
 #define LCD_nRD_Pin             GPIO_PIN_15
 #define LCD_nRD_GPIO_Port       GPIOD
 #define LCD_nCS_Pin             GPIO_PIN_8
 #define LCD_nCS_GPIO_Port       GPIOC
 #define SDCARD_nCS_Pin          GPIO_PIN_11
 #define SDCARD_nCS_GPIO_Port    GPIOD
 #define SDCARD_DETECT_Pin       GPIO_PIN_15
 #define SDCARD_DETECT_GPIO_Port GPIOB
 #define TOUCH_DI_Pin            GPIO_PIN_5
 #define TOUCH_DI_GPIO_Port      GPIOC
 #define TOUCH_nCS_Pin           GPIO_PIN_9
 #define TOUCH_nCS_GPIO_Port     GPIOC
 #define WIFI_DI_Pin             GPIO_PIN_9
 #define WIFI_DI_GPIO_Port       GPIOA
 #define FLASH_nCS_Pin           GPIO_PIN_9
 #define FLASH_nCS_GPIO_Port     GPIOB
#define FIRMWARE_FILENAME       "mks.bin"
//Vector address to load firmware and boot 
#define MAIN_PR_OFFSET 0x7000
//printed via UART1 during debug builds
#define HARDWARE                "MKS TFT 2.8 V1.4 Board"  
#define DISPLAY_Y 				240
#define DISPLAY_X				320
#define ROTATION				1
#define LCD_WIDTH	  320
#define LCD_HEIGHT	240

#define BYTE_HEIGHT 16
#define BYTE_WIDTH  (BYTE_HEIGHT/2)
  
#define ICON_WIDTH  70
#define ICON_HEIGHT 70
#define TITLE_END_Y 40
  
#define LIST_ICON_WIDTH   55
#define LIST_ICON_HEIGHT  50
  
#define selecticonw 70
#define ROOT_DIR			"MKS"
#define SPI1_CS_PIN PB9
#include "spiflash_w25q16dv.h"

#endif

/**
 *  if you dont want to rename firmware update file after flashing 
 *  == Caution ==
 *  Bootloader will write firmware to flash each boot if file not rename after update
 *  define only for testing purpose, remove define in release builds 
 */
#define DONT_RENAME



//Common Defines 
#define LOCATION                "0:/"
#define FIRMWARE      LOCATION FIRMWARE_FILENAME
#define OLD                     "old"
#define RENAME_FILE        LOCATION OLD FIRMWARE_FILENAME
extern FATFS sdFileSystem;		// 0:/


#if !defined(MAIN_PR_OFFSET)
#define MAIN_PR_OFFSET 0x8000
#endif
#if !defined(FIRMWARE)
#define FIRMWARE                "0:/firmware.bin"
#endif 


typedef enum
{
	FLASH_RESULT_OK = 0,
	FLASH_RESULT_FILE_ERROR,
	FLASH_RESULT_FLASH_ERROR,
	FLASH_FILE_NOT_EXISTS,
	FLASH_FILE_CANNOT_OPEN,
	FLASH_FILE_INVALID_HASH,
	FLASH_FILE_TOO_BIG
} FlashResult;

FlashResult flash(const char *fname);
extern const uint32_t *mcuFirstPageAddr;

typedef void (*Callable)();

#define u8 uint8_t
#define u16 uint16_t

#endif /* __BOOT_CONF_H */

