#include "stm32f1xx_hal.h"
#include "boot_conf.h"

extern unsigned int _isr_real;
const uint32_t *mcuFirstPageAddr = (const uint32_t *) (0x8000000 + MAIN_PR_OFFSET);

// uint32_t mcuLastPageAddr = ((uint32_t) &_isr_real) - FLASH_PAGE_SIZE;

uint8_t buffer[FLASH_PAGE_SIZE];
uint32_t bufferLen = 0;

static FIL fil;
static FILINFO info;

HAL_StatusTypeDef erase(uint32_t from, uint32_t to)
{
	HAL_StatusTypeDef res = HAL_OK;

	for (uint32_t i = from; i <= to; i += FLASH_PAGE_SIZE)
	{
		FLASH_EraseInitTypeDef erase;

		erase.TypeErase = FLASH_TYPEERASE_PAGES;
		erase.Banks = FLASH_BANK_1;
		erase.PageAddress = i;
		erase.NbPages = 1;

		uint32_t error = 0;
		res = HAL_FLASHEx_Erase(&erase, &error);
		#ifdef DEBUG
		//Heavy debug disabled by default 		
		//printf("erase page %#010x\n\r",i);
		#endif
		if (res != HAL_OK) {
			return res;
		}
	}
	return res;
}

FRESULT readNextPage(uint8_t *target, uint32_t *read)
{
	uint16_t blocksCount = 16;
	uint16_t fileBlock = FLASH_PAGE_SIZE / blocksCount;

	*read = 0;
	UINT readNow = 0;
	FRESULT res = FR_OK;

	for (uint16_t i = 0; i<blocksCount; i++)
	{
		res = f_read(&fil, target, fileBlock, &readNow);

		target += readNow;
		*read += readNow;

		if (res != FR_OK || readNow != fileBlock)
			break;
	}
	return res;
}

HAL_StatusTypeDef flashWrite(uint32_t position, uint8_t *data, uint32_t size)
{
	#ifdef DEBUG
	//Heavy debug disabled by default 	
	//printf("write page %#010x\n\r",position);
	#endif
	HAL_StatusTypeDef res = HAL_OK;
	for (uint32_t i=0; i<size; i+=2)
	{
		
		res = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, position + i, *(uint16_t*)&data[i]);
		if (res != HAL_OK)
			break;
	}
	return res;
}
// Colours 
void speccy_border(int progress)
{
	//totally for fun, reminder of the ole' days 
	int top_box = 10;
	int bottom_box = 10;
	int side_box = 10;
	int colour = WHITE;
	int offset = 0x6b;
	
	colour = (colour-(0xff*progress));
	GUI_SetColor(colour);
	GUI_FillRectColor(1, 1, side_box, 240,colour);
	colour=colour-(offset*3);
   	GUI_FillRectColor(1, 1, 320, top_box,colour);
	colour=colour-(offset*2);
	GUI_FillRectColor(320-side_box, 1, 320, 240,colour);
	colour=colour-(offset*4);
	GUI_FillRectColor(1, 240-bottom_box, 320, 240,colour);
	
}

void drawProgressbar(int x,int y, int width,int height, int progress)
{
	int gap =5;
	uint16_t colour_a,colour_b;
	colour_a = WHITE;
	colour_b = 0xfff;
   progress = progress > 100 ? 100 : progress; // set the progress value to 100
   progress = progress < 0 ? 0 :progress; // start the counting to 0-100
   GUI_SetRange(0,0,320-1,240-1);
   colour_a = (colour_a-(colour_b*progress));
   float bar = ((float)(width-1) / 100) * progress;
   GUI_SetColor(RED);
   GUI_DrawRect(x, y, width , (y+height));
	if (bar<width-gap)
   		GUI_FillRectColor(x+gap,y+gap,bar,(y+height)-gap,colour_a);
	else
		GUI_FillRectColor(x+gap,y+gap,bar-gap,(y+height)-gap,colour_a);
	

  #ifdef DEBUG
 //Heavy debug disabled by default 
 // printf("progress bar after calc %d\n\r",progress);
  #endif
 
  
}

FlashResult flash(const char *fname)
{
	FRESULT res = f_stat(fname, &info);
	if (res != FR_OK)
		return FLASH_FILE_NOT_EXISTS;

	// Checking file size
//	if (info.fsize > getMaxFlashImageSize())
//		return FLASH_FILE_TOO_BIG;

	res = f_open(&fil, fname, FA_OPEN_EXISTING | FA_READ);
if (res == FR_OK)
  {
#ifdef DEBUG		  
	  printf("flash open ok\n\r");
#endif

  }
	if (res != FR_OK)
	{
#ifdef DEBUG	
	  printf("flash open file failed\n\r");
#endif
		return FLASH_RESULT_FILE_ERROR;
	}
	uint32_t position = (uint32_t) mcuFirstPageAddr;

	if (HAL_OK != HAL_FLASH_Unlock())
		return FLASH_RESULT_FLASH_ERROR;
#ifdef DEBUG			
	printf("flash unlock\n\r");
#endif
	if (HAL_OK != erase((uint32_t) mcuFirstPageAddr, info.fsize + (uint32_t)mcuFirstPageAddr))
	{
#ifdef DEBUG	
	printf("erase error\n\r");
#endif
		return FLASH_RESULT_FLASH_ERROR;
	}
	uint32_t total=(uint32_t)mcuFirstPageAddr-0x8000000;
	int size = f_size(&fil);
	int prog=0;
	GUI_DispString(80, 45, (uint8_t*)"Flashing System");
	do {
		readNextPage(buffer, &bufferLen);
		if (HAL_OK != flashWrite(position, buffer, bufferLen))
			return FLASH_RESULT_FLASH_ERROR;
		position += bufferLen;
		#ifdef GFX_UI	
		//save some cyles if we dont need to calc this 
		prog=((position-(uint32_t)mcuFirstPageAddr)*100/info.fsize);
		#endif
		#ifdef DEBUG	
		printf("address %#010x ",position);
		printf("Percent  %d  \n\r",prog);
		#endif
		#ifdef GFX_UI
		
		drawProgressbar(20,100, 300,20,prog);
		speccy_border(prog);
		#endif
	} while (bufferLen != 0);

	f_close(&fil);
	HAL_FLASH_Lock();
#ifdef DEBUG
	  printf("flash write complete \r\n");
#endif
	return FR_OK;
}

