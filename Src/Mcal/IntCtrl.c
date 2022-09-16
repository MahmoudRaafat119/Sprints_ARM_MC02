/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  IntCrtl.c
 *        \brief  Nested Vector Interrupt Controller Driver
 *
 *      \details  The Driver Configure All MCU interrupts Priority into gorups and subgroups
 *                Enable NVIC Interrupt Gate for Peripherals
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "IntCtrl.h"
#include "Mcu_Hw.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/	
#define NVIC_GROUPING_SYSTEM_XXX    4
#define NVIC_GROUPING_SYSTEM_XXY    5
#define NVIC_GROUPING_SYSTEM_XYY    6
#define NVIC_GROUPING_SYSTEM_YYY    7

#define APINT_VECTKEY 							0x05FA
#define APINT_VECTKEY_FIRLED_OFFSET             16u           /*16 bit*/
#define PRIGROUP_FIELED_OFFSET		         	0x8u
#define NVIC_REG_NUM_OF_PRI_FIELDS	            4u             /*4 bit*/


/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

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
* \Syntax          : Svoid IntCrtl_Init(void)        
* \Description     : NVIC initialization                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : void
*******************************************************************************/
void IntCrtl_Init(void)
{
	Nvic_IntType intNum;
	uint8  Group, SubGroup,i,GroupingField;
	uint32 EnableRegOffset, EnableBitOffset;
	uint32 PriorityRegOffset, PriorityBitOffset;
	
	/* configure grouping and subgrouping int APINT register in SCB */
	APINT = (APINT_VECTKEY<<APINT_VECTKEY_FIRLED_OFFSET)
					|(NVIC_GROUPING_SYSTEM << PRIGROUP_FIELED_OFFSET);
	
	//#define		SCB_AIRCR		*((volatile uint32 *)(0xE000ED00+0x0C))
	//SCB_AIRCR = NVIC_GROUPING_SYSTEM;
	
	for(i=0; i< NVIC_ACTIVATED_INT_SIZE ;i++)
	{
        intNum			= NVIC_Cfg[i].Interupt_Number;
		Group		    = NVIC_Cfg[i].Group_Priority;
		SubGroup        = NVIC_Cfg[i].SubGroup_Priority;
		
		/* enable\disable based on user configuration */
		EnableRegOffset = (intNum/WORD_LENGTH_BITS)*WORD_LENGTH_BYTES; 
		EnableBitOffset = intNum%WORD_LENGTH_BITS;
			
		GET_HWREG(NVIC_ENABLE_BASE_ADDRESS,EnableRegOffset) |= (1 << EnableBitOffset);


			#if NVIC_GROUPING_SYSTEM == NVIC_GROUPING_SYSTEM_XXX
						GroupingField = Group;
						
			#elif NVIC_GROUPING_SYSTEM == NVIC_GROUPING_SYSTEM_XXY
						GroupingField = ((Group<<1)&0x6) | (SubGroup&0x1);
						
			#elif NVIC_GROUPING_SYSTEM == NVIC_GROUPING_SYSTEM_XYY
				    	GroupingField = ((Group<<2)&0x4) | (SubGroup&0x3);
						
			#elif NVIC_GROUPING_SYSTEM == NVIC_GROUPING_SYSTEM_YYY
					    GroupingField = Group;
			#else
				#error INVALID GROUPING SELECTION			
			#endif
			
						/* asign group\subgroup priority */
		
			PriorityRegOffset = (intNum/NVIC_REG_NUM_OF_PRI_FIELDS)*WORD_LENGTH_BYTES;
			PriorityBitOffset = 5 + (8 * (intNum%NVIC_REG_NUM_OF_PRI_FIELDS));
			GET_HWREG(NVIC_PRI_BASE_ADDRESS,PriorityRegOffset) |= (GroupingField << PriorityBitOffset);

	}	
}



/**********************************************************************************************************************
 *  END OF FILE: IntCrtl.c
 *********************************************************************************************************************/
