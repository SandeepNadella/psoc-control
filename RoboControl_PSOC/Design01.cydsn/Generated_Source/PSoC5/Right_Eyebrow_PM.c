/*******************************************************************************
* File Name: Right_Eyebrow_PM.c
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

#include "Right_Eyebrow.h"

static Right_Eyebrow_backupStruct Right_Eyebrow_backup;


/*******************************************************************************
* Function Name: Right_Eyebrow_SaveConfig
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
*  Right_Eyebrow_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Right_Eyebrow_SaveConfig(void) 
{

    #if(!Right_Eyebrow_UsingFixedFunction)
        #if(!Right_Eyebrow_PWMModeIsCenterAligned)
            Right_Eyebrow_backup.PWMPeriod = Right_Eyebrow_ReadPeriod();
        #endif /* (!Right_Eyebrow_PWMModeIsCenterAligned) */
        Right_Eyebrow_backup.PWMUdb = Right_Eyebrow_ReadCounter();
        #if (Right_Eyebrow_UseStatus)
            Right_Eyebrow_backup.InterruptMaskValue = Right_Eyebrow_STATUS_MASK;
        #endif /* (Right_Eyebrow_UseStatus) */

        #if(Right_Eyebrow_DeadBandMode == Right_Eyebrow__B_PWM__DBM_256_CLOCKS || \
            Right_Eyebrow_DeadBandMode == Right_Eyebrow__B_PWM__DBM_2_4_CLOCKS)
            Right_Eyebrow_backup.PWMdeadBandValue = Right_Eyebrow_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Right_Eyebrow_KillModeMinTime)
             Right_Eyebrow_backup.PWMKillCounterPeriod = Right_Eyebrow_ReadKillTime();
        #endif /* (Right_Eyebrow_KillModeMinTime) */

        #if(Right_Eyebrow_UseControl)
            Right_Eyebrow_backup.PWMControlRegister = Right_Eyebrow_ReadControlRegister();
        #endif /* (Right_Eyebrow_UseControl) */
    #endif  /* (!Right_Eyebrow_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Right_Eyebrow_RestoreConfig
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
*  Right_Eyebrow_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Right_Eyebrow_RestoreConfig(void) 
{
        #if(!Right_Eyebrow_UsingFixedFunction)
            #if(!Right_Eyebrow_PWMModeIsCenterAligned)
                Right_Eyebrow_WritePeriod(Right_Eyebrow_backup.PWMPeriod);
            #endif /* (!Right_Eyebrow_PWMModeIsCenterAligned) */

            Right_Eyebrow_WriteCounter(Right_Eyebrow_backup.PWMUdb);

            #if (Right_Eyebrow_UseStatus)
                Right_Eyebrow_STATUS_MASK = Right_Eyebrow_backup.InterruptMaskValue;
            #endif /* (Right_Eyebrow_UseStatus) */

            #if(Right_Eyebrow_DeadBandMode == Right_Eyebrow__B_PWM__DBM_256_CLOCKS || \
                Right_Eyebrow_DeadBandMode == Right_Eyebrow__B_PWM__DBM_2_4_CLOCKS)
                Right_Eyebrow_WriteDeadTime(Right_Eyebrow_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Right_Eyebrow_KillModeMinTime)
                Right_Eyebrow_WriteKillTime(Right_Eyebrow_backup.PWMKillCounterPeriod);
            #endif /* (Right_Eyebrow_KillModeMinTime) */

            #if(Right_Eyebrow_UseControl)
                Right_Eyebrow_WriteControlRegister(Right_Eyebrow_backup.PWMControlRegister);
            #endif /* (Right_Eyebrow_UseControl) */
        #endif  /* (!Right_Eyebrow_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Right_Eyebrow_Sleep
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
*  Right_Eyebrow_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Right_Eyebrow_Sleep(void) 
{
    #if(Right_Eyebrow_UseControl)
        if(Right_Eyebrow_CTRL_ENABLE == (Right_Eyebrow_CONTROL & Right_Eyebrow_CTRL_ENABLE))
        {
            /*Component is enabled */
            Right_Eyebrow_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Right_Eyebrow_backup.PWMEnableState = 0u;
        }
    #endif /* (Right_Eyebrow_UseControl) */

    /* Stop component */
    Right_Eyebrow_Stop();

    /* Save registers configuration */
    Right_Eyebrow_SaveConfig();
}


/*******************************************************************************
* Function Name: Right_Eyebrow_Wakeup
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
*  Right_Eyebrow_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Right_Eyebrow_Wakeup(void) 
{
     /* Restore registers values */
    Right_Eyebrow_RestoreConfig();

    if(Right_Eyebrow_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Right_Eyebrow_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
