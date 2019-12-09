/*******************************************************************************
* File Name: Left_Eye.c
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

#include "Left_Eye.h"

/* Error message for removed <resource> through optimization */
#ifdef Left_Eye_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* Left_Eye_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 Left_Eye_initVar = 0u;


/*******************************************************************************
* Function Name: Left_Eye_Start
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
*  Left_Eye_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Left_Eye_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(Left_Eye_initVar == 0u)
    {
        Left_Eye_Init();
        Left_Eye_initVar = 1u;
    }
    Left_Eye_Enable();

}


/*******************************************************************************
* Function Name: Left_Eye_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  Left_Eye_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Left_Eye_Init(void) 
{
    #if (Left_Eye_UsingFixedFunction || Left_Eye_UseControl)
        uint8 ctrl;
    #endif /* (Left_Eye_UsingFixedFunction || Left_Eye_UseControl) */

    #if(!Left_Eye_UsingFixedFunction)
        #if(Left_Eye_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 Left_Eye_interruptState;
        #endif /* (Left_Eye_UseStatus) */
    #endif /* (!Left_Eye_UsingFixedFunction) */

    #if (Left_Eye_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        Left_Eye_CONTROL |= Left_Eye_CFG0_MODE;
        #if (Left_Eye_DeadBand2_4)
            Left_Eye_CONTROL |= Left_Eye_CFG0_DB;
        #endif /* (Left_Eye_DeadBand2_4) */

        ctrl = Left_Eye_CONTROL3 & ((uint8 )(~Left_Eye_CTRL_CMPMODE1_MASK));
        Left_Eye_CONTROL3 = ctrl | Left_Eye_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        Left_Eye_RT1 &= ((uint8)(~Left_Eye_RT1_MASK));
        Left_Eye_RT1 |= Left_Eye_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        Left_Eye_RT1 &= ((uint8)(~Left_Eye_SYNCDSI_MASK));
        Left_Eye_RT1 |= Left_Eye_SYNCDSI_EN;

    #elif (Left_Eye_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = Left_Eye_CONTROL & ((uint8)(~Left_Eye_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~Left_Eye_CTRL_CMPMODE1_MASK));
        Left_Eye_CONTROL = ctrl | Left_Eye_DEFAULT_COMPARE2_MODE |
                                   Left_Eye_DEFAULT_COMPARE1_MODE;
    #endif /* (Left_Eye_UsingFixedFunction) */

    #if (!Left_Eye_UsingFixedFunction)
        #if (Left_Eye_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            Left_Eye_AUX_CONTROLDP0 |= (Left_Eye_AUX_CTRL_FIFO0_CLR);
        #else /* (Left_Eye_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            Left_Eye_AUX_CONTROLDP0 |= (Left_Eye_AUX_CTRL_FIFO0_CLR);
            Left_Eye_AUX_CONTROLDP1 |= (Left_Eye_AUX_CTRL_FIFO0_CLR);
        #endif /* (Left_Eye_Resolution == 8) */

        Left_Eye_WriteCounter(Left_Eye_INIT_PERIOD_VALUE);
    #endif /* (!Left_Eye_UsingFixedFunction) */

    Left_Eye_WritePeriod(Left_Eye_INIT_PERIOD_VALUE);

        #if (Left_Eye_UseOneCompareMode)
            Left_Eye_WriteCompare(Left_Eye_INIT_COMPARE_VALUE1);
        #else
            Left_Eye_WriteCompare1(Left_Eye_INIT_COMPARE_VALUE1);
            Left_Eye_WriteCompare2(Left_Eye_INIT_COMPARE_VALUE2);
        #endif /* (Left_Eye_UseOneCompareMode) */

        #if (Left_Eye_KillModeMinTime)
            Left_Eye_WriteKillTime(Left_Eye_MinimumKillTime);
        #endif /* (Left_Eye_KillModeMinTime) */

        #if (Left_Eye_DeadBandUsed)
            Left_Eye_WriteDeadTime(Left_Eye_INIT_DEAD_TIME);
        #endif /* (Left_Eye_DeadBandUsed) */

    #if (Left_Eye_UseStatus || Left_Eye_UsingFixedFunction)
        Left_Eye_SetInterruptMode(Left_Eye_INIT_INTERRUPTS_MODE);
    #endif /* (Left_Eye_UseStatus || Left_Eye_UsingFixedFunction) */

    #if (Left_Eye_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        Left_Eye_GLOBAL_ENABLE |= Left_Eye_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        Left_Eye_CONTROL2 |= Left_Eye_CTRL2_IRQ_SEL;
    #else
        #if(Left_Eye_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Left_Eye_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Left_Eye_STATUS_AUX_CTRL |= Left_Eye_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(Left_Eye_interruptState);

            /* Clear the FIFO to enable the Left_Eye_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            Left_Eye_ClearFIFO();
        #endif /* (Left_Eye_UseStatus) */
    #endif /* (Left_Eye_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Left_Eye_Enable
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
void Left_Eye_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Left_Eye_UsingFixedFunction)
        Left_Eye_GLOBAL_ENABLE |= Left_Eye_BLOCK_EN_MASK;
        Left_Eye_GLOBAL_STBY_ENABLE |= Left_Eye_BLOCK_STBY_EN_MASK;
    #endif /* (Left_Eye_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (Left_Eye_UseControl || Left_Eye_UsingFixedFunction)
        Left_Eye_CONTROL |= Left_Eye_CTRL_ENABLE;
    #endif /* (Left_Eye_UseControl || Left_Eye_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Left_Eye_Stop
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
void Left_Eye_Stop(void) 
{
    #if (Left_Eye_UseControl || Left_Eye_UsingFixedFunction)
        Left_Eye_CONTROL &= ((uint8)(~Left_Eye_CTRL_ENABLE));
    #endif /* (Left_Eye_UseControl || Left_Eye_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (Left_Eye_UsingFixedFunction)
        Left_Eye_GLOBAL_ENABLE &= ((uint8)(~Left_Eye_BLOCK_EN_MASK));
        Left_Eye_GLOBAL_STBY_ENABLE &= ((uint8)(~Left_Eye_BLOCK_STBY_EN_MASK));
    #endif /* (Left_Eye_UsingFixedFunction) */
}

#if (Left_Eye_UseOneCompareMode)
    #if (Left_Eye_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Left_Eye_SetCompareMode
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
        void Left_Eye_SetCompareMode(uint8 comparemode) 
        {
            #if(Left_Eye_UsingFixedFunction)

                #if(0 != Left_Eye_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << Left_Eye_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != Left_Eye_CTRL_CMPMODE1_SHIFT) */

                Left_Eye_CONTROL3 &= ((uint8)(~Left_Eye_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                Left_Eye_CONTROL3 |= comparemodemasked;

            #elif (Left_Eye_UseControl)

                #if(0 != Left_Eye_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << Left_Eye_CTRL_CMPMODE1_SHIFT)) &
                                                Left_Eye_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & Left_Eye_CTRL_CMPMODE1_MASK;
                #endif /* (0 != Left_Eye_CTRL_CMPMODE1_SHIFT) */

                #if(0 != Left_Eye_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << Left_Eye_CTRL_CMPMODE2_SHIFT)) &
                                               Left_Eye_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & Left_Eye_CTRL_CMPMODE2_MASK;
                #endif /* (0 != Left_Eye_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                Left_Eye_CONTROL &= ((uint8)(~(Left_Eye_CTRL_CMPMODE1_MASK |
                                            Left_Eye_CTRL_CMPMODE2_MASK)));
                Left_Eye_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (Left_Eye_UsingFixedFunction) */
        }
    #endif /* Left_Eye_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (Left_Eye_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Left_Eye_SetCompareMode1
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
        void Left_Eye_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != Left_Eye_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << Left_Eye_CTRL_CMPMODE1_SHIFT)) &
                                           Left_Eye_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & Left_Eye_CTRL_CMPMODE1_MASK;
            #endif /* (0 != Left_Eye_CTRL_CMPMODE1_SHIFT) */

            #if (Left_Eye_UseControl)
                Left_Eye_CONTROL &= ((uint8)(~Left_Eye_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                Left_Eye_CONTROL |= comparemodemasked;
            #endif /* (Left_Eye_UseControl) */
        }
    #endif /* Left_Eye_CompareMode1SW */

#if (Left_Eye_CompareMode2SW)


    /*******************************************************************************
    * Function Name: Left_Eye_SetCompareMode2
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
    void Left_Eye_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != Left_Eye_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << Left_Eye_CTRL_CMPMODE2_SHIFT)) &
                                                 Left_Eye_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & Left_Eye_CTRL_CMPMODE2_MASK;
        #endif /* (0 != Left_Eye_CTRL_CMPMODE2_SHIFT) */

        #if (Left_Eye_UseControl)
            Left_Eye_CONTROL &= ((uint8)(~Left_Eye_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            Left_Eye_CONTROL |= comparemodemasked;
        #endif /* (Left_Eye_UseControl) */
    }
    #endif /*Left_Eye_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!Left_Eye_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Left_Eye_WriteCounter
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
    void Left_Eye_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(Left_Eye_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: Left_Eye_ReadCounter
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
    uint16 Left_Eye_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(Left_Eye_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(Left_Eye_CAPTURE_LSB_PTR));
    }

    #if (Left_Eye_UseStatus)


        /*******************************************************************************
        * Function Name: Left_Eye_ClearFIFO
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
        void Left_Eye_ClearFIFO(void) 
        {
            while(0u != (Left_Eye_ReadStatusRegister() & Left_Eye_STATUS_FIFONEMPTY))
            {
                (void)Left_Eye_ReadCapture();
            }
        }

    #endif /* Left_Eye_UseStatus */

#endif /* !Left_Eye_UsingFixedFunction */


/*******************************************************************************
* Function Name: Left_Eye_WritePeriod
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
void Left_Eye_WritePeriod(uint16 period) 
{
    #if(Left_Eye_UsingFixedFunction)
        CY_SET_REG16(Left_Eye_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(Left_Eye_PERIOD_LSB_PTR, period);
    #endif /* (Left_Eye_UsingFixedFunction) */
}

#if (Left_Eye_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Left_Eye_WriteCompare
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
    void Left_Eye_WriteCompare(uint16 compare) \
                                       
    {
        #if(Left_Eye_UsingFixedFunction)
            CY_SET_REG16(Left_Eye_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(Left_Eye_COMPARE1_LSB_PTR, compare);
        #endif /* (Left_Eye_UsingFixedFunction) */

        #if (Left_Eye_PWMMode == Left_Eye__B_PWM__DITHER)
            #if(Left_Eye_UsingFixedFunction)
                CY_SET_REG16(Left_Eye_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(Left_Eye_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (Left_Eye_UsingFixedFunction) */
        #endif /* (Left_Eye_PWMMode == Left_Eye__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: Left_Eye_WriteCompare1
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
    void Left_Eye_WriteCompare1(uint16 compare) \
                                        
    {
        #if(Left_Eye_UsingFixedFunction)
            CY_SET_REG16(Left_Eye_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(Left_Eye_COMPARE1_LSB_PTR, compare);
        #endif /* (Left_Eye_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: Left_Eye_WriteCompare2
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
    void Left_Eye_WriteCompare2(uint16 compare) \
                                        
    {
        #if(Left_Eye_UsingFixedFunction)
            CY_SET_REG16(Left_Eye_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(Left_Eye_COMPARE2_LSB_PTR, compare);
        #endif /* (Left_Eye_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (Left_Eye_DeadBandUsed)


    /*******************************************************************************
    * Function Name: Left_Eye_WriteDeadTime
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
    void Left_Eye_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!Left_Eye_DeadBand2_4)
            CY_SET_REG8(Left_Eye_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            Left_Eye_DEADBAND_COUNT &= ((uint8)(~Left_Eye_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(Left_Eye_DEADBAND_COUNT_SHIFT)
                Left_Eye_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << Left_Eye_DEADBAND_COUNT_SHIFT)) &
                                                    Left_Eye_DEADBAND_COUNT_MASK;
            #else
                Left_Eye_DEADBAND_COUNT |= deadtime & Left_Eye_DEADBAND_COUNT_MASK;
            #endif /* (Left_Eye_DEADBAND_COUNT_SHIFT) */

        #endif /* (!Left_Eye_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: Left_Eye_ReadDeadTime
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
    uint8 Left_Eye_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!Left_Eye_DeadBand2_4)
            return (CY_GET_REG8(Left_Eye_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(Left_Eye_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(Left_Eye_DEADBAND_COUNT & Left_Eye_DEADBAND_COUNT_MASK)) >>
                                                                           Left_Eye_DEADBAND_COUNT_SHIFT));
            #else
                return (Left_Eye_DEADBAND_COUNT & Left_Eye_DEADBAND_COUNT_MASK);
            #endif /* (Left_Eye_DEADBAND_COUNT_SHIFT) */
        #endif /* (!Left_Eye_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (Left_Eye_UseStatus || Left_Eye_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Left_Eye_SetInterruptMode
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
    void Left_Eye_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(Left_Eye_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: Left_Eye_ReadStatusRegister
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
    uint8 Left_Eye_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(Left_Eye_STATUS_PTR));
    }

#endif /* (Left_Eye_UseStatus || Left_Eye_UsingFixedFunction) */


#if (Left_Eye_UseControl)


    /*******************************************************************************
    * Function Name: Left_Eye_ReadControlRegister
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
    uint8 Left_Eye_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(Left_Eye_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: Left_Eye_WriteControlRegister
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
    void Left_Eye_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(Left_Eye_CONTROL_PTR, control);
    }

#endif /* (Left_Eye_UseControl) */


#if (!Left_Eye_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Left_Eye_ReadCapture
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
    uint16 Left_Eye_ReadCapture(void) 
    {
        return (CY_GET_REG16(Left_Eye_CAPTURE_LSB_PTR));
    }

#endif /* (!Left_Eye_UsingFixedFunction) */


#if (Left_Eye_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Left_Eye_ReadCompare
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
    uint16 Left_Eye_ReadCompare(void) 
    {
        #if(Left_Eye_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(Left_Eye_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(Left_Eye_COMPARE1_LSB_PTR));
        #endif /* (Left_Eye_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: Left_Eye_ReadCompare1
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
    uint16 Left_Eye_ReadCompare1(void) 
    {
        return (CY_GET_REG16(Left_Eye_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: Left_Eye_ReadCompare2
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
    uint16 Left_Eye_ReadCompare2(void) 
    {
        return (CY_GET_REG16(Left_Eye_COMPARE2_LSB_PTR));
    }

#endif /* (Left_Eye_UseOneCompareMode) */


/*******************************************************************************
* Function Name: Left_Eye_ReadPeriod
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
uint16 Left_Eye_ReadPeriod(void) 
{
    #if(Left_Eye_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(Left_Eye_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(Left_Eye_PERIOD_LSB_PTR));
    #endif /* (Left_Eye_UsingFixedFunction) */
}

#if ( Left_Eye_KillModeMinTime)


    /*******************************************************************************
    * Function Name: Left_Eye_WriteKillTime
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
    void Left_Eye_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(Left_Eye_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: Left_Eye_ReadKillTime
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
    uint8 Left_Eye_ReadKillTime(void) 
    {
        return (CY_GET_REG8(Left_Eye_KILLMODEMINTIME_PTR));
    }

#endif /* ( Left_Eye_KillModeMinTime) */

/* [] END OF FILE */
