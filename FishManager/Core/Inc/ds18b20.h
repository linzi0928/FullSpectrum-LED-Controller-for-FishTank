#include "main.h"
void  DS18B20_IO_IN(void)  {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS18B20_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
}	
void  DS18B20_IO_OUT(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS18B20_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
}
#define	DS18B20_DQ_IN  HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin)
   	
uint8_t DS18B20_Init(void);
short DS18B20_Get_Temp(void);
void DS18B20_Start(void);
void DS18B20_Write_Byte(uint8_t dat);
uint8_t DS18B20_Read_Byte(void);
uint8_t DS18B20_Read_Bit(void);
uint8_t DS18B20_Check(void);
void DS18B20_Rst(void);
















