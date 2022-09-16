/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dio.c
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
#include "Dio.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/
#define CHANNEL_SIZE_IN_PORT                8u
#define MAX_NUMBER_OF_GPIO_PORTS						6u
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
* \Syntax          : Dio_LevelType Dio_ReadChannel ( Dio_ChannelType ChannelId )        
* \Description     : Reads channel value
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : ChannelId   ID of DIO channel                     
* \Parameters (out): Dio_LevelType                                                      
* \Return value:   : Dio_LevelType  STD_LOW,
																		 STD_HIGH                                  
*******************************************************************************/
Dio_LevelType Dio_ReadChannel ( Dio_ChannelType ChannelId )
{
	Dio_LevelType level;
	uint32 gpioDataOffset;
	uint32 portIndex,pinIndex,PortBaseAddr;
	
	portIndex = ChannelId / CHANNEL_SIZE_IN_PORT;
	pinIndex = ChannelId % CHANNEL_SIZE_IN_PORT;
	
	PortBaseAddr = Port_BaseAddr[portIndex];
	
	gpioDataOffset = 4 * (1<<pinIndex);
	level = (*((uint32*)(PortBaseAddr+gpioDataOffset))) >> pinIndex;
	
	return level;
}
/******************************************************************************
* \Syntax          : void Dio_WriteChannel ( Dio_ChannelType ChannelId, Dio_LevelType Level )        
* \Description     : Writes channel value
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : ChannelId   ID of DIO channel                     
* \Parameters (out): None                                                      
* \Return value:   : None                                  
*******************************************************************************/
void Dio_WriteChannel ( Dio_ChannelType ChannelId, Dio_LevelType Level )
{
	uint32 gpioDataOffset;
	uint32 portIndex,pinIndex,PortBaseAddr;
	
	portIndex = ChannelId / CHANNEL_SIZE_IN_PORT;
	pinIndex = ChannelId % CHANNEL_SIZE_IN_PORT;
	
	PortBaseAddr = Port_BaseAddr[portIndex];
	
	gpioDataOffset = 4 * (1<<pinIndex);
	*((uint32 volatile*)(PortBaseAddr+gpioDataOffset)) = (Level<<pinIndex);
	
}
/******************************************************************************
* \Syntax          : Dio_PortLevelType Dio_ReadPort ( Dio_PortType PortId )       
* \Description     : Reads Port value
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : PortId   ID of DIO port                    
* \Parameters (out): Dio_PortLevelType                                                      
* \Return value:   : Dio_PortLevelType   uint8                                  
*******************************************************************************/
Dio_PortLevelType Dio_ReadPort ( Dio_PortType PortId )
{
	Dio_PortLevelType portLevel;
	uint32 gpioDataOffset = 0X3FC;
	uint32 PortBaseAddr;
	PortBaseAddr = Port_BaseAddr[PortId];
	
	portLevel = *((uint32*)(PortBaseAddr+gpioDataOffset));
	
	return portLevel;
}
/******************************************************************************
* \Syntax          : void Dio_WritePort ( Dio_PortType PortId, Dio_PortLevelType Level );       
* \Description     : writes Port value
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : PortId   ID of DIO port
										 Level  	Value to be written                    
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void Dio_WritePort ( Dio_PortType PortId, Dio_PortLevelType Level )
{
	uint32 gpioDataOffset = 0x3FC;
	uint32 PortBaseAddr;
	
	PortBaseAddr = Port_BaseAddr[PortId];
	
	*((uint32*)(PortBaseAddr+gpioDataOffset)) = Level;
}
/******************************************************************************
* \Syntax          : void Dio_WritePort ( Dio_PortType PortId, Dio_PortLevelType Level );       
* \Description     : writes Port value
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : ChannelId   ID of DIO channel                   
* \Parameters (out): Dio_LevelType                                                      
* \Return value:   : Dio_LevelType	STD_LOW,
																		STD_HIGH                                 
*******************************************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
	Dio_LevelType levelType;
	if (Dio_ReadChannel(ChannelId) == STD_LOW)
	{
		Dio_WriteChannel(ChannelId,STD_HIGH);
		levelType = STD_HIGH;
	}
	else if (Dio_ReadChannel(ChannelId) == STD_HIGH)
	{
		Dio_WriteChannel(ChannelId,STD_LOW);
		levelType = STD_LOW;
	}
	
	return levelType;
}
/**********************************************************************************************************************
 *  END OF FILE: Dio.c
 *********************************************************************************************************************/