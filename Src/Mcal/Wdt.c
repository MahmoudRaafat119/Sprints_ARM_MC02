/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Wdt.c
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
#include "platform_Types.h" 
#include "Wdt.h"
#include "Mcu_hw.h"
#include "Wdt_Cfg.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/
#define WRC         31
#define RESEN		1
#define INTEN		0
#define INTTYPE     2

#define WDG_UNLOCK_VALUE			0x1ACCE551
/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
volatile WdgMaxTimeout		locMaxTimeout;
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
extern volatile uint32 GlobalSystemClock;
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
* \Syntax          : void Wdg_Init ( const Wdg_ConfigType* ConfigPtr )        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : void  
*******************************************************************************/
void Wdg_Init ( const Wdg_ConfigType* ConfigPtr )
{
	WdgNotificationEnable		locNotificationEn;
	WdgInterruptType				locInterruptType;
	WdgInitialTimeout				locIntialTimeout;
	
	
	locNotificationEn	=	ConfigPtr->notificationEn;
	locInterruptType	=	ConfigPtr->interruptType;
	locIntialTimeout	=	ConfigPtr->intialTimeout;
	locMaxTimeout			=	ConfigPtr->maxTimeout;
	
	locIntialTimeout *= GlobalSystemClock;
	
	WDTLOAD(WDT_BASE_ADDRESS) = locIntialTimeout;

	while ((WDTCTL(WDT_1_BASE_ADDRESS)>>WRC) == 0);

	WDTCTL(WDT_BASE_ADDRESS) |= (1<<RESEN);

	while ((WDTCTL(WDT_1_BASE_ADDRESS)>>WRC) == 0);

	WDTCTL(WDT_BASE_ADDRESS) |= (1<<INTTYPE);
	while ((WDTCTL(WDT_1_BASE_ADDRESS)>>WRC) == 0);
	
	WDTCTL(WDT_BASE_ADDRESS) |= (1<<INTEN);

	WDTLOCK(WDT_BASE_ADDRESS) = 1;

}

/******************************************************************************
* \Syntax          : void Wdg_SetTriggerCondition ( uint16 timeout )        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) :                        
* \Parameters (out): None                                                      
* \Return value:   : void  
*******************************************************************************/
void Wdg_SetTriggerCondition ( uint16 timeout )
{
	if(timeout > locMaxTimeout)
	{
		/*Report to Det*/
	}
	else
	{
		WDTLOCK(WDT_BASE_ADDRESS) = 0x1ACCE551;

		timeout *= GlobalSystemClock;
		WDTLOAD(WDT_BASE_ADDRESS) = timeout;
		
		WDTLOCK(WDT_BASE_ADDRESS) = 1;
	}
}

/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
