/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
	
	 Copyright [2021] [GIE Studio]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
	 GIE�����ң�����Apache2.0Э�鿪Դ
	 
	 ��ˮ������ȫ���׿�����
	 ����LED����3·��PT4115������������0.9A��������������0.3A����12V����
	 PWM����޷�Ƶ����Ƶ����������Ƶ�����ճ������Զ�����
	 �¶�̽ͷ������ˮ����ֿ�18B20
	 ��Ļ��LX12864B11
	 ��ӭ��עbվup����GIE�����ң���ó���ɻ�
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
float tempu=0;
int hour=0,min=0,sec=0;//����ʱ��
int light_hour=9,light_min=0,light_time=9,feed_time=12,sec_prev=0,sun_flag=0;
char light_mode[]="A\0D";
int PWM_Compare_Val[3]={0,0,0};
int knob_val[3];
uint8_t light_flag=0;
RTC_TimeTypeDef stimestructure;
void Main_UI_Refresh(void);
void Key_Scan(void);
void PWM_Refresh(void);
void Value_Refresh(void);
void PWM_Manual_Convert(void);
void PWM_Next_Calc(int *PWM_now,int *PWM_goal);
void PWM_Value_Valid(void);
void PWM_Auto_Convert(int *PWM_PREV);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	DS18B20_Init();
	init12864();
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		Value_Refresh();//ʱ�䡢��ť���¶�ֵˢ��
		PWM_Refresh();//PWM���ֵˢ�£��ճ������Զ��������ֶ����ƣ�
		Key_Scan();//������ʱ�߼�
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Main_UI_Refresh(void)//��������ʾˢ�º���
{
	uint8_t dis_buffer[10];
	memset(dis_buffer,0,10);
	
	sprintf(( char *)dis_buffer,"%.1f:",tempu);
	displayASC12X24_str(0,0,dis_buffer);//�����¶���ʾ
	for(int ixx=0;ixx<8;ixx++)
	{
		printf_lcd(64,ixx,"|");
	}
	printf_lcd(0,4,"%02d:%02d:%02d",hour,min,stimestructure.Seconds);
	printf_lcd(0,5,"SR %02d:%02d",light_hour,light_min);//�����
	printf_lcd(0,6,"DT %02dH   ",light_time);
	printf_lcd(0,7,"GIE Studio");
	printf_lcd(0,3,"-----------");
	
	printf_lcd(72,0,"Mode %s",light_mode);
	printf_lcd(72,1,"CHW %02.1f%% ",(float)PWM_Compare_Val[0]/40.96f);
	printf_lcd(72,2,"CHB %02.1f%% ",(float)PWM_Compare_Val[1]/40.96f);
	printf_lcd(72,3,"CHU %02.1f%% ",(float)PWM_Compare_Val[2]/40.96f);
	
	printf_lcd(72,4,"TW %04d",knob_val[0]);
	printf_lcd(72,5,"TB %04d",knob_val[1]);
	printf_lcd(72,6,"TU %04d",knob_val[2]);
	printf_lcd(72,7,"2021-9-17");
}
void Value_Refresh(void)//����ֵ���º���
{
//	short temp_ds=0;
	HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);//����ʱ��
	hour=stimestructure.Hours;
	min=stimestructure.Minutes;
	tempu=(float)DS18B20_Get_Temp()/10;//��ȡ�¶�
	if(sec_prev!=stimestructure.Seconds)
	{
		sec_prev=stimestructure.Seconds;
		Main_UI_Refresh();
	}
		for(int ixx=0;ixx<3;ixx++)//��ȡ��ťֵ
		{
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1,5000);
			knob_val[ixx]=HAL_ADC_GetValue(&hadc1);
		}
		HAL_ADC_Stop(&hadc1);
}
void PWM_Refresh(void)//�Զ��ճ������߼����ֶ�ģʽ�߼�
{
		if(light_mode[0]=='A')
		{
			//�Զ�ģʽ
				int PWM_Compare_Val_Prev[3];//�洢�л������õ���ֵ
				for(int ixx=0;ixx<3;ixx++)
				{
					PWM_Compare_Val_Prev[ixx]=PWM_Compare_Val[ixx];
				}
				int now_time_min=(60*stimestructure.Hours)+stimestructure.Minutes;//�������Ⱦ���ʹ�þ���ʱ����㣬����ֱ�ӼӼ���
				if((now_time_min>=(light_hour*60+light_min))&&(now_time_min<((light_hour+light_time)*60)+light_min))//�ڿ���ʱ����
				{
					int now_time_sec=(60*now_time_min)+stimestructure.Seconds-(60*(light_hour*60+light_min));//��ȡ���ճ�����
					if(now_time_sec<4096)//�ճ�ʱ��Σ�4096s��Լ1.1Сʱ��
					{
						PWM_Compare_Val[0]=0;//�׹⣨�ճ�ʱ���ް׹⣩
						PWM_Compare_Val[1]=now_time_sec;//����������������
						PWM_Compare_Val[2]=0;//���⣨�ճ�ʱ��������⣩
					}
					else if(now_time_sec<6144)//��������ǿ�׶Σ�2048s��Լ34���ӣ�
					{
						PWM_Compare_Val[0]=2*(now_time_sec-4096);
						PWM_Compare_Val[1]=4096;
						PWM_Compare_Val[2]=2*(now_time_sec-4096);
					}
					else if(now_time_sec>=(60*(light_hour*60)-2048))//����ʱ��Σ�2048s��Լ34���ӣ�
					{
						for(int ixx=0;ixx<3;ixx+=2)
						{
							if(PWM_Compare_Val[ixx]>=2)
								PWM_Compare_Val[ixx]=4096-(2*(now_time_sec-(3600*light_hour-2048)));//����ǰ�׹�����������0
						}
						PWM_Compare_Val[1]=4096-(now_time_sec-(3600*light_hour-2048));//����΢������
					}
					else
					{
						for(int ixx=0;ixx<3;ixx++)
							PWM_Compare_Val[ixx]=4096;
					}
				}
				else if((now_time_min>=((light_hour+light_time)*60)+light_min)&&(now_time_min<=((light_hour+light_time+2)*60)+light_min))//�������ά��1.7Сʱ������΢��������߹�����
				{
					int sd_time_sec=(60*now_time_min)+stimestructure.Seconds-(60*(((light_hour+light_time)*60)+light_min));//��������������
					PWM_Compare_Val[0]=0;//�رհ׹�
					PWM_Compare_Val[1]=2048-(sd_time_sec/3);//�����������뽥��
					PWM_Compare_Val[2]=0;//�ر������
					if(PWM_Compare_Val[1]<0)
						PWM_Compare_Val[1]=0;
				}
				else//��ȫ��ʱ��
				{
					PWM_Compare_Val[0]=0;
					PWM_Compare_Val[1]=0;
					PWM_Compare_Val[2]=0;
				}
				if(light_mode[2]=='D')//����Ǵ��ֶ�ģʽ�л���������ִ�й��߽���
				{
					light_mode[2]='A';
					PWM_Auto_Convert(PWM_Compare_Val_Prev);
				}
		}
		else
		{
			//�ֶ�ģʽ
			if(light_mode[2]=='A')
			PWM_Manual_Convert();//�����л�
			else
			{
				PWM_Compare_Val[0]=knob_val[0];
				PWM_Compare_Val[1]=knob_val[1];
				PWM_Compare_Val[2]=knob_val[2];
			}
		}
	PWM_Value_Valid();
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,PWM_Compare_Val[0]);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,PWM_Compare_Val[1]);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,PWM_Compare_Val[2]);
}
void PWM_Manual_Convert(void)//�ֶ�ģʽ�µĽ���Ч��
{
	if(PWM_Compare_Val[0]==knob_val[0]&&PWM_Compare_Val[1]==knob_val[1]&&PWM_Compare_Val[2]==knob_val[2])
	{
		return;
	}
	else
	{
		while(1)
		{
			PWM_Next_Calc(PWM_Compare_Val,knob_val);
			PWM_Next_Calc(PWM_Compare_Val+1,knob_val+1);
			PWM_Next_Calc(PWM_Compare_Val+2,knob_val+2);
			Soft_delay_us(100);
			if(PWM_Compare_Val[0]==knob_val[0]&&PWM_Compare_Val[1]==knob_val[1]&&PWM_Compare_Val[2]==knob_val[2])
			 break;
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,PWM_Compare_Val[0]);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,PWM_Compare_Val[1]);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,PWM_Compare_Val[2]);
		}
		light_mode[2]='D';
	}
}
void PWM_Auto_Convert(int *PWM_PREV)//�Զ�ģʽ�µĽ���Ч��
{
	while(1)
	{
		PWM_Next_Calc(PWM_PREV,PWM_Compare_Val);
		PWM_Next_Calc(PWM_PREV+1,PWM_Compare_Val+1);
		PWM_Next_Calc(PWM_PREV+2,PWM_Compare_Val+2);
		Soft_delay_us(100);
		if(PWM_Compare_Val[0]==PWM_PREV[0]&&PWM_Compare_Val[1]==PWM_PREV[1]&&PWM_Compare_Val[2]==PWM_PREV[2])
		 break;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,PWM_PREV[0]);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,PWM_PREV[1]);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,PWM_PREV[2]);
	}
}
void PWM_Next_Calc(int *PWM_now,int *PWM_goal)//���ݵ�ǰ��Ŀ����㽥��ֵ
{
	if(((PWM_now[0]-PWM_goal[0])>0))
	{
		PWM_now[0]--;
	}
	else if(PWM_now[0]-PWM_goal[0]<0)
	{
		PWM_now[0]++;
	}
}
void Key_Scan(void)//������ʱ�����Զ�ģʽ�л�
{
	uint8_t key1n=0;
	uint32_t cnt_f=0;
	if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//key2Ϊģʽ�л���
	{
		HAL_Delay(10);
		if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//key2Ϊģʽ�л���
		{	
			while(!HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin));
			if(light_mode[0]=='A')
			{
				light_mode[0]='M';
				light_mode[2]='A';
			}
			else
			{
				light_mode[0]='A';
			}
		}
	}
	if(HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin)==0)//---------------key1Ϊ���ܼ������ü���--------------------
	{
		while(1)
		{
			if(HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin)==0)//��ʱ���ٴ�ȷ�ϰ�������
			{	
				HAL_Delay(10);
				if(HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin)==0)//---------------key1Ϊ���ܼ������ü���--------------------
				{	
					HAL_Delay(20);
					while(!HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin));
					key1n++;
					if(key1n>6)
						key1n=6;
				}
			}
			switch(key1n)
			{	
			case 1:
						if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//��ʱ���ٴ�ȷ�ϰ�������
						{	
							HAL_Delay(10);
							if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//---------------key1Ϊ���ܼ������ü���--------------------
							{	
								while(!HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin));
								hour++;
								if(hour>23)
									hour=0;
							}
						}
						break;
			case 2:
						if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//��ʱ���ٴ�ȷ�ϰ�������
						{	
							HAL_Delay(10);
							if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//---------------key1Ϊ���ܼ������ü���--------------------
							{	
								while(!HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin));
								min++;
								if(min>59)
									min=0;
							}
						}
					break;
			case 3:
					if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//��ʱ���ٴ�ȷ�ϰ�������
					{	
						HAL_Delay(10);
						if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//---------------key1Ϊ���ܼ������ü���--------------------
						{	
							while(!HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin));
							light_hour++;
							if(light_hour>16)
								light_hour=0;
						}
					}
				break;
		case 4:
					if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//��ʱ���ٴ�ȷ�ϰ�������
					{	
						HAL_Delay(10);
						if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//---------------key1Ϊ���ܼ������ü���--------------------
						{	
							while(!HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin));
							light_min++;
							if(light_min>59)
								light_min=0;
						}
					}
				break;
			case 5:
					if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//��ʱ���ٴ�ȷ�ϰ�������
					{	
						HAL_Delay(10);
						if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)==0)//---------------key1Ϊ���ܼ������ü���--------------------
						{	
							while(!HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin));
							light_time++;
							if(light_time>18)
								light_time=3;
						}
					}
					break;		
			case 6:
					stimestructure.Hours=hour;
					stimestructure.Minutes=min;
					HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
					light_mode[2]='D';
					return;			
			}
			cnt_f++;
			if(cnt_f<=50)
			{
				printf_lcd(0,4,"%02d:%02d:%02d",hour,min,stimestructure.Seconds);
				printf_lcd(0,5,"SR %02d:%02d",light_hour,light_min);
				printf_lcd(0,6,"DT %02dH   ",light_time);
			}
			if(cnt_f>50&&cnt_f<100)
				{	
					if(key1n==1)	
					{
						printf_lcd(0,4,"  :%02d:%02d",min,stimestructure.Seconds);
					}
					else if(key1n==2)	
					{
						printf_lcd(0,4,"%02d:  :%02d",hour,stimestructure.Seconds);
					}
					else if(key1n==3)	
					{
						printf_lcd(0,5,"SR   :%02d",light_min);
					}
					else if(key1n==4)
					{
						printf_lcd(0,5,"SR %02d:  ",light_hour);
					}
					else
					{
						printf_lcd(0,6,"DT   H   ");
					}
				}
			if(cnt_f>=100)
				{
					cnt_f=0;
				}
		}
	}
}
void PWM_Value_Valid(void)//�޶�ֵ��PWM��Χ��
{
	for(int ixx=0;ixx<3;ixx++)
	{
		if(PWM_Compare_Val[ixx]<0)
			PWM_Compare_Val[ixx]=0;
		else if(PWM_Compare_Val[ixx]>4095)
			PWM_Compare_Val[ixx]=4095;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
