/*******************************************************************************
* File Name: Jaw_PM.c
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

#include "Jaw.h"

static Jaw_backupStruct Jaw_backup;


/*******************************************************************************
* Function Name: Jaw_SaveConfig
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
*  Jaw_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Jaw_SaveConfig(void) 
{

    #if(!Jaw_UsingFixedFunction)
        #if(!Jaw_PWMModeIsCenterAligned)
            Jaw_backup.PWMPeriod = Jaw_ReadPeriod();
        #endif /* (!Jaw_PWMModeIsCenterAligned) */
        Jaw_backup.PWMUdb = Jaw_ReadCounter();
        #if (Jaw_UseStatus)
            Jaw_backup.InterruptMaskValue = Jaw_STATUS_MASK;
        #endif /* (Jaw_UseStatus) */

        #if(Jaw_DeadBandMode == Jaw__B_PWM__DBM_256_CLOCKS || \
            Jaw_DeadBandMode == Jaw__B_PWM__DBM_2_4_CLOCKS)
            Jaw_backup.PWMdeadBandValue = Jaw_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Jaw_KillModeMinTime)
             Jaw_backup.PWMKillCounterPeriod = Jaw_ReadKillTime();
        #endif /* (Jaw_KillModeMinTime) */

        #if(Jaw_UseControl)
            Jaw_backup.PWMControlRegister = Jaw_ReadControlRegister();
        #endif /* (Jaw_UseControl) */
    #endif  /* (!Jaw_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Jaw_RestoreConfig
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
*  Jaw_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Jaw_RestoreConfig(void) 
{
        #if(!Jaw_UsingFixedFunction)
            #if(!Jaw_PWMModeIsCenterAligned)
                Jaw_WritePeriod(Jaw_backup.PWMPeriod);
            #endif /* (!Jaw_PWMModeIsCenterAligned) */

            Jaw_WriteCounter(Jaw_backup.PWMUdb);

            #if (Jaw_UseStatus)
                Jaw_STATUS_MASK = Jaw_backup.InterruptMaskValue;
            #endif /* (Jaw_UseStatus) */

            #if(Jaw_DeadBandMode == Jaw__B_PWM__DBM_256_CLOCKS || \
                Jaw_DeadBandMode == Jaw__B_PWM__DBM_2_4_CLOCKS)
                Jaw_WriteDeadTime(Jaw_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Jaw_KillModeMinTime)
                Jaw_WriteKillTime(Jaw_backup.PWMKillCounterPeriod);
            #endif /* (Jaw_KillModeMinTime) */

            #if(Jaw_UseControl)
                Jaw_WriteControlRegister(Jaw_backup.PWMControlRegister);
            #endif /* (Jaw_UseControl) */
        #endif  /* (!Jaw_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Jaw_Sleep
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
*  Jaw_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Jaw_Sleep(void) 
{
    #if(Jaw_UseControl)
        if(Jaw_CTRL_ENABLE == (Jaw_CONTROL & Jaw_CTRL_ENABLE))
        {
            /*Component is enabled */
            Jaw_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Jaw_backup.PWMEnableState = 0u;
        }
    #endif /* (Jaw_UseControl) */

    /* Stop component */
    Jaw_Stop();

    /* Save registers configuration */
    Jaw_SaveConfig();
}


/*******************************************************************************
* Function Name: Jaw_Wakeup
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
*  Jaw_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Jaw_Wakeup(void) 
{
     /* Restore registers values */
    Jaw_RestoreConfig();

    if(Jaw_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Jaw_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
