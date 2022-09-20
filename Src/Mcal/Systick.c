/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Systick.c
 *        \brief
 *
 *      \details
 *
 *
 *********************************************************************************************************************/
#ifndef SYSTICK_C
#define SYSTICK_C

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Systick.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
 * \Syntax          : void Systick_Enable(void);
 * \Description     : Enables Systick Timer and enables systick interrupts
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 *******************************************************************************/
void Systick_Enable(void){
    SYSTICK_CURRENT_REG=0;
    /*Enables EN bit to start the counter
     *Set INTEN bit to trigger interrupt when finished
     *Set CLK_SRC bit to get the clock source from system clock
     *Priority is set in IntCtrl driver */
    
    SYSTICK_CTRL_REG|=(7);
}
/******************************************************************************
 * \Syntax          : void Systick_SetPeriod(uint32 seconds);
 * \Description     : Sets period between interrupts
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 *******************************************************************************/
void Systick_SetPeriod(uint32 milliSeconds){
    SYSTICK_RELOAD_REG=(16000*milliSeconds)-1;
}
/******************************************************************************
 * \Syntax          : void Systick_Off(void);
 * \Description     : disable systick timer
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 *******************************************************************************/
void Systick_Off(void){
    SYSTICK_CTRL_REG=0;
    SYSTICK_CURRENT_REG=0;
    SYSTICK_RELOAD_REG=0;
}
/**********************************************************************************************************************
 *  END OF FILE: IntCtrl.c
 *********************************************************************************************************************/
#endif