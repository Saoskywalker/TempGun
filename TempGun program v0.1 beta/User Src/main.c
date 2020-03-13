/*****************************************************************************
Name: hammer
version: v1.0 Released
Editor: Aysi
Describe: 
History: 
2019.12.26: Created
*****************************************************************************/

/* Includes ------------------------------------------------------------------*/
// #define DEBUG
#include "UserBaseLib.h"
#include "delay.h"
#include "oled.h"
#include "mlx90615.h"
//#include <stdio.h>
// #include "UART_Frame.h"

/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/*Bee function*/
static const u16 BeeModPeriod[] = {2, 3, 4, 5, 6};
static const u16 BeeModCompare[] = {1, 2, 2, 2, 2};
u8 BeeMod = 0, BeeTime = 0;
void BeeFunction(void)
{
	static u16 BeeTimeCnt = 0;

	if (BeeTime > 0)
	{
		if (++BeeTimeCnt >= BeeModPeriod[BeeMod])
		{
			BeeTimeCnt = 0;
			BeeTime--;
		}
		else
		{
			if (BeeTimeCnt >= BeeModCompare[BeeMod])
				SOUND_PIN = 0;
			else
				SOUND_PIN = 1;
		}
	}
	else
	{
		BeeTimeCnt = 0;
		SOUND_PIN = 0;
	}
}

//key scan
static u8 MCU_DK_KeyData = 0;

u8 Key_Get(void)
{
  return MCU_DK_KeyData;
}

void Key_Scan(void)
{
  static u8 keyCnt2 = 0;
  static u16 KeyStartCnt = 0;

  if (POWER_PIN == 1)
  {
    if (++keyCnt2 >= 20) //sensibility
    {
      keyCnt2 = 20;
      MCU_DK_KeyData |= KEY_START;
      if (++KeyStartCnt >= 3000) //3s
      {
        KeyStartCnt = 3000;
        MCU_DK_KeyData |= KEY_POWER_KEEP;
      }
    }
  }
  else
  {
    keyCnt2 = 0;
    KeyStartCnt = 0;
    MCU_DK_KeyData &= ~KEY_START;
    MCU_DK_KeyData &= ~KEY_POWER_KEEP;
  }
}

void main(void)
{
  u16 advalue = 0, s1 = 0, s10 = 0, adtemp = 0;
  u8 VolLowCnt = 0, ChaLowCnt = 0;
  u8 TaskNumber = 1, KeyValue = 0;
  u8 KeyUp = 0, KeyT = 0;
  u8 averageCnt = 0;
  u16 tttt = 0;

  P17_PushPull_Mode;
  KEEP_PIN = 1; //power kepp
  //io init
  //note: after power up, N76E003 IO is only input(float) mode, P0,P1,P3 is 1 P2 only input mode
  clr_P2S_0; //POWER KEY, HIZ mode

  P02_Quasi_Mode;
  SOUND_PIN = 0;

#ifndef DEBUG
  IWDG_Configuration(); //Open IWDG
#endif

  // Uart1Init(115200);
  Tim2_Time_Upmode_conf(TIMER_DIV4_VALUE_100us); //100us
  set_EA;                                        //Open main interrupt

  AD1Init();
  mlx90615Init();

  OLED_Init(); //init oled
  OLED_Clear();

  OLED_ShowString(5, 6, "TEMP:", 16);

  while (1)
  {
#ifndef DEBUG
    IWDG_Feed; //Clear IWDG cnt
#endif
    if (FlagState.ms2)
    {
      FlagState.ms2 = 0;
      switch (TaskNumber)
      {
      case 1: //UART T&R process
      {
        TaskNumber++;
        break;
      }
      case 2: //KEY GET
      {
        KeyValue = Key_Get();
        TaskNumber++;
        break;
      }
      case 3: //key process
      {
        if ((KeyValue & KEY_START) != KEY_START && (KeyValue & KEY_POWER_KEEP) != KEY_POWER_KEEP)
        {
          KeyUp = 1;
        }
        if (((KeyValue & KEY_POWER_KEEP) == KEY_POWER_KEEP) && KeyUp)
        {
          KeyUp = 0;
        }
        if (((KeyValue & KEY_START) == KEY_START) && KeyUp)
        {
          KeyUp = 0;
          FlagState.work = 1;
        }
        TaskNumber++;
        break;
      }
      case 4: //work process
      {
        if (++s10 >= 6000) //60s
        {
          //over time without operate, close machine
          s10 = 0;
          OLED_Clear();
          KEEP_PIN = 0; //close keep voltage, after releasing key, it will close
          Set_All_GPIO_Only_Input_Mode;
        }
        if (FlagState.work)
        {
          s10 = 0;
          tttt = mlxGetToTemp();
          OLED_ShowString(36,1,"checking",16);
          OLED_ShowNum(50+20, 6, tttt%10, 3, 16);
          OLED_ShowString(78,6,".",16);
          OLED_ShowNum(50, 6, tttt/10, 3, 16);
          OLED_ShowString(100,6,"C",16);
          OLED_ShowString(36,1,"---OK---",16);
          BeeMod = 1;
          BeeTime = 100;
          FlagState.work = 0;
        }
        TaskNumber++;
        break;
      }
      case 5: //error process
      {
        TaskNumber = 1;
        /* if (++s1 >= 100) //1s
        {
          s1 = 0;
          advalue = 0;
          for (averageCnt = 0; averageCnt < 8; averageCnt++)
          {
            set_ADCS; //start adc convert
            while (ADCF == 0); //check EOC flag
            clr_ADCF; //clear EOC flag
            adtemp = 0;
            adtemp = ADCRH;
            adtemp = (adtemp << 4) + ADCRL;
            advalue += adtemp;
          }
          advalue = advalue >> 3;          // x/8
          if (advalue <= 2047) //battery no power, about 2.4V
          {
            OLED_ShowString(36,1,"power low",16);
            delay_ms(500);
            delay_ms(500);
#ifndef DEBUG
            IWDG_Feed; //Clear IWDG cnt
#endif
            OLED_ShowString(36,1,"-closing-",16);
            delay_ms(500);
            delay_ms(500);
            OLED_Clear();
            KEEP_PIN = 0; //close keep voltage, after releasing key, it will close
            Set_All_GPIO_Only_Input_Mode;
          }
        } */
        break;
      }
      default:
      {
        TaskNumber = 5;
        FlagState.error = 1;
        break;
      }
      }
    }
  }
}
