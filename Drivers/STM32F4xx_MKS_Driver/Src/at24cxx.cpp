
#include "at24cxx.h"


#define heval_I2c					hi2c1								/*	I2C_HandleTypeDef structure		*/

#define I2C_TIMEOUT  100 /*<! Value of Timeout when I2C communication fails */

#define I2Cx_Init		MX_I2C1_Init			/*	Initializes I2C HAL.	*/
#define HAL_I2C_DeInit		HAL_I2C_MspDeInit		/*	De-initialize		*/

/*
static void     I2Cx_Write(uint8_t Addr, uint8_t Reg, uint16_t MemAddSize, uint8_t Value);
static uint8_t  I2Cx_Read(uint8_t Addr, uint8_t Reg, uint16_t MemAddSize);
static HAL_StatusTypeDef I2Cx_WriteMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length);
static HAL_StatusTypeDef I2Cx_ReadMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length);
static HAL_StatusTypeDef  I2Cx_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
static void     I2Cx_Error(uint8_t Addr);
*/
/**
  * @brief  Reads a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  MemAddSize Size of internal memory address
  * @retval Data to be read
  */
uint8_t HAL::I2Cx_Read(uint8_t Addr, uint8_t Reg, uint16_t MemAddSize)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t Value = 0;
  
  status = HAL_I2C_Mem_Read(&heval_I2c, Addr, Reg, MemAddSize, &Value, 1, I2C_TIMEOUT);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2Cx_Error(Addr);
  }
  
  return Value;   
}

/**
  * @brief  Writes a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  MemAddSize Size of internal memory address
  * @param  Value: Data to be written
  */
void HAL::I2Cx_Write(uint8_t Addr, uint8_t Reg, uint16_t MemAddSize, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&heval_I2c, Addr, (uint16_t)Reg, MemAddSize, &Value, 1, I2C_TIMEOUT); 
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* I2C error occured */
    I2Cx_Error(Addr);
  }
}

/**
  * @brief  Reads multiple data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  MemAddSize Size of internal memory address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval Number of read data
  */
HAL_StatusTypeDef HAL::I2Cx_ReadMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Read(&heval_I2c, Addr, (uint16_t)Reg, MemAddSize, Buffer, Length, I2C_TIMEOUT);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* I2C error occured */
    I2Cx_Error(Addr);
  }
  return status;    
}

/**
  * @brief  Write a value in a register of the device through BUS in using DMA mode
  * @param  Addr: Device address on BUS Bus.  
  * @param  Reg: The target register address to write
  * @param  MemAddSize Size of internal memory address
  * @param  Buffer: The target register value to be written 
  * @param  Length: buffer size to be written
  * @retval HAL status
  */
HAL_StatusTypeDef HAL::I2Cx_WriteMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&heval_I2c, Addr, (uint16_t)Reg, MemAddSize, Buffer, Length, I2C_TIMEOUT);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the I2C Bus */
    I2Cx_Error(Addr);
  }
  return status;
}

/**
  * @brief  Checks if target device is ready for communication. 
  * @note   This function is used with Memory devices
  * @param  DevAddress: Target device address
  * @param  Trials: Number of trials
  * @retval HAL status
  */
HAL_StatusTypeDef HAL::I2Cx_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{ 
  return (HAL_I2C_IsDeviceReady(&heval_I2c, DevAddress, Trials, I2C_TIMEOUT));
}

/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  Addr: I2C Address
  */
void HAL::I2Cx_Error(uint8_t Addr)
{
	/* De-initialize the IOE comunication BUS */
  HAL_I2C_DeInit(&heval_I2c);
  
  /* Re-Initiaize the IOE comunication BUS */
  I2Cx_Init(100000);  
}

/******************************** LINK I2C AT24CXX *****************************/

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  */
void HAL::AT24CXX_Init(void)
{
  //I2Cx_Init();        //skyblue
}

/**
  * @brief  Camera writes single data.
  * @param  Reg: Reg address 
  * @param  Value: Data to be written
  */
void HAL::AT24CXX_WriteByte(uint8_t Reg, uint8_t Value)
{
  I2Cx_Write(AT24CXX_ADDRESS, Reg, AT24CXX_MEMADD_SIZE, Value);
	
    HAL_Delay(2);       //������ʱ
}

/**
  * @brief  Camera reads single data.
  * @param  Reg: Reg address 
  * @retval Read data
  */
uint8_t HAL::AT24CXX_ReadByte(uint8_t Reg)
{
  return I2Cx_Read(AT24CXX_ADDRESS, Reg, AT24CXX_MEMADD_SIZE);
}

/** ����  ����EEPROM��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���
	*         ���ܳ���EEPROMҳ�Ĵ�С��AT24C04ÿҳ��16���ֽڡ�
	* ����  ��-pBuffer ������ָ��
	*         -MemAddress �������ݵ�EEPROM�ĵ�ַ 
	*         -BufferSize Ҫд��EEPROM���ֽ���
	*/
HAL_StatusTypeDef HAL::AT24CXX_PageWrite(uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
	volatile uint32_t ii=0;
	HAL_StatusTypeDef status = HAL_OK;
	
	status = I2Cx_WriteMultiple((AT24CXX_ADDRESS+((MemAddress/256)<<1)),MemAddress,AT24CXX_MEMADD_SIZE, pBuffer,BufferSize); //skyblue
	
	//HAL_Delay(3);   //������ʱ
	/*while(ii--!=0){
		
	}*/
	for(ii=0;ii<90000;ii++);
	//ii=3000;
	return status;
}

/**
  * @brief Write data to I2C EEPROM driver in using DMA channel
  * @param MemAddress: memory address
  * @param pBuffer: Pointer to data buffer
  * @param BufferSize: Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL::AT24CXX_Write(uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	HAL_StatusTypeDef err = HAL_OK;

	Addr = MemAddress % AT24CXX_PAGE_SIZE;
	count = AT24CXX_PAGE_SIZE - Addr;
	NumOfPage =  BufferSize / AT24CXX_PAGE_SIZE;
	NumOfSingle = BufferSize % AT24CXX_PAGE_SIZE;
 
	/* If WriteAddr is I2C_PageSize aligned  */
	if(Addr == 0) 
	{
		/* If NumByteToWrite < I2C_PageSize */
		if(NumOfPage == 0) 
		{
			return (AT24CXX_PageWrite(MemAddress,pBuffer,BufferSize));
		}
		/* If NumByteToWrite > I2C_PageSize */
		else  
		{
			while(NumOfPage--)
			{
				err = AT24CXX_PageWrite(MemAddress,pBuffer,AT24CXX_PAGE_SIZE);
				if(err != HAL_OK)
					return err; 
				MemAddress +=  AT24CXX_PAGE_SIZE;
				pBuffer += AT24CXX_PAGE_SIZE;
			}

			if(NumOfSingle!=0)
			{
				return (AT24CXX_PageWrite(MemAddress,pBuffer,NumOfSingle)); 
			}
		}
	}
	  /* If WriteAddr is not I2C_PageSize aligned  */
	else 
	{
		/* If NumByteToWrite < I2C_PageSize */
		if(BufferSize <= count) 
		{
			return (AT24CXX_PageWrite(MemAddress,pBuffer,NumOfSingle));
		}
		/* If NumByteToWrite > I2C_PageSize */
		else
		{
			BufferSize -= count;
			NumOfPage =  BufferSize / AT24CXX_PAGE_SIZE;
			NumOfSingle = BufferSize % AT24CXX_PAGE_SIZE;	
		  
			err = AT24CXX_PageWrite(MemAddress,pBuffer,count);
			if(err != HAL_OK)
				return err; 
			MemAddress += count;
			pBuffer += count;
		  
			while(NumOfPage--)
			{
				err = AT24CXX_PageWrite(MemAddress,pBuffer,AT24CXX_PAGE_SIZE);
				if(err != HAL_OK)
					return err;
				MemAddress +=  AT24CXX_PAGE_SIZE;
				pBuffer += AT24CXX_PAGE_SIZE;  
			}
			
			if(NumOfSingle != 0)
			{
				return (AT24CXX_PageWrite(MemAddress,pBuffer,NumOfSingle)); 
			}
		}
	}
/*			��ʽ 2 ��Ч�ʵ�							*/	
//	while(BufferSize--)
//	{
//		AT24CXX_WriteByte(MemAddress,*pBuffer);
//		MemAddress++;
//		pBuffer++;
//	}
	return err;
}

/**
  * @brief  Reads data from I2C EEPROM driver in using DMA channel.
  * @param  MemAddress: memory address
  * @param  pBuffer: Pointer to data buffer
  * @param  BufferSize: Amount of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef HAL::AT24CXX_Read(uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
  return (I2Cx_ReadMultiple((AT24CXX_ADDRESS+((MemAddress/256)<<1)), MemAddress, AT24CXX_MEMADD_SIZE, pBuffer, BufferSize));    
}

/**
  * @brief  Checks if target device is ready for communication. 
  * @note   This function is used with Memory devices
  * @param  Trials: Number of trials
  * @retval HAL status
  */
HAL_StatusTypeDef HAL::AT24CXX_IsDeviceReady(uint32_t Trials)
{ 
  return (I2Cx_IsDeviceReady(AT24CXX_ADDRESS, Trials));
}

//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(2047)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
uint8_t HAL::AT24CXX_Check(void)
{
	uint8_t temp;
	temp=AT24CXX_ReadByte(AT24CXX_TYPE);//����ÿ�ο�����дAT24CXX			   
	if(temp==0XAA)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
            AT24CXX_WriteByte(AT24CXX_TYPE,0XAA);
	    temp=AT24CXX_ReadByte(AT24CXX_TYPE);	  
            if(temp==0XAA)return 0;
	}
	return 1;											  
}













