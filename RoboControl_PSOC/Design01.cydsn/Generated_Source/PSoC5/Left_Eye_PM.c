/*******************************************************************************
* File Name: Left_Eye_PM.c
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

#include "Left_Eye.h"

static Left_Eye_backupStruct Left_Eye_backup;


/*******************************************************************************
* Function Name: Left_Eye_SaveConfig
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
*  Left_Eye_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Left_Eye_SaveConfig(void) 
{

    #if(!Left_Eye_UsingFixedFunction)
        #if(!Left_Eye_PWMModeIsCenterAligned)
            Left_Eye_backup.PWMPeriod = Left_Eye_ReadPeriod();
        #endif /* (!Left_Eye_PWMModeIsCenterAligned) */
        Left_Eye_backup.PWMUdb = Left_Eye_ReadCounter();
        #if (Left_Eye_UseStatus)
            Left_Eye_backup.InterruptMaskValue = Left_Eye_STATUS_MASK;
        #endif /* (Left_Eye_UseStatus) */

        #if(Left_Eye_DeadBandMode == Left_Eye__B_PWM__DBM_256_CLOCKS || \
            Left_Eye_DeadBandMode == Left_Eye__B_PWM__DBM_2_4_CLOCKS)
            Left_Eye_backup.PWMdeadBandValue = Left_Eye_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Left_Eye_KillModeMinTime)
             Left_Eye_backup.PWMKillCounterPeriod = Left_Eye_ReadKillTime();
        #endif /* (Left_Eye_KillModeMinTime) */

        #if(Left_Eye_UseControl)
            Left_Eye_backup.PWMControlRegister = Left_Eye_ReadControlRegister();
        #endif /* (Left_Eye_UseControl) */
    #endif  /* (!Left_Eye_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Left_Eye_RestoreConfig
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
*  Left_Eye_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Left_Eye_RestoreConfig(void) 
{
        #if(!Left_Eye_UsingFixedFunction)
            #if(!Left_Eye_PWMModeIsCenterAligned)
                Left_Eye_WritePeriod(Left_Eye_backup.PWMPeriod);
            #endif /* (!Left_Eye_PWMModeIsCenterAligned) */

            Left_Eye_WriteCounter(Left_Eye_backup.PWMUdb);

            #if (Left_Eye_UseStatus)
                Left_Eye_STATUS_MASK = Left_Eye_backup.InterruptMaskValue;
            #endif /* (Left_Eye_UseStatus) */

            #if(Left_Eye_DeadBandMode == Left_Eye__B_PWM__DBM_256_CLOCKS || \
                Left_Eye_DeadBandMode == Left_Eye__B_PWM__DBM_2_4_CLOCKS)
                Left_Eye_WriteDeadTime(Left_Eye_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Left_Eye_KillModeMinTime)
                Left_Eye_WriteKillTime(Left_Eye_backup.PWMKillCounterPeriod);
            #endif /* (Left_Eye_KillModeMinTime) */

            #if(Left_Eye_UseControl)
                Left_Eye_WriteControlRegister(Left_Eye_backup.PWMControlRegister);
            #endif /* (Left_Eye_UseControl) */
        #endif  /* (!Left_Eye_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Left_Eye_Sleep
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
*  Left_Eye_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Left_Eye_Sleep(void) 
{
    #if(Left_Eye_UseControl)
        if(Left_Eye_CTRL_ENABLE == (Left_Eye_CONTROL & Left_Eye_CTRL_ENABLE))
        {
            /*Component is enabled */
            Left_Eye_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Left_Eye_backup.PWMEnableState = 0u;
        }
    #endif /* (Left_Eye_UseControl) */

    /* Stop component */
    Left_Eye_Stop();

    /* Save registers configuration */
    Left_Eye_SaveConfig();
}


/*******************************************************************************
* Function Name: Left_Eye_Wakeup
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
*  Left_Eye_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Left_Eye_Wakeup(void) 
{
     /* Restore registers values */
    Left_Eye_RestoreConfig();

    if(Left_Eye_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Left_Eye_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
