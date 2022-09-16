/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  FileName.c
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "Timer_Cfg.h"
#include "Timer.h"
#include "Mcu_hw.h"
/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/
#define MAX_NUMBER_OF_GPIO_GPT 		12u

#define TAEN 		0
#define TBEN 		8

#define TACDIR	4

#define PREDEF_TIMER					GPT_32_64_BIT_WIDE_TIMER4
/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
GptNotification	locGptNotification[MAX_NUMBER_OF_GPIO_GPT];

static const Gpt_ConfigType*			globalGptConfig;
static volatile uint32 gptPredefTimer_1USPrescale;
static volatile uint32 gptPredefTimer_100US_32BitPrescale;
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
extern volatile uint32 GlobalSystemClock;
static const uint32 Gpt_BaseAddress[MAX_NUMBER_OF_GPIO_GPT] = {GPT_16_32_BIT_TIMER0_BASE_ADDRESS,			
	                                                           GPT_16_32_BIT_TIMER1_BASE_ADDRESS,		
	                                                           GPT_16_32_BIT_TIMER2_BASE_ADDRESS,			
	                                                           GPT_16_32_BIT_TIMER3_BASE_ADDRESS,			
	                                                           GPT_16_32_BIT_TIMER4_BASE_ADDRESS,			
	                                                           GPT_16_32_BIT_TIMER5_BASE_ADDRESS,			
	                                                           GPT_32_64_BIT_WIDE_TIMER0_BASE_ADDRESS,
	                                                           GPT_32_64_BIT_WIDE_TIMER1_BASE_ADDRESS,
	                                                           GPT_32_64_BIT_WIDE_TIMER2_BASE_ADDRESS,
	                                                           GPT_32_64_BIT_WIDE_TIMER3_BASE_ADDRESS,
	                                                           GPT_32_64_BIT_WIDE_TIMER4_BASE_ADDRESS,
	                                                           GPT_32_64_BIT_WIDE_TIMER5_BASE_ADDRESS};
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
* \Syntax          : void Gpt_Init( const Gpt_ConfigType* ConfigPtr)        
* \Description     : Initializes the GPT driver.                                   
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : ConfigPtr   Pointer to a selected configuration structure                     
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void Gpt_Init( const Gpt_ConfigType* ConfigPtr)
{
	uint32 i,prescale;
	uint32 gptBaseAddress;
	Gpt_ChannelType  			locChannel;
	ChannelMode					locChannelMode;
	Gpt_ChannelTickFrequency 	locChannelTickFreq;
	GptChannelTickValueMax  	locChannelTickMaxValue;
	globalGptConfig = ConfigPtr;
	
	for(i=0;i<GPT_ACTIVATED_CHANNELS_SIZE;i++)
	{
		locChannel					= ConfigPtr[i].channel;
		locChannelTickFreq			= ConfigPtr[i].channelTickFreq;
		locChannelTickMaxValue		= ConfigPtr[i].channelTickMaxValue;
		locChannelMode				= ConfigPtr[i].channelMode;
		
		locGptNotification[locChannel]= ConfigPtr[i].gptNotification;
		
		gptBaseAddress = Gpt_BaseAddress[locChannel];
		GPTMCTL(gptBaseAddress) =	0;
		
		/*
		* 16/32-bit timer, selects the 16-bit timer configuration.
		* 32/64-bit wide timer, selects the 32-bit timer configuration.
		*/
		GPTMCFG(gptBaseAddress) = 0x4;

		GPTMTAMR(gptBaseAddress) |= (1<<TACDIR);

		if (locChannelMode == GPT_CH_MODE_ONESHOT)
		{
			GPTMTAMR(gptBaseAddress) |= 0x1;
		}
		else if (locChannelMode == GPT_CH_MODE_CONTINUOUS)
		{
			GPTMTAMR(gptBaseAddress) |= 0x2; 
		}
		
		Gpt_DisableNotification(locChannel);
	}		
	
		
/* Predef timers */
	gptPredefTimer_1USPrescale = GlobalSystemClock / 1000;
	for(i=2;;i*=2)
	{
		if(gptPredefTimer_1USPrescale/i == 1)
		{
			if(gptPredefTimer_1USPrescale%i <= (i/2))
			{
				gptPredefTimer_1USPrescale = i;
			}
			else
			{
				gptPredefTimer_1USPrescale = i*2;
			}
			break;
		}
	}	
	gptPredefTimer_100US_32BitPrescale = GlobalSystemClock / 10;
	for(i=2;;i*=2)
	{
		if(gptPredefTimer_100US_32BitPrescale/i == 1)
		{
			if(gptPredefTimer_100US_32BitPrescale%i <= (i/2))
			{
				gptPredefTimer_100US_32BitPrescale = i;
			}
			else
			{
				gptPredefTimer_100US_32BitPrescale = i*2;
			}
			break;
		}
	}	
	
#if GPT_PREDEF_TIMER_1US_16BIT_STATUS == Enable
	gptBaseAddress = Gpt_BaseAddress[PREDEF_TIMER];
	GPTMCTL(gptBaseAddress) =	0;
	GPTMCFG(gptBaseAddress) = 0x4;
	/* counting up */
	GPTMTAMR(gptBaseAddress) |= (1<<TACDIR);
	/* continuous mode*/
	GPTMTAMR(gptBaseAddress) |= 0x2;
	/* Disablling interupt notifation */
	Gpt_DisableNotification(PREDEF_TIMER);	
	
	GPTMTAILR(gptBaseAddress) = 0xFFFF * gptPredefTimer_1USPrescale;
		
#endif
	
#if GPT_PREDEF_TIMER_1US_24BIT_STATUS == Enable
	gptBaseAddress = Gpt_BaseAddress[PREDEF_TIMER];
	GPTMCTL(gptBaseAddress) =	0;
	GPTMCFG(gptBaseAddress) = 0x4;
	/* counting up */
	GPTMTAMR(gptBaseAddress) |= (1<<TACDIR);
	/* continuous mode*/
	GPTMTAMR(gptBaseAddress) |= 0x2; 
	/* Disablling interupt notifation */
	Gpt_DisableNotification(PREDEF_TIMER);	
	
	GPTMTAILR(gptBaseAddress) = 0xFFFFFF * gptPredefTimer_1USPrescale;
	
#endif

#if GPT_PREDEF_TIMER_1US_32BIT_STATUS == Enable
	gptBaseAddress = Gpt_BaseAddress[PREDEF_TIMER];
	GPTMCTL(gptBaseAddress) =	0;
	GPTMCFG(gptBaseAddress) = 0x4;
	/* counting up */
	GPTMTAMR(gptBaseAddress) |= (1<<TACDIR);	
	/* continuous mode*/
	GPTMTAMR(gptBaseAddress) |= 0x2; 
	/* Disablling interupt notifation */
	Gpt_DisableNotification(PREDEF_TIMER);
	
	GPTMTAILR(gptBaseAddress) = 0xFFFFFFFF * gptPredefTimer_1USPrescale;
	
#endif
		
#if GPT_PREDEF_TIMER_100US_32BIT_STATUS == Enable
	gptBaseAddress = Gpt_BaseAddress[PREDEF_TIMER];
	GPTMCTL(gptBaseAddress) =	0;
	GPTMCFG(gptBaseAddress) = 0x4;
	/* counting up */
	GPTMTAMR(gptBaseAddress) |= (1<<TACDIR);
		/* continuous mode*/
	GPTMTAMR(gptBaseAddress) |= 0x2;
	/* Disablling interupt notifation */
	Gpt_DisableNotification(PREDEF_TIMER);
	
	GPTMTAILR(gptBaseAddress) = 0xFFFFFFFF * gptPredefTimer_100US_32BitPrescale;
	
#endif	
}


/******************************************************************************
* \Syntax          : void Gpt_DisableNotification( Gpt_ChannelType Channel )        
* \Description     : Reentrant (but not for the same timer channel)                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : Channel   Numeric identifier of the GPT channel.                     
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void Gpt_DisableNotification( Gpt_ChannelType Channel )
{
	uint32 gptBaseAddress;
	gptBaseAddress = Gpt_BaseAddress[Channel];
	GPTMIMR(gptBaseAddress) = 0;
}
/******************************************************************************
* \Syntax          : void Gpt_EnableNotification( Gpt_ChannelType Channel )        
* \Description     : Enables the interrupt notification for a channel (relevant in normal mode).                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant (but not for the same timer channel)                                             
* \Parameters (in) : Channel   Numeric identifier of the GPT channel.                    
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void Gpt_EnableNotification( Gpt_ChannelType Channel )
{
	uint32 gptBaseAddress;
	gptBaseAddress = Gpt_BaseAddress[Channel];
	GPTMIMR(gptBaseAddress) = 0x1;
}

/******************************************************************************
* \Syntax          : Gpt_GetTimeElapsed( Gpt_ChannelType Channel )        
* \Description     : Returns the time already elapsed.                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : Channel   Numeric identifier of the GPT channel.                    
* \Parameters (out): Gpt_ValueType                                                      
* \Return value:   : Gpt_ValueType  uint32                                  
*******************************************************************************/
Gpt_ValueType Gpt_GetTimeElapsed( Gpt_ChannelType Channel )
{
	Gpt_ValueType value;
	uint32 i;
	uint32 prescale;
	uint32 gptBaseAddress;
	Gpt_ChannelTickFrequency 	locChannelTickFreq;
	GptChannelTickValueMax  	locChannelMaxValue;
	
	gptBaseAddress = Gpt_BaseAddress[Channel];
	for(i=0;i<MAX_NUMBER_OF_GPIO_GPT;i++)
	{
		if (globalGptConfig[i].channel == Channel)
		{
			locChannelTickFreq = globalGptConfig[i].channelTickFreq;
			locChannelMaxValue = globalGptConfig[i].channelTickMaxValue;
			
			break;
		}
	}
	
	prescale = GlobalSystemClock / locChannelTickFreq;
	
	for(i=2;;i*=2)
	{
		if(prescale/i == 1)
		{
			if(prescale%i <= (i/2))
			{
				prescale = i;
			}
			else
			{
				prescale = i*2;
			}
			break;
		}
	}
	value = (GPTMTAV(gptBaseAddress) / prescale) & locChannelMaxValue;
	
	return value;
}

/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
Std_ReturnType Gpt_GetPredefTimerValue( Gpt_PredefTimerType PredefTimer, uint32* TimeValuePtr)
{
	Std_ReturnType returnValue = E_NOT_OK;
	uint32 gptBaseAddress;
	
	gptBaseAddress = Gpt_BaseAddress[PREDEF_TIMER];
	if (PredefTimer == GPT_PREDEF_TIMER_1US_16BIT )
	{
		*TimeValuePtr = (GPTMTAV(gptBaseAddress) / gptPredefTimer_1USPrescale) & 0xFFFF;
		returnValue = E_OK;
	}
	else if (PredefTimer == GPT_PREDEF_TIMER_1US_24BIT )
	{
		*TimeValuePtr = (GPTMTAV(gptBaseAddress) / gptPredefTimer_1USPrescale) & 0xFFFFFF;
		returnValue = E_OK;
	}
	else if (PredefTimer == GPT_PREDEF_TIMER_1US_32BIT )
	{
		*TimeValuePtr = (GPTMTAV(gptBaseAddress) / gptPredefTimer_1USPrescale) & 0xFFFFFFFF;
		returnValue = E_OK;
	}
	else if (PredefTimer == GPT_PREDEF_TIMER_100US_32BIT )
	{
		*TimeValuePtr = (GPTMTAV(gptBaseAddress) / gptPredefTimer_100US_32BitPrescale) & 0xFFFFFFFF;
		returnValue = E_OK;
	}
	else
	{
		returnValue = E_NOT_OK;
	}
	return returnValue;
}

/******************************************************************************
* \Syntax          : Gpt_ValueType Gpt_GetTimeRemaining( Gpt_ChannelType Channel )       
* \Description     : Returns the time remaining until the target time is reached.                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : Channel   Numeric identifier of the GPT channel.                     
* \Parameters (out): None                                                      
* \Return value:   : Gpt_ValueType  uint32                                 
*******************************************************************************/
Gpt_ValueType Gpt_GetTimeRemaining( Gpt_ChannelType Channel )
{
	Gpt_ValueType value, currentValue, remainingValue;
	uint32 i;
	uint32 prescale;
	uint32 gptBaseAddress;
	Gpt_ChannelTickFrequency 	locChannelTickFreq;
	GptChannelTickValueMax  	locChannelMaxValue;
	
	gptBaseAddress = Gpt_BaseAddress[Channel];
	
	for(i=0;i<MAX_NUMBER_OF_GPIO_GPT;i++)
	{
		if (globalGptConfig[i].channel == Channel)
		{
			locChannelTickFreq = globalGptConfig[i].channelTickFreq;
			locChannelMaxValue = globalGptConfig[i].channelTickMaxValue;
			
			break;
		}
	}
	
	prescale = GlobalSystemClock / locChannelTickFreq;
	
	for(i=2;;i*=2)
	{
		if(prescale/i == 1)
		{
			if(prescale%i <= (i/2))
			{
				prescale = i;
			}
			else
			{
				prescale = i*2;
			}
			break;
		}
	}
	currentValue = (GPTMTAV(gptBaseAddress) / prescale) & locChannelMaxValue;
	value = (GPTMTAILR(gptBaseAddress) / prescale) & locChannelMaxValue;
	
	remainingValue = value - currentValue;
	
	return remainingValue;
}


/******************************************************************************
* \Syntax          : void Gpt_StartTimer( Gpt_ChannelType Channel, Gpt_ValueType Value );        
* \Description     : Starts a timer channel.                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant(but not for the same timer channel)                                             
* \Parameters (in) : Channel   Numeric identifier of the GPT channel. 
*										 Value		 Target time in number of ticks.
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void Gpt_StartTimer( Gpt_ChannelType Channel, Gpt_ValueType Value )
{
	uint32 i;
	uint32 prescale;
	uint32 gptBaseAddress;
	Gpt_ChannelTickFrequency 	locChannelTickFreq;
	
	gptBaseAddress = Gpt_BaseAddress[Channel];
		for(i=0;i<MAX_NUMBER_OF_GPIO_GPT;i++)
	{
		if (globalGptConfig[i].channel == Channel)
		{
			locChannelTickFreq = globalGptConfig[i].channelTickFreq;			
			break;
		}
	}
	
	prescale = GlobalSystemClock / locChannelTickFreq;
	
	for(i=2;;i*=2)
	{
		if(prescale/i == 1)
		{
			if(prescale%i <= (i/2))
			{
				prescale = i;
			}
			else
			{
				prescale = i*2;
			}
			break;
		}
	}
	/* adding the value deppending on the freq required */
	GPTMTAILR(gptBaseAddress) = Value * prescale;
	
	GPTMCTL(gptBaseAddress) |= (1<<TAEN);
}

/******************************************************************************
* \Syntax          : void Gpt_StopTimer( Gpt_ChannelType Channel )       
* \Description     : Stops a timer channel.                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant (but not for the same timer channel)                                             
* \Parameters (in) : Channel   Numeric identifier of the GPT channel.                     
* \Parameters (out): None                                                      
* \Return value:   : None                           
*******************************************************************************/
void Gpt_StopTimer( Gpt_ChannelType Channel )
{
	uint32 gptBaseAddress;
	gptBaseAddress = Gpt_BaseAddress[Channel];
	
	GPTMCTL(gptBaseAddress) &= (~(1<<TAEN));
}



void TIMER0A_Handler(void)
{
	static GptNotification gptNotificationFn_TIMER0A;
	if (locGptNotification[GPT_16_32_BIT_TIMER0] != NULL)
	{
		gptNotificationFn_TIMER0A = locGptNotification[GPT_16_32_BIT_TIMER0];
		gptNotificationFn_TIMER0A();
	}
}

void TIMER1A_Handler(void)
{
	static GptNotification gptNotificationFn_TIMER1A;
	if (locGptNotification[GPT_16_32_BIT_TIMER1] != NULL)
	{
		gptNotificationFn_TIMER1A = locGptNotification[GPT_16_32_BIT_TIMER1];
		gptNotificationFn_TIMER1A();
	}
}

void TIMER2A_Handler(void)
{
	static GptNotification gptNotificationFn_TIMER2A;
	if (locGptNotification[GPT_16_32_BIT_TIMER2] != NULL)
	{
		gptNotificationFn_TIMER2A = locGptNotification[GPT_16_32_BIT_TIMER2];
		gptNotificationFn_TIMER2A();
	}
}

void TIMER3A_Handler(void)
{
	static GptNotification gptNotificationFn_TIMER3A;
	if (locGptNotification[GPT_16_32_BIT_TIMER3] != NULL)
	{
		gptNotificationFn_TIMER3A = locGptNotification[GPT_16_32_BIT_TIMER3];
		gptNotificationFn_TIMER3A();
	}
}

void TIMER4A_Handler(void)
{
	static GptNotification gptNotificationFn_TIMER4A;
	if (locGptNotification[GPT_16_32_BIT_TIMER4] != NULL)
	{
		gptNotificationFn_TIMER4A = locGptNotification[GPT_16_32_BIT_TIMER4];
		gptNotificationFn_TIMER4A();
	} 
}

void TIMER5A_Handler(void)
{
	static GptNotification gptNotificationFn_TIMER5A;
	if (locGptNotification[GPT_16_32_BIT_TIMER5] != NULL)
	{
		gptNotificationFn_TIMER5A = locGptNotification[GPT_16_32_BIT_TIMER5];
		gptNotificationFn_TIMER5A();
	}
}

void WTIMER0A_Handler(void)
{
	static GptNotification gptNotificationFn_WTIMER0A;
	if (locGptNotification[GPT_32_64_BIT_WIDE_TIMER0] != NULL)
	{
		gptNotificationFn_WTIMER0A = locGptNotification[GPT_32_64_BIT_WIDE_TIMER0];
		gptNotificationFn_WTIMER0A();
	}
}

void WTIMER1A_Handler(void)
{
	static GptNotification gptNotificationFn_WTIMER1A;
	if (locGptNotification[GPT_32_64_BIT_WIDE_TIMER1] != NULL)
	{
		gptNotificationFn_WTIMER1A = locGptNotification[GPT_32_64_BIT_WIDE_TIMER1];
		gptNotificationFn_WTIMER1A();
	}
}

void WTIMER2A_Handler(void)
{
	static GptNotification gptNotificationFn_WTIMER2A;
	if (locGptNotification[GPT_32_64_BIT_WIDE_TIMER2] != NULL)
	{
		gptNotificationFn_WTIMER2A = locGptNotification[GPT_32_64_BIT_WIDE_TIMER2];
		gptNotificationFn_WTIMER2A();
	}
}

void WTIMER3A_Handler(void)
{
	static GptNotification gptNotificationFn_WTIMER3A;
	if (locGptNotification[GPT_32_64_BIT_WIDE_TIMER3] != NULL)
	{
		gptNotificationFn_WTIMER3A = locGptNotification[GPT_32_64_BIT_WIDE_TIMER3];
		gptNotificationFn_WTIMER3A();
	}
}

void WTIMER4A_Handler(void)
{
	static GptNotification gptNotificationFn_WTIMER4A;
	if (locGptNotification[GPT_32_64_BIT_WIDE_TIMER4] != NULL)
	{
		gptNotificationFn_WTIMER4A = locGptNotification[GPT_32_64_BIT_WIDE_TIMER4];
		gptNotificationFn_WTIMER4A();
	}
}

void WTIMER5A_Handler(void)
{
	static GptNotification gptNotificationFn_WTIMER5A;
	if (locGptNotification[GPT_32_64_BIT_WIDE_TIMER5] != NULL)
	{
		gptNotificationFn_WTIMER5A = locGptNotification[GPT_32_64_BIT_WIDE_TIMER5];
		gptNotificationFn_WTIMER5A();
	}
}

/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
