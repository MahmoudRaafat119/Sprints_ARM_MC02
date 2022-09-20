/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Mcu_Lcfg.c
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
#include "Mcu.h"
#include "Mcu_Cfg.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/* Configuartion file */
#define MCU_CLOCK_GATE_GPIOA     20u
#define MCU_CLOCK_GATE_GPIOB     21u
#define MCU_CLOCK_GATE_GPIOC     22u
#define MCU_CLOCK_GATE_GPIOD     23u
#define MCU_CLOCK_GATE_GPIOE     24u
#define MCU_CLOCK_GATE_GPIOF     25u

#define MCU_CLOCK_GATE_16_32_BIT_GPT0       10u
#define MCU_CLOCK_GATE_16_32_BIT_GPT1       11u
#define MCU_CLOCK_GATE_16_32_BIT_GPT2       12u
#define MCU_CLOCK_GATE_16_32_BIT_GPT3       13u
#define MCU_CLOCK_GATE_16_32_BIT_GPT4       14u
#define MCU_CLOCK_GATE_16_32_BIT_GPT5       15u

#define MCU_CLOCK_GATE_32_64_BIT_GPT0       150u
#define MCU_CLOCK_GATE_32_64_BIT_GPT1       151u
#define MCU_CLOCK_GATE_32_64_BIT_GPT2       152u
#define MCU_CLOCK_GATE_32_64_BIT_GPT3       153u
#define MCU_CLOCK_GATE_32_64_BIT_GPT4       154u
#define MCU_CLOCK_GATE_32_64_BIT_GPT5       155u


#define MCU_CLOCK_GATE_WD0       00u
#define MCU_CLOCK_GATE_WD1       01u

#define Mcu_Clock_0        0
#define Mcu_Clock_1        1
#define Mcu_Clock_2        2


uint8 Mcu_ActivatedClockGates[MCU_ACTIVATED_CLOCK_GATES_SIZE]=
{
	MCU_CLOCK_GATE_GPIOA,
	MCU_CLOCK_GATE_GPIOB,
	MCU_CLOCK_GATE_GPIOC,
	MCU_CLOCK_GATE_GPIOD,
	MCU_CLOCK_GATE_GPIOE,
	MCU_CLOCK_GATE_GPIOF,
	MCU_CLOCK_GATE_16_32_BIT_GPT0,
	MCU_CLOCK_GATE_32_64_BIT_GPT1,
	MCU_CLOCK_GATE_32_64_BIT_GPT4,
	MCU_CLOCK_GATE_WD0
};

const Mcu_ConfigType McuConfig[MCU_CLOCK_SETTINGS_SIZE]=
{
	/*ClockSettingID*/ /* ClockSource                        PllUse     Freq_KHz  */
	/* 0            */ {  MCU_CLOCK_SOURCE_MOSC             ,ENABLE     ,25000},
	/* 1            */ {  MCU_CLOCK_SOURCE_PIOSC            ,DISABLE    ,16000},
};

/**********************************************************************************************************************
 *  END OF FILE: Mcu_Lcfg.c
 *********************************************************************************************************************/
