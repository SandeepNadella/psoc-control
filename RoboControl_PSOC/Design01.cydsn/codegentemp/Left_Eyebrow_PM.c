/*******************************************************************************
* File Name: Left_Eyebrow_PM.c
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

#include "Left_Eyebrow.h"

static Left_Eyebrow_backupStruct Left_Eyebrow_backup;


/*******************************************************************************
* Function Name: Left_Eyebrow_SaveConfig
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
*  Left_Eyebrow_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Left_Eyebrow_SaveConfig(void) 
{

    #if(!Left_Eyebrow_UsingFixedFunction)
        #if(!Left_Eyebrow_PWMModeIsCenterAligned)
            Left_Eyebrow_backup.PWMPeriod = Left_Eyebrow_ReadPeriod();
        #endif /* (!Left_Eyebrow_PWMModeIsCenterAligned) */
        Left_Eyebrow_backup.PWMUdb = Left_Eyebrow_ReadCounter();
        #if (Left_Eyebrow_UseStatus)
            Left_Eyebrow_backup.InterruptMaskValue = Left_Eyebrow_STATUS_MASK;
        #endif /* (Left_Eyebrow_UseStatus) */

        #if(Left_Eyebrow_DeadBandMode == Left_Eyebrow__B_PWM__DBM_256_CLOCKS || \
            Left_Eyebrow_DeadBandMode == Left_Eyebrow__B_PWM__DBM_2_4_CLOCKS)
            Left_Eyebrow_backup.PWMdeadBandValue = Left_Eyebrow_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Left_Eyebrow_KillModeMinTime)
             Left_Eyebrow_backup.PWMKillCounterPeriod = Left_Eyebrow_ReadKillTime();
        #endif /* (Left_Eyebrow_KillModeMinTime) */

        #if(Left_Eyebrow_UseControl)
            Left_Eyebrow_backup.PWMControlRegister = Left_Eyebrow_ReadControlRegister();
        #endif /* (Left_Eyebrow_UseControl) */
    #endif  /* (!Left_Eyebrow_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Left_Eyebrow_RestoreConfig
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
*  Left_Eyebrow_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Left_Eyebrow_RestoreConfig(void) 
{
        #if(!Left_Eyebrow_UsingFixedFunction)
            #if(!Left_Eyebrow_PWMModeIsCenterAligned)
                Left_Eyebrow_WritePeriod(Left_Eyebrow_backup.PWMPeriod);
            #endif /* (!Left_Eyebrow_PWMModeIsCenterAligned) */

            Left_Eyebrow_WriteCounter(Left_Eyebrow_backup.PWMUdb);

            #if (Left_Eyebrow_UseStatus)
                Left_Eyebrow_STATUS_MASK = Left_Eyebrow_backup.InterruptMaskValue;
            #endif /* (Left_Eyebrow_UseStatus) */

            #if(Left_Eyebrow_DeadBandMode == Left_Eyebrow__B_PWM__DBM_256_CLOCKS || \
                Left_Eyebrow_DeadBandMode == Left_Eyebrow__B_PWM__DBM_2_4_CLOCKS)
                Left_Eyebrow_WriteDeadTime(Left_Eyebrow_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Left_Eyebrow_KillModeMinTime)
                Left_Eyebrow_WriteKillTime(Left_Eyebrow_backup.PWMKillCounterPeriod);
            #endif /* (Left_Eyebrow_KillModeMinTime) */

            #if(Left_Eyebrow_UseControl)
                Left_Eyebrow_WriteControlRegister(Left_Eyebrow_backup.PWMControlRegister);
            #endif /* (Left_Eyebrow_UseControl) */
        #endif  /* (!Left_Eyebrow_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Left_Eyebrow_Sleep
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
*  Left_Eyebrow_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Left_Eyebrow_Sleep(void) 
{
    #if(Left_Eyebrow_UseControl)
        if(Left_Eyebrow_CTRL_ENABLE == (Left_Eyebrow_CONTROL & Left_Eyebrow_CTRL_ENABLE))
        {
            /*Component is enabled */
            Left_Eyebrow_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Left_Eyebrow_backup.PWMEnableState = 0u;
        }
    #endif /* (Left_Eyebrow_UseControl) */

    /* Stop component */
    Left_Eyebrow_Stop();

    /* Save registers configuration */
    Left_Eyebrow_SaveConfig();
}


/*******************************************************************************
* Function Name: Left_Eyebrow_Wakeup
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
*  Left_Eyebrow_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Left_Eyebrow_Wakeup(void) 
{
     /* Restore registers values */
    Left_Eyebrow_RestoreConfig();

    if(Left_Eyebrow_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Left_Eyebrow_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
