/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Timer.h
 *       Module:  -
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Platform_Types.h"
#include "Std_Types.h"
#include "Timer_Cfg.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum
{
	GPT_16_32_BIT_TIMER0,
	GPT_16_32_BIT_TIMER1,
	GPT_16_32_BIT_TIMER2,
	GPT_16_32_BIT_TIMER3,
	GPT_16_32_BIT_TIMER4,
	GPT_16_32_BIT_TIMER5,
	GPT_32_64_BIT_WIDE_TIMER0,
	GPT_32_64_BIT_WIDE_TIMER1,
	GPT_32_64_BIT_WIDE_TIMER2,
	GPT_32_64_BIT_WIDE_TIMER3,
	GPT_32_64_BIT_WIDE_TIMER4,
	GPT_32_64_BIT_WIDE_TIMER5
}Gpt_ChannelType; 

typedef uint32 Gpt_ValueType;

typedef enum
{
	GPT_MODE_NORMAL,
	GPT_MODE_SLEEP 
}Gpt_ModeType;

typedef enum
{
	GPT_PREDEF_TIMER_1US_16BIT,
	GPT_PREDEF_TIMER_1US_24BIT,
	GPT_PREDEF_TIMER_1US_32BIT,
	GPT_PREDEF_TIMER_100US_32BIT
}Gpt_PredefTimerType;

typedef uint32 Gpt_ChannelTickFrequency;
typedef uint32 GptChannelTickValueMax;

typedef enum
{
	GPT_CH_MODE_CONTINUOUS,
	GPT_CH_MODE_ONESHOT 
}ChannelMode;

typedef void(*GptNotification)(void);

typedef struct
{
	Gpt_ChannelType  			channel;
	Gpt_ChannelTickFrequency    channelTickFreq;
	GptChannelTickValueMax  	channelTickMaxValue;
	ChannelMode					channelMode;
	GptNotification				gptNotification;
}Gpt_ConfigType;

#define PREDEF_TIMERS_INT_SIZE   4

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
extern const Gpt_ConfigType Gpt_Config[GPT_ACTIVATED_CHANNELS_SIZE];
 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
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
void Gpt_Init( const Gpt_ConfigType* ConfigPtr);
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
void Gpt_DisableNotification( Gpt_ChannelType Channel );
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
void Gpt_EnableNotification( Gpt_ChannelType Channel );
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
Gpt_ValueType Gpt_GetTimeElapsed( Gpt_ChannelType Channel );

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
Std_ReturnType Gpt_GetPredefTimerValue( Gpt_PredefTimerType PredefTimer, uint32* TimeValuePtr);
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
Gpt_ValueType Gpt_GetTimeRemaining( Gpt_ChannelType Channel );
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
void Gpt_StartTimer( Gpt_ChannelType Channel, Gpt_ValueType Value );

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
void Gpt_StopTimer( Gpt_ChannelType Channel );
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
void Gpt_Notification_Channel ( void);


/******************************************************************************
 * \Syntax          : void GPT_Attach(GPT_Notification CallBack,GPT_ValueType Value)
 * \Description     : This Function is used to make user only attach the callback function + Value of time in milliseconds 
 * 						if he wants to work with only one timer in his application
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : CallBack >> Callback Function that will be excuted when Timer interrupt is activated
 * 					  ValueinMs	>> Value of Desired time to pass in milliseconds
 * \Parameters (out): None
 * \Return value:   : None
 *******************************************************************************/
void GPT_Attach(GptNotification Copy_GPTCallBack,Gpt_ValueType Copy_GPTValueinMs);
 
#endif  /* TIMER_H */

/**********************************************************************************************************************
 *  END OF FILE: Timer.h
 *********************************************************************************************************************/
