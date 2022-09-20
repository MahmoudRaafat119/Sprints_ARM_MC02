/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Port.c
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
#include "Port.h"
#include "Port_Cfg.h"
#include "Mcu_Hw.h"
/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/


#define CHANNEL_SIZE_IN_PORT               8u
#define MAX_NUMBER_OF_GPIO_PORTS           6u

#define GPIO_COMMIT_UNLOCKING_VALUE				 0x4C4F434B
#define PIN_FIELD_SIZE_IN_GPIOPCTL				 4
/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
static const uint32 Port_BaseAddr[MAX_NUMBER_OF_GPIO_PORTS] = {GPIO_APB_BASE_ADDRESS_A,
	                                                      GPIO_APB_BASE_ADDRESS_B,
	                                                      GPIO_APB_BASE_ADDRESS_C,
	                                                      GPIO_APB_BASE_ADDRESS_D,
	                                                      GPIO_APB_BASE_ADDRESS_E,
	                                                      GPIO_APB_BASE_ADDRESS_F};
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

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
* \Syntax          : void Port_Init ( const Port_ConfigType* ConfigPtr )        
* \Description     : Initializes the Port Driver module.                                   
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : ConfigPtr       Pointer to configuration set.                     
* \Parameters (out): None                                                      
* \Return value:   : None  
*                                                                      
*******************************************************************************/
void Port_Init ( const Port_ConfigType* ConfigPtr )
{
	Port_PinType								locPinId;
	Port_PinDirectionType				locPinDir;
	Port_PinModeType						locPinMode; 
	Port_PinOutputCurrentType		locCurrentStrength;
	Port_PinInternalAttachType	locInternalAttach;
	Port_PinExternalIntType			locExternalInt;
	uint8 PortIndex,PinIndex,i;
	uint32 PortBaseAddr;
	
for(i=0; i< PORT_ACTIVATED_CHANNELS_SIZE; i++)
{
		locPinId					 = ConfigPtr[i].pinId;
		locPinDir					 = ConfigPtr[i].pinDir;
		locPinMode				 = ConfigPtr[i].pinMode;
		locCurrentStrength = ConfigPtr[i].currentStrength;
		locInternalAttach	 = ConfigPtr[i].internalAttach;
		locExternalInt		 = ConfigPtr[i].externalInt;
		
		PortIndex = locPinId / CHANNEL_SIZE_IN_PORT;
		PinIndex  = locPinId % CHANNEL_SIZE_IN_PORT;
		
		PortBaseAddr = Port_BaseAddr[PortIndex] ;
		
		GPIODIR(PortBaseAddr) |= (locPinDir << PinIndex);
		/* setting pin mode */
		if(locPinMode == Port_PinMode_X_DIO)
		{
			GPIOLOCK(PortBaseAddr) = GPIO_COMMIT_UNLOCKING_VALUE;
			GPIOAFSEL(PortBaseAddr) = 0;
		}
		else
		{
			GPIOLOCK(PortBaseAddr)	= GPIO_COMMIT_UNLOCKING_VALUE;
			GPIOCR(PortBaseAddr) 		= 0x1;
			GPIOPCTL(PortBaseAddr) |= (locPinMode << (PIN_FIELD_SIZE_IN_GPIOPCTL * PinIndex));
		}
		
		/* setting current strength */
		if(locCurrentStrength == PORT_PIN_CURRENT_2m)
		{
			GPIODR2R(PortBaseAddr) |= (1 << PinIndex);
		}
		else if(locCurrentStrength == PORT_PIN_CURRENT_4m)
		{
			GPIODR4R(PortBaseAddr) |= (1 << PinIndex);
		}
		else if(locCurrentStrength == PORT_PIN_CURRENT_8m)
		{
			GPIODR8R(PortBaseAddr) |= (1 << PinIndex);
		}
		
		/* setting internal attach type */
		if (locInternalAttach == PORT_ATTACH_PULLUP)
		{
			GPIOLOCK(PortBaseAddr)	= GPIO_COMMIT_UNLOCKING_VALUE;
			GPIOCR(PortBaseAddr) 		= 0x1;
			
			GPIOPUR(PortBaseAddr) |= (1 << PinIndex);
			
		} 
		else if (locInternalAttach == PORT_ATTACH_PULLDOWN)
		{
			GPIOLOCK(PortBaseAddr)	= GPIO_COMMIT_UNLOCKING_VALUE;
			GPIOCR(PortBaseAddr) 		= 0x1;
			
			GPIOPDR(PortBaseAddr) |= (1 << PinIndex);
		}
		else if (locInternalAttach == PORT_ATTACH_OPENDRAIN)
		{
			GPIOLOCK(PortBaseAddr)	= GPIO_COMMIT_UNLOCKING_VALUE;
			GPIOCR(PortBaseAddr) 		= 0x1;
			
			GPIOODR(PortBaseAddr) |= (1 << PinIndex);
		}
		
		
		/* setting external interupt mode */
		if (locExternalInt == PORT_EXT_INT_DISABLE)
		{
			GPIOIM(PortBaseAddr) &= (~(1 << PinIndex));
		}
		/* configure the interupt with the edges */
		else if (locExternalInt == PORT_EXT_INT_RISING)
		{
			GPIOIM(PortBaseAddr) 	&= (~(1 << PinIndex));
			GPIOIS(PortBaseAddr) 	|= (1 << PinIndex);
			GPIOIBE(PortBaseAddr)	&= (~(1 << PinIndex));
			GPIOIEV(PortBaseAddr)	|= (1 << PinIndex);
			GPIORIS(PortBaseAddr)	&= (~(1 << PinIndex));
			GPIOIM(PortBaseAddr) 	|= (1 << PinIndex);
			
		}
		else if (locExternalInt == PORT_EXT_INT_FALLING)
		{
			GPIOIM(PortBaseAddr)	&= (~(1 << PinIndex));
			GPIOIS(PortBaseAddr)	|= (1 << PinIndex);
			GPIOIBE(PortBaseAddr)	&= (~(1 << PinIndex));
			GPIOIEV(PortBaseAddr) 	&= (~(1 << PinIndex));
			GPIORIS(PortBaseAddr) 	&= (~(1 << PinIndex));
			GPIOIM(PortBaseAddr)	|= (1 << PinIndex);
		}
		else if (locExternalInt == PORT_EXT_INT_BOTH)
		{
			GPIOIM(PortBaseAddr) 	&= (~(1 << PinIndex));
			GPIOIS(PortBaseAddr) 	|= (1 << PinIndex);
			GPIOIBE(PortBaseAddr)	|= (1 << PinIndex);
			GPIORIS(PortBaseAddr) 	&= (~(1 << PinIndex));
			GPIOIM(PortBaseAddr) 	|= (1 << PinIndex);
		}
	}
}
