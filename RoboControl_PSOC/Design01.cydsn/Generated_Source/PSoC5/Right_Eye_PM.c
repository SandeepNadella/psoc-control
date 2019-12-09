/*******************************************************************************
* File Name: Right_Eye_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Right_Eye.h"

static Right_Eye_backupStruct Right_Eye_backup;


/*******************************************************************************
* Function Name: Right_Eye_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Right_Eye_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Right_Eye_SaveConfig(void) 
{

    #if(!Right_Eye_UsingFixedFunction)
        #if(!Right_Eye_PWMModeIsCenterAligned)
            Right_Eye_backup.PWMPeriod = Right_Eye_ReadPeriod();
        #endif /* (!Right_Eye_PWMModeIsCenterAligned) */
        Right_Eye_backup.PWMUdb = Right_Eye_ReadCounter();
        #if (Right_Eye_UseStatus)
            Right_Eye_backup.InterruptMaskValue = Right_Eye_STATUS_MASK;
        #endif /* (Right_Eye_UseStatus) */

        #if(Right_Eye_DeadBandMode == Right_Eye__B_PWM__DBM_256_CLOCKS || \
            Right_Eye_DeadBandMode == Right_Eye__B_PWM__DBM_2_4_CLOCKS)
            Right_Eye_backup.PWMdeadBandValue = Right_Eye_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Right_Eye_KillModeMinTime)
             Right_Eye_backup.PWMKillCounterPeriod = Right_Eye_ReadKillTime();
        #endif /* (Right_Eye_KillModeMinTime) */

        #if(Right_Eye_UseControl)
            Right_Eye_backup.PWMControlRegister = Right_Eye_ReadControlRegister();
        #endif /* (Right_Eye_UseControl) */
    #endif  /* (!Right_Eye_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Right_Eye_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Right_Eye_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Right_Eye_RestoreConfig(void) 
{
        #if(!Right_Eye_UsingFixedFunction)
            #if(!Right_Eye_PWMModeIsCenterAligned)
                Right_Eye_WritePeriod(Right_Eye_backup.PWMPeriod);
            #endif /* (!Right_Eye_PWMModeIsCenterAligned) */

            Right_Eye_WriteCounter(Right_Eye_backup.PWMUdb);

            #if (Right_Eye_UseStatus)
                Right_Eye_STATUS_MASK = Right_Eye_backup.InterruptMaskValue;
            #endif /* (Right_Eye_UseStatus) */

            #if(Right_Eye_DeadBandMode == Right_Eye__B_PWM__DBM_256_CLOCKS || \
                Right_Eye_DeadBandMode == Right_Eye__B_PWM__DBM_2_4_CLOCKS)
                Right_Eye_WriteDeadTime(Right_Eye_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Right_Eye_KillModeMinTime)
                Right_Eye_WriteKillTime(Right_Eye_backup.PWMKillCounterPeriod);
            #endif /* (Right_Eye_KillModeMinTime) */

            #if(Right_Eye_UseControl)
                Right_Eye_WriteControlRegister(Right_Eye_backup.PWMControlRegister);
            #endif /* (Right_Eye_UseControl) */
        #endif  /* (!Right_Eye_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Right_Eye_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Right_Eye_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Right_Eye_Sleep(void) 
{
    #if(Right_Eye_UseControl)
        if(Right_Eye_CTRL_ENABLE == (Right_Eye_CONTROL & Right_Eye_CTRL_ENABLE))
        {
            /*Component is enabled */
            Right_Eye_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Right_Eye_backup.PWMEnableState = 0u;
        }
    #endif /* (Right_Eye_UseControl) */

    /* Stop component */
    Right_Eye_Stop();

    /* Save registers configuration */
    Right_Eye_SaveConfig();
}


/*******************************************************************************
* Function Name: Right_Eye_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Right_Eye_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Right_Eye_Wakeup(void) 
{
     /* Restore registers values */
    Right_Eye_RestoreConfig();

    if(Right_Eye_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Right_Eye_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
