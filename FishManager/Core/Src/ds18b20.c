/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ds18b20.c
 *
 * Project:
 * --------
 *   AES_Sensors
 *
 * Description:
 * ------------
 * This file is the driver of ds18b20 of AES sensors using STM32WB.
 * 
 * Note:
 *============================================================================
 *             HISTORY
 *  2020-01-10 V0.1 Modified By LinZi, Tianjin University
 *----------------------------------------------------------------------------
 * 
 ****************************************************************************/
#include "ds18b20.h"
void Soft_delay_us(uint16_t time)
{
	   uint32_t i;  
   while(time--)
   {
      i=8; 
      while(i--){};
        __nop();
        __nop();
        __nop();
        __nop();
   }
}
void DS18B20_Rst(void)	   
{                 
		DS18B20_IO_OUT(); 
    HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);
    Soft_delay_us(750);  
    HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);
		Soft_delay_us(15);
}
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_IO_IN();
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		Soft_delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		Soft_delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
uint8_t DS18B20_Read_Bit(void)
{
    uint8_t data;
	DS18B20_IO_OUT();
    HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);; 
	Soft_delay_us(2);
    HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);; 
	DS18B20_IO_IN();
	Soft_delay_us(12);
	if(DS18B20_DQ_IN)
		data=1;
  else 
		data=0;	 
    Soft_delay_us(50);           
    return data;
}
uint8_t DS18B20_Read_Byte(void) 
{        
    uint8_t i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
void DS18B20_Write_Byte(uint8_t dat)     
 {             
    uint8_t j;
    uint8_t testb;
	DS18B20_IO_OUT();
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);
            Soft_delay_us(2);                            
            HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);
            Soft_delay_us(60);             
        }
        else 
        {
            HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);
            Soft_delay_us(60);             
            HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);
            Soft_delay_us(2);                          
        }
    }
}
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0x44);
} 
 	 
uint8_t DS18B20_Init(void)
{
	DS18B20_Rst();

	return DS18B20_Check();
}  
short DS18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL,TH;
	short tem;
    DS18B20_Start ();
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0xbe);  
    TL=DS18B20_Read_Byte();
    TH=DS18B20_Read_Byte();
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;
    }else temp=1; 	  
    tem=TH;
    tem<<=8;    
    tem+=TL;
    tem=(float)tem*0.620f; 
	if(temp)return tem;
	else return -tem;    
} 
 
