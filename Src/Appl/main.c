#include "IntCtrl.h"
#include "Systick.h"
#include "Dio.h"
#include "Port.h"

static volatile uint32 Tick;
uint32 OnTime=4000;
uint32 OffTime=2000;
uint8 OnFlag=1;
	
void SysTick_Handler(void){
	Tick++;
	if(Tick ==(OnTime/1000) && OnFlag==1){
			Dio_FlipChannel(Port_Pin_F1);
			Tick=0;
			OnFlag=0;
		}
		else if(Tick == (OffTime/1000) && OnFlag==0){
			Dio_FlipChannel(Port_Pin_F1);
			Tick=0;
			OnFlag=1;
		}
}

int main(void)
{
	Port_Init(Port_Config);
	Systick_Off();
	/*set period to trigger an interrupt every 1 second*/
	Systick_SetPeriod(1000);
	/*Light up the Led*/
	Dio_WriteChannel(Channel_F1,STD_HIGH);
	uint32 channelLevel = Dio_ReadChannel(Port_Pin_F1);
	/*start timer*/
	Systick_Enable();
	
	while(1){
		
	}
	return E_OK;
}
