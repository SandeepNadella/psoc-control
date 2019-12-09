/*******************************************************************************
* File Name: Left_Eyebrow.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Left_Eyebrow.h"

/* Error message for removed <resource> through optimization */
#ifdef Left_Eyebrow_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* Left_Eyebrow_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 Left_Eyebrow_initVar = 0u;


/*******************************************************************************
* Function Name: Left_Eyebrow_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Left_Eyebrow_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Left_Eyebrow_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(Left_Eyebrow_initVar == 0u)
    {
        Left_Eyebrow_Init();
        Left_Eyebrow_initVar = 1u;
    }
    Left_Eyebrow_Enable();

}


/*******************************************************************************
* Function Name: Left_Eyebrow_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  Left_Eyebrow_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Left_Eyebrow_Init(void) 
{
    #if (Left_Eyebrow_UsingFixedFunction || Left_Eyebrow_UseControl)
        uint8 ctrl;
    #endif /* (Left_Eyebrow_UsingFixedFunction || Left_Eyebrow_UseControl) */

    #if(!Left_Eyebrow_UsingFixedFunction)
        #if(Left_Eyebrow_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 Left_Eyebrow_interruptState;
        #endif /* (Left_Eyebrow_UseStatus) */
    #endif /* (!Left_Eyebrow_UsingFixedFunction) */

    #if (Left_Eyebrow_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        Left_Eyebrow_CONTROL |= Left_Eyebrow_CFG0_MODE;
        #if (Left_Eyebrow_DeadBand2_4)
            Left_Eyebrow_CONTROL |= Left_Eyebrow_CFG0_DB;
        #endif /* (Left_Eyebrow_DeadBand2_4) */

        ctrl = Left_Eyebrow_CONTROL3 & ((uint8 )(~Left_Eyebrow_CTRL_CMPMODE1_MASK));
        Left_Eyebrow_CONTROL3 = ctrl | Left_Eyebrow_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        Left_Eyebrow_RT1 &= ((uint8)(~Left_Eyebrow_RT1_MASK));
        Left_Eyebrow_RT1 |= Left_Eyebrow_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        Left_Eyebrow_RT1 &= ((uint8)(~Left_Eyebrow_SYNCDSI_MASK));
        Left_Eyebrow_RT1 |= Left_Eyebrow_SYNCDSI_EN;

    #elif (Left_Eyebrow_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = Left_Eyebrow_CONTROL & ((uint8)(~Left_Eyebrow_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~Left_Eyebrow_CTRL_CMPMODE1_MASK));
        Left_Eyebrow_CONTROL = ctrl | Left_Eyebrow_DEFAULT_COMPARE2_MODE |
                                   Left_Eyebrow_DEFAULT_COMPARE1_MODE;
    #endif /* (Left_Eyebrow_UsingFixedFunction) */

    #if (!Left_Eyebrow_UsingFixedFunction)
        #if (Left_Eyebrow_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            Left_Eyebrow_AUX_CONTROLDP0 |= (Left_Eyebrow_AUX_CTRL_FIFO0_CLR);
        #else /* (Left_Eyebrow_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            Left_Eyebrow_AUX_CONTROLDP0 |= (Left_Eyebrow_AUX_CTRL_FIFO0_CLR);
            Left_Eyebrow_AUX_CONTROLDP1 |= (Left_Eyebrow_AUX_CTRL_FIFO0_CLR);
        #endif /* (Left_Eyebrow_Resolution == 8) */

        Left_Eyebrow_WriteCounter(Left_Eyebrow_INIT_PERIOD_VALUE);
    #endif /* (!Left_Eyebrow_UsingFixedFunction) */

    Left_Eyebrow_WritePeriod(Left_Eyebrow_INIT_PERIOD_VALUE);

        #if (Left_Eyebrow_UseOneCompareMode)
            Left_Eyebrow_WriteCompare(Left_Eyebrow_INIT_COMPARE_VALUE1);
        #else
            Left_Eyebrow_WriteCompare1(Left_Eyebrow_INIT_COMPARE_VALUE1);
            Left_Eyebrow_WriteCompare2(Left_Eyebrow_INIT_COMPARE_VALUE2);
        #endif /* (Left_Eyebrow_UseOneCompareMode) */

        #if (Left_Eyebrow_KillModeMinTime)
            Left_Eyebrow_WriteKillTime(Left_Eyebrow_MinimumKillTime);
        #endif /* (Left_Eyebrow_KillModeMinTime) */

        #if (Left_Eyebrow_DeadBandUsed)
            Left_Eyebrow_WriteDeadTime(Left_Eyebrow_INIT_DEAD_TIME);
        #endif /* (Left_Eyebrow_DeadBandUsed) */

    #if (Left_Eyebrow_UseStatus || Left_Eyebrow_UsingFixedFunction)
        Left_Eyebrow_SetInterruptMode(Left_Eyebrow_INIT_INTERRUPTS_MODE);
    #endif /* (Left_Eyebrow_UseStatus || Left_Eyebrow_UsingFixedFunction) */

    #if (Left_Eyebrow_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        Left_Eyebrow_GLOBAL_ENABLE |= Left_Eyebrow_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        Left_Eyebrow_CONTROL2 |= Left_Eyebrow_CTRL2_IRQ_SEL;
    #else
        #if(Left_Eyebrow_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Left_Eyebrow_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Left_Eyebrow_STATUS_AUX_CTRL |= Left_Eyebrow_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(Left_Eyebrow_interruptState);

            /* Clear the FIFO to enable the Left_Eyebrow_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            Left_Eyebrow_ClearFIFO();
        #endif /* (Left_Eyebrow_UseStatus) */
    #endif /* (Left_Eyebrow_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Left_Eyebrow_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void Left_Eyebrow_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Left_Eyebrow_UsingFixedFunction)
        Left_Eyebrow_GLOBAL_ENABLE |= Left_Eyebrow_BLOCK_EN_MASK;
        Left_Eyebrow_GLOBAL_STBY_ENABLE |= Left_Eyebrow_BLOCK_STBY_EN_MASK;
    #endif /* (Left_Eyebrow_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (Left_Eyebrow_UseControl || Left_Eyebrow_UsingFixedFunction)
        Left_Eyebrow_CONTROL |= Left_Eyebrow_CTRL_ENABLE;
    #endif /* (Left_Eyebrow_UseControl || Left_Eyebrow_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Left_Eyebrow_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void Left_Eyebrow_Stop(void) 
{
    #if (Left_Eyebrow_UseControl || Left_Eyebrow_UsingFixedFunction)
        Left_Eyebrow_CONTROL &= ((uint8)(~Left_Eyebrow_CTRL_ENABLE));
    #endif /* (Left_Eyebrow_UseControl || Left_Eyebrow_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (Left_Eyebrow_UsingFixedFunction)
        Left_Eyebrow_GLOBAL_ENABLE &= ((uint8)(~Left_Eyebrow_BLOCK_EN_MASK));
        Left_Eyebrow_GLOBAL_STBY_ENABLE &= ((uint8)(~Left_Eyebrow_BLOCK_STBY_EN_MASK));
    #endif /* (Left_Eyebrow_UsingFixedFunction) */
}

#if (Left_Eyebrow_UseOneCompareMode)
    #if (Left_Eyebrow_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Left_Eyebrow_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Left_Eyebrow_SetCompareMode(uint8 comparemode) 
        {
            #if(Left_Eyebrow_UsingFixedFunction)

                #if(0 != Left_Eyebrow_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << Left_Eyebrow_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != Left_Eyebrow_CTRL_CMPMODE1_SHIFT) */

                Left_Eyebrow_CONTROL3 &= ((uint8)(~Left_Eyebrow_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                Left_Eyebrow_CONTROL3 |= comparemodemasked;

            #elif (Left_Eyebrow_UseControl)

                #if(0 != Left_Eyebrow_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << Left_Eyebrow_CTRL_CMPMODE1_SHIFT)) &
                                                Left_Eyebrow_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & Left_Eyebrow_CTRL_CMPMODE1_MASK;
                #endif /* (0 != Left_Eyebrow_CTRL_CMPMODE1_SHIFT) */

                #if(0 != Left_Eyebrow_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << Left_Eyebrow_CTRL_CMPMODE2_SHIFT)) &
                                               Left_Eyebrow_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & Left_Eyebrow_CTRL_CMPMODE2_MASK;
                #endif /* (0 != Left_Eyebrow_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                Left_Eyebrow_CONTROL &= ((uint8)(~(Left_Eyebrow_CTRL_CMPMODE1_MASK |
                                            Left_Eyebrow_CTRL_CMPMODE2_MASK)));
                Left_Eyebrow_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (Left_Eyebrow_UsingFixedFunction) */
        }
    #endif /* Left_Eyebrow_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (Left_Eyebrow_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Left_Eyebrow_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Left_Eyebrow_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != Left_Eyebrow_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << Left_Eyebrow_CTRL_CMPMODE1_SHIFT)) &
                                           Left_Eyebrow_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & Left_Eyebrow_CTRL_CMPMODE1_MASK;
            #endif /* (0 != Left_Eyebrow_CTRL_CMPMODE1_SHIFT) */

            #if (Left_Eyebrow_UseControl)
                Left_Eyebrow_CONTROL &= ((uint8)(~Left_Eyebrow_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                Left_Eyebrow_CONTROL |= comparemodemasked;
            #endif /* (Left_Eyebrow_UseControl) */
        }
    #endif /* Left_Eyebrow_CompareMode1SW */

#if (Left_Eyebrow_CompareMode2SW)


    /*******************************************************************************
    * Function Name: Left_Eyebrow_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Left_Eyebrow_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != Left_Eyebrow_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << Left_Eyebrow_CTRL_CMPMODE2_SHIFT)) &
                                                 Left_Eyebrow_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & Left_Eyebrow_CTRL_CMPMODE2_MASK;
        #endif /* (0 != Left_Eyebrow_CTRL_CMPMODE2_SHIFT) */

        #if (Left_Eyebrow_UseControl)
            Left_Eyebrow_CONTROL &= ((uint8)(~Left_Eyebrow_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            Left_Eyebrow_CONTROL |= comparemodemasked;
        #endif /* (Left_Eyebrow_UseControl) */
    }
    #endif /*Left_Eyebrow_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!Left_Eyebrow_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Left_Eyebrow_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void Left_Eyebrow_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(Left_Eyebrow_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: Left_Eyebrow_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint16 Left_Eyebrow_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(Left_Eyebrow_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(Left_Eyebrow_CAPTURE_LSB_PTR));
    }

    #if (Left_Eyebrow_UseStatus)


        /*******************************************************************************
        * Function Name: Left_Eyebrow_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Left_Eyebrow_ClearFIFO(void) 
        {
            while(0u != (Left_Eyebrow_ReadStatusRegister() & Left_Eyebrow_STATUS_FIFONEMPTY))
            {
                (void)Left_Eyebrow_ReadCapture();
            }
        }

    #endif /* Left_Eyebrow_UseStatus */

#endif /* !Left_Eyebrow_UsingFixedFunction */


/*******************************************************************************
* Function Name: Left_Eyebrow_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void Left_Eyebrow_WritePeriod(uint16 period) 
{
    #if(Left_Eyebrow_UsingFixedFunction)
        CY_SET_REG16(Left_Eyebrow_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(Left_Eyebrow_PERIOD_LSB_PTR, period);
    #endif /* (Left_Eyebrow_UsingFixedFunction) */
}

#if (Left_Eyebrow_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Left_Eyebrow_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void Left_Eyebrow_WriteCompare(uint16 compare) \
                                       
    {
        #if(Left_Eyebrow_UsingFixedFunction)
            CY_SET_REG16(Left_Eyebrow_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(Left_Eyebrow_COMPARE1_LSB_PTR, compare);
        #endif /* (Left_Eyebrow_UsingFixedFunction) */

        #if (Left_Eyebrow_PWMMode == Left_Eyebrow__B_PWM__DITHER)
            #if(Left_Eyebrow_UsingFixedFunction)
                CY_SET_REG16(Left_Eyebrow_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(Left_Eyebrow_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (Left_Eyebrow_UsingFixedFunction) */
        #endif /* (Left_Eyebrow_PWMMode == Left_Eyebrow__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: Left_Eyebrow_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Left_Eyebrow_WriteCompare1(uint16 compare) \
                                        
    {
        #if(Left_Eyebrow_UsingFixedFunction)
            CY_SET_REG16(Left_Eyebrow_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(Left_Eyebrow_COMPARE1_LSB_PTR, compare);
        #endif /* (Left_Eyebrow_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: Left_Eyebrow_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Left_Eyebrow_WriteCompare2(uint16 compare) \
                                        
    {
        #if(Left_Eyebrow_UsingFixedFunction)
            CY_SET_REG16(Left_Eyebrow_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(Left_Eyebrow_COMPARE2_LSB_PTR, compare);
        #endif /* (Left_Eyebrow_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (Left_Eyebrow_DeadBandUsed)


    /*******************************************************************************
    * Function Name: Left_Eyebrow_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Left_Eyebrow_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!Left_Eyebrow_DeadBand2_4)
            CY_SET_REG8(Left_Eyebrow_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            Left_Eyebrow_DEADBAND_COUNT &= ((uint8)(~Left_Eyebrow_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(Left_Eyebrow_DEADBAND_COUNT_SHIFT)
                Left_Eyebrow_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << Left_Eyebrow_DEADBAND_COUNT_SHIFT)) &
                                                    Left_Eyebrow_DEADBAND_COUNT_MASK;
            #else
                Left_Eyebrow_DEADBAND_COUNT |= deadtime & Left_Eyebrow_DEADBAND_COUNT_MASK;
            #endif /* (Left_Eyebrow_DEADBAND_COUNT_SHIFT) */

        #endif /* (!Left_Eyebrow_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: Left_Eyebrow_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 Left_Eyebrow_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!Left_Eyebrow_DeadBand2_4)
            return (CY_GET_REG8(Left_Eyebrow_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(Left_Eyebrow_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(Left_Eyebrow_DEADBAND_COUNT & Left_Eyebrow_DEADBAND_COUNT_MASK)) >>
                                                                           Left_Eyebrow_DEADBAND_COUNT_SHIFT));
            #else
                return (Left_Eyebrow_DEADBAND_COUNT & Left_Eyebrow_DEADBAND_COUNT_MASK);
            #endif /* (Left_Eyebrow_DEADBAND_COUNT_SHIFT) */
        #endif /* (!Left_Eyebrow_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (Left_Eyebrow_UseStatus || Left_Eyebrow_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Left_Eyebrow_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Left_Eyebrow_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(Left_Eyebrow_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: Left_Eyebrow_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 Left_Eyebrow_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(Left_Eyebrow_STATUS_PTR));
    }

#endif /* (Left_Eyebrow_UseStatus || Left_Eyebrow_UsingFixedFunction) */


#if (Left_Eyebrow_UseControl)


    /*******************************************************************************
    * Function Name: Left_Eyebrow_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 Left_Eyebrow_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(Left_Eyebrow_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: Left_Eyebrow_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Left_Eyebrow_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(Left_Eyebrow_CONTROL_PTR, control);
    }

#endif /* (Left_Eyebrow_UseControl) */


#if (!Left_Eyebrow_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Left_Eyebrow_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint16 Left_Eyebrow_ReadCapture(void) 
    {
        return (CY_GET_REG16(Left_Eyebrow_CAPTURE_LSB_PTR));
    }

#endif /* (!Left_Eyebrow_UsingFixedFunction) */


#if (Left_Eyebrow_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Left_Eyebrow_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint16 Left_Eyebrow_ReadCompare(void) 
    {
        #if(Left_Eyebrow_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(Left_Eyebrow_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(Left_Eyebrow_COMPARE1_LSB_PTR));
        #endif /* (Left_Eyebrow_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: Left_Eyebrow_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 Left_Eyebrow_ReadCompare1(void) 
    {
        return (CY_GET_REG16(Left_Eyebrow_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: Left_Eyebrow_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 Left_Eyebrow_ReadCompare2(void) 
    {
        return (CY_GET_REG16(Left_Eyebrow_COMPARE2_LSB_PTR));
    }

#endif /* (Left_Eyebrow_UseOneCompareMode) */


/*******************************************************************************
* Function Name: Left_Eyebrow_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint16 Left_Eyebrow_ReadPeriod(void) 
{
    #if(Left_Eyebrow_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(Left_Eyebrow_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(Left_Eyebrow_PERIOD_LSB_PTR));
    #endif /* (Left_Eyebrow_UsingFixedFunction) */
}

#if ( Left_Eyebrow_KillModeMinTime)


    /*******************************************************************************
    * Function Name: Left_Eyebrow_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Left_Eyebrow_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(Left_Eyebrow_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: Left_Eyebrow_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 Left_Eyebrow_ReadKillTime(void) 
    {
        return (CY_GET_REG8(Left_Eyebrow_KILLMODEMINTIME_PTR));
    }

#endif /* ( Left_Eyebrow_KillModeMinTime) */

/* [] END OF FILE */
