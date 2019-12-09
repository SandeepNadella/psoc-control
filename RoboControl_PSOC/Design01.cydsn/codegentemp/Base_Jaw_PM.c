/*******************************************************************************
* File Name: Base_Jaw_PM.c
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

#include "Base_Jaw.h"

static Base_Jaw_backupStruct Base_Jaw_backup;


/*******************************************************************************
* Function Name: Base_Jaw_SaveConfig
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
*  Base_Jaw_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Base_Jaw_SaveConfig(void) 
{

    #if(!Base_Jaw_UsingFixedFunction)
        #if(!Base_Jaw_PWMModeIsCenterAligned)
            Base_Jaw_backup.PWMPeriod = Base_Jaw_ReadPeriod();
        #endif /* (!Base_Jaw_PWMModeIsCenterAligned) */
        Base_Jaw_backup.PWMUdb = Base_Jaw_ReadCounter();
        #if (Base_Jaw_UseStatus)
            Base_Jaw_backup.InterruptMaskValue = Base_Jaw_STATUS_MASK;
        #endif /* (Base_Jaw_UseStatus) */

        #if(Base_Jaw_DeadBandMode == Base_Jaw__B_PWM__DBM_256_CLOCKS || \
            Base_Jaw_DeadBandMode == Base_Jaw__B_PWM__DBM_2_4_CLOCKS)
            Base_Jaw_backup.PWMdeadBandValue = Base_Jaw_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Base_Jaw_KillModeMinTime)
             Base_Jaw_backup.PWMKillCounterPeriod = Base_Jaw_ReadKillTime();
        #endif /* (Base_Jaw_KillModeMinTime) */

        #if(Base_Jaw_UseControl)
            Base_Jaw_backup.PWMControlRegister = Base_Jaw_ReadControlRegister();
        #endif /* (Base_Jaw_UseControl) */
    #endif  /* (!Base_Jaw_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Base_Jaw_RestoreConfig
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
*  Base_Jaw_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Base_Jaw_RestoreConfig(void) 
{
        #if(!Base_Jaw_UsingFixedFunction)
            #if(!Base_Jaw_PWMModeIsCenterAligned)
                Base_Jaw_WritePeriod(Base_Jaw_backup.PWMPeriod);
            #endif /* (!Base_Jaw_PWMModeIsCenterAligned) */

            Base_Jaw_WriteCounter(Base_Jaw_backup.PWMUdb);

            #if (Base_Jaw_UseStatus)
                Base_Jaw_STATUS_MASK = Base_Jaw_backup.InterruptMaskValue;
            #endif /* (Base_Jaw_UseStatus) */

            #if(Base_Jaw_DeadBandMode == Base_Jaw__B_PWM__DBM_256_CLOCKS || \
                Base_Jaw_DeadBandMode == Base_Jaw__B_PWM__DBM_2_4_CLOCKS)
                Base_Jaw_WriteDeadTime(Base_Jaw_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Base_Jaw_KillModeMinTime)
                Base_Jaw_WriteKillTime(Base_Jaw_backup.PWMKillCounterPeriod);
            #endif /* (Base_Jaw_KillModeMinTime) */

            #if(Base_Jaw_UseControl)
                Base_Jaw_WriteControlRegister(Base_Jaw_backup.PWMControlRegister);
            #endif /* (Base_Jaw_UseControl) */
        #endif  /* (!Base_Jaw_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Base_Jaw_Sleep
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
*  Base_Jaw_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Base_Jaw_Sleep(void) 
{
    #if(Base_Jaw_UseControl)
        if(Base_Jaw_CTRL_ENABLE == (Base_Jaw_CONTROL & Base_Jaw_CTRL_ENABLE))
        {
            /*Component is enabled */
            Base_Jaw_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Base_Jaw_backup.PWMEnableState = 0u;
        }
    #endif /* (Base_Jaw_UseControl) */

    /* Stop component */
    Base_Jaw_Stop();

    /* Save registers configuration */
    Base_Jaw_SaveConfig();
}


/*******************************************************************************
* Function Name: Base_Jaw_Wakeup
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
*  Base_Jaw_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Base_Jaw_Wakeup(void) 
{
     /* Restore registers values */
    Base_Jaw_RestoreConfig();

    if(Base_Jaw_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Base_Jaw_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
