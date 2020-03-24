#include "spi_ic.h"
#include "GPIO_Init.h"
#include "boot_conf.h"
#include "stm32f1xx_hal_spi.h" 
SPI_HandleTypeDef alt_spi;   
 

#ifndef SPI1_CS_PIN
  #define SPI1_CS_PIN  PA4
#endif

#ifndef SPI2_CS_PIN
  #define SPI2_CS_PIN  PB12
#endif

#ifndef SPI3_CS_PIN
  #define SPI3_CS_PIN  PA15
#endif

static SPI_TypeDef *spi[_SPI_CNT] = {
  SPI1,  //CS--PA4   SCK--PA5   MISO--PA6   MOSI--PA7
  SPI2,  //CS--PB12  SCK--PB13  MISO--PB14  MOSI--PB15
  SPI3}; //CS--PA15  SCK--PB3   MISO--PB4   MOSI--PB5

static const uint16_t spi_cs[_SPI_CNT] = {SPI1_CS_PIN, SPI2_CS_PIN, SPI3_CS_PIN};  //CS
static const uint16_t spi_sck[_SPI_CNT]  = {PA5,  PB13, PB3};  //SCK
static const uint16_t spi_miso[_SPI_CNT] = {PA6,  PB14, PB4};  //MISO
static const uint16_t spi_mosi[_SPI_CNT] = {PA7,  PB15, PB5};  //MOSI  
    
void SPI_GPIO_Init(uint8_t port)
{
  GPIO_InitSet(spi_sck[port], MGPIO_MODE_AF_PP, 0);  //SCK
  GPIO_InitSet(spi_miso[port], MGPIO_MODE_AF_PP, 0);  //MISO
  GPIO_InitSet(spi_mosi[port], MGPIO_MODE_AF_PP, 0);  //MOSI
  GPIO_InitSet(spi_cs[port], MGPIO_MODE_OUT_PP, 0);  //CS
  GPIO_SetLevel(spi_cs[port], 1);
}    

void SPI_GPIO_DeInit(uint8_t port)
{
  //set all of spi pins to input
  GPIO_InitSet(spi_sck[port], MGPIO_MODE_IPN, 0);  //SCK
  GPIO_InitSet(spi_miso[port], MGPIO_MODE_IPN, 0);  //MISO
  GPIO_InitSet(spi_mosi[port], MGPIO_MODE_IPN, 0);  //MOSI 
  GPIO_InitSet(spi_cs[port], MGPIO_MODE_IPN, 0);  //CS
}

//new function 

void alt_SPI_Init(void)
{
  

  alt_spi.Instance = SPI1;
  alt_spi.Init.Mode = SPI_MODE_MASTER;
  alt_spi.Init.Direction = SPI_DIRECTION_2LINES;
  alt_spi.Init.DataSize = SPI_DATASIZE_8BIT;
  alt_spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
  alt_spi.Init.CLKPhase = SPI_PHASE_2EDGE;
  alt_spi.Init.NSS = SPI_NSS_SOFT;
  alt_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  alt_spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
  alt_spi.Init.TIMode = SPI_TIMODE_DISABLE;
  alt_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  alt_spi.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&alt_spi) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_alt_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 
  }
}

void SPI_Protocol_Init(uint8_t port, uint8_t baudrate)
{
  SPI_InitTypeDef SPI_InitStructure;
 

return ;
switch(port)
{
  case _SPI1 : __HAL_RCC_SPI1_CLK_ENABLE();break;
  case _SPI2 : __HAL_RCC_SPI1_CLK_ENABLE();break;
  case _SPI3 : __HAL_RCC_SPI1_CLK_ENABLE();break;
  default:break;
}
  //SPI_InitStructure.Instance = SPI1;
  SPI_InitStructure.BaudRatePrescaler = baudrate<<3;
  //SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.CLKPolarity = SPI_POLARITY_LOW;
  SPI_InitStructure.CLKPhase = SPI_PHASE_2EDGE;
  //SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.CRCPolynomial = 7;
  SPI_InitStructure.DataSize = SPI_DATASIZE_8BIT;
  SPI_InitStructure.Direction = SPI_DIRECTION_2LINES;
  SPI_InitStructure.FirstBit =SPI_FIRSTBIT_MSB;
  SPI_InitStructure.Mode = SPI_MODE_MASTER;
  SPI_InitStructure.NSS = SPI_NSS_SOFT;
  HAL_SPI_Init(&SPI_InitStructure);
 // SPI_Init(spi[port], &SPI_InitStructure);
  
 // SPI_Cmd(spi[port],ENABLE);
}

void SPI_Config(uint8_t port)
{
 // SPI_GPIO_Init(port);
 // SPI_Protocol_Init(port, 1);
}

void SPI_DeConfig(uint8_t port)
{
  SPI_GPIO_DeInit(port);
  SPI_I2S_DeInit(spi[port]);
}


uint16_t SPI_Read_Write(SPI_HandleTypeDef *spiHandle, uint16_t d)
{
  int Dummy = 0xFF;
	int Data = 0;
 	while ((HAL_SPI_GetState(&alt_spi) != HAL_SPI_STATE_READY));
  HAL_SPI_TransmitReceive(&alt_spi, &d, &Data, 1, 5000);
#ifdef DEBUG
  //Heavy debug disable by default 
  //printf("data  %#010x\n\r",Data);
#endif
  return Data;
}


void SPI_CS_Set(uint8_t port, uint8_t level)
{
  GPIO_SetLevel(spi_cs[port], level);
}

