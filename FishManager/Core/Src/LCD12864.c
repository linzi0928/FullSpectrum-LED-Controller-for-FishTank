/*************************************************************/
// project    :	221-3059-1428
// driver IC  :	st7567
// LCD        :	1/65 duty, 1/9bias, 8.8V vop
// interface  :	SPI
// ver        :	00
// date       :  -07
// other      :	VDD 3.1v
/**************************************************************/
#include "LCD12864.h"
#include "stdarg.h"
#include "stdio.h"
void writec(uchar);
void stop(void);
void writed(uchar);

#define  uchar  unsigned char
#define  uint   unsigned int
 uchar vop=0x20;//¸ºÏÔ15£¬ÕýÏÔ20
 void flash(uint32_t t)
{
	t=t*5;
  while(t>0)
  {
    t--;             //TT-					   `
  }
}
//-----------------------------
void writec(uchar com)
{ 
	unsigned char i ;
	
	RS_L();

   for(i=0;i<8;i++)
    { 
		if(com&0x80)
			SDI_H();
		else
			SDI_L();
		com=com<<1;
	  SCL_H();
		flash(1);
	  SCL_L();
	  }
  
	RS_H();
 }

void writed(uchar dat)
{  
	unsigned char i;
	
    RS_H();

  for(i=0;i<8;i++)
    {
		if(dat&0x80)
			SDI_H();
		else
			SDI_L();
		dat=dat<<1;
	  SCL_H();
		flash(1);
	  SCL_L();
	  }
  
	RS_H();
}
void Clear_Scr ()
{
   uchar row,col;
   for (row=0xb0; row<0xb8; row++)
   {
      writec(row);//set page address
      writec(0x10);//set column address
      writec(0x00);
      for(col=0;col<128;col++)
      writed(0X00);
   }
}
 void init12864 ()
{
   RES_H();
   flash(1000);
   RES_L();
   flash(2000);
   RES_H();
   flash(1000);

 writec(0xe3);  // reset signal
 writec(0xa3);  //(0xa2 1/9 bias,1/65 duty )
 writec(0xa0);  // ADC select
 writec(0xc8);  // command output select
 writec(0x2f);  // power control
 writec(0x24);  // select resistor ratio Rb/Ra
 writec(0x81);  // select volume
 writec(vop);   // vop
 writec(0xf8);  // x4
 writec(0x08);  // x4
	Clear_Scr ();
 writec(0xaf);  //display on
}
// void Refresh_Cache()
// {
//  uchar col,row;
//  writec(0xb0);//set page address
//  writec(0x10);//set column address
//  writec(0x00);
// 	 for(row=0;row<8;row++)
//    {
// 	 for(col=0;col<128;col++)
// 		 {
// 			writed(Display_Cache[col][row]);
// 		 }
// 	 }
// }
void displayASC12X24(int x,int y,uchar p)
{
	int x_M,x_L,jt;
	x_M=x>>4;
	x_L=(x&0x0f);
	y=(y/8)+0xb0;
	writec(y);
	writec(0x10+x_M);//set column address
  writec(x_L);
	for(int jty=0;jty<6;jty++)
	{
		for(jt=0;jt<6;jt++)
		{
			writed(ASC12X24[((p-0x2e)*36)+jt+(jty*6)]);
		}
		if(jty%2!=0&&jty!=0)
		{
			writec(0x10+x_M);//set column address
			writec(x_L);
			y=y-0xb0;
			y=y+1;
			y=y+0xb0;
			writec(y);
		}
	}
}

void displayASC12X24_str(int x,int y,uchar p[])
{
	int jt,si;
	si=strlen((const char *)p);
	for(jt=0;jt<si;jt++)
	{
		displayASC12X24(x+(12*jt),y,*(p+jt));
	}
}
void displayASC5X8(int x,int y,uchar p)
{
	int x_M,x_L,jt;
	x_M=x>>4;
	x_L=(x&0x0f);
	y=(y/8)+0xb0;
	writec(y);
	writec(0x10+x_M);//set column address
  writec(x_L);
	
	for(jt=7;jt>=0;jt--)
	{
		writed(ASC5X8_Trans(((p-0x20)*8)+jt));
	}
	
}
void displayASC5X8_str(int x,int y,uchar p[])
{
	int jt,si;
	si=strlen((const char *)p);
	for(jt=0;jt<si;jt++)
	{
		displayASC5X8(x+(6*jt),y,*(p+jt));
	}
}
void displaychar(uchar *p)
{
   uchar row,col;

   for (row=0xb0; row<0xb8; row++)
   {
      writec(row);//set page address
      writec(0x10);//set column address
      writec(0x00);
      for(col=0;col<128;col++)
      writed(*p++);
   }
}
void printf_lcd(uint8_t x,uint8_t y,char *fmt, ...)
{
	y=y*8;
	//uint8_t nn;
	int s_len=0,lcd_offset=0;
//	uchar tmph,tmpl;
	const uint16_t CMD_BUFFER_LEN=256;
	char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN???????
//	uint16_t i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr);
	while(buffer[s_len])
	{
		if(buffer[s_len]<0x20)
		{
			buffer[s_len]=0x20;
		}
		s_len++;
	}
//	if(s_len>=27)
//		lcd_offset=s_len-27;

	displayASC5X8_str(x,y,(uint8_t *)buffer+lcd_offset);

	va_end(arg_ptr);
}
//unsigned char   ASC12X24_Trans(int numb)
//{
//	unsigned char result,row,col,col2;
//	int tt;
//	row=numb/48;
//	col=numb%48;
//	col2=pow(2,col);
//	for(tt=0;tt<48;tt++)
//	{	
//	result>>=1;
//	result=result|(((ASC12X24[row*48+tt])&col2)<<(7-col));
//	}
//	return result;
//}
unsigned char   ASC5X8_Trans(int numb)
{
	unsigned char row,col,col2;
	unsigned long result;
	int tt;
	row=numb/8;
	col=numb%8;
	col2=pow(2,col);
	for(tt=0;tt<7;tt++)
	{	
	result>>=1;
	result=result|(((ASC5X8[row*8+tt])&col2)<<(7-col));
	}
	return result;
}
void displayHZ12(int x, int y, int num)
{
	int x_M,x_L,jt;
	x_M=x>>4;
	x_L=(x&0x0f);
	y=(y/8)+0xb0;
	writec(y);
	writec(0x10+x_M);//set column address
  writec(x_L);
	for(jt=0;jt<12;jt++)
	{
		writed(HZ12X12_S[24*num+jt]);
	}
	y+=1;
	writec(y);
	writec(0x10+x_M);//set column address
  writec(x_L);
	for(jt=12;jt<24;jt++)
	{
		writed(HZ12X12_S[24*num+jt]);
	}
}
void displayHZ12_str(int x, int y, unsigned char  d,int num1,int num2)
{
	int jt;
	for(jt=num1;jt<num2+1;jt++)
	{
		displayHZ12(x+12*(jt-num1),y,jt);
	}
}
