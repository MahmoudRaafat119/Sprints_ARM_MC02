#include "IntCtrl.h"
#include "Systick.h"
#include "Dio.h"
#include "Port.h"
#include "Mcu.h"

static volatile uint32 Tick;
void SysTick_Handler(void);
void SysTick_Handler(void){
	Tick++;
	}

Dio_LevelType channelLevel= STD_LOW;
int main(void)
{
	uint32 OnTime=5000;
	uint32 OffTime=3000;
	uint8 OnFlag=1;
	Port_Init(Port_Config);
	Systick_Off();
		/*set period to trigger an interrupt every 1 second*/
		Systick_SetPeriod(1000);
		/*Light up the Led*/
		Dio_WriteChannel(Channel_B7,STD_HIGH);
		channelLevel = Dio_ReadChannel(Port_Pin_B1);
		/*start timer*/
		Systick_Enable();
	
	while(1){
		if(Tick ==(OnTime/1000) && OnFlag==1){
			Dio_FlipChannel(Port_Pin_B1);
			Tick=0;
			OnFlag=0;
		}
		else if(Tick == (OffTime/1000) && OnFlag==0){
			Dio_FlipChannel(Port_Pin_B1);
			Tick=0;
			OnFlag=1;
		}
	}
	return E_OK;
}
