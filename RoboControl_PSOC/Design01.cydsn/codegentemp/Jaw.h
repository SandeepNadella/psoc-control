/*******************************************************************************
* File Name: Jaw.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_Jaw_H)
#define CY_PWM_Jaw_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Jaw_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define Jaw_Resolution                     (16u)
#define Jaw_UsingFixedFunction             (0u)
#define Jaw_DeadBandMode                   (0u)
#define Jaw_KillModeMinTime                (0u)
#define Jaw_KillMode                       (0u)
#define Jaw_PWMMode                        (1u)
#define Jaw_PWMModeIsCenterAligned         (0u)
#define Jaw_DeadBandUsed                   (0u)
#define Jaw_DeadBand2_4                    (0u)

#if !defined(Jaw_PWMUDB_genblk8_stsreg__REMOVED)
    #define Jaw_UseStatus                  (1u)
#else
    #define Jaw_UseStatus                  (0u)
#endif /* !defined(Jaw_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(Jaw_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define Jaw_UseControl                 (1u)
#else
    #define Jaw_UseControl                 (0u)
#endif /* !defined(Jaw_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define Jaw_UseOneCompareMode              (0u)
#define Jaw_MinimumKillTime                (1u)
#define Jaw_EnableMode                     (0u)

#define Jaw_CompareMode1SW                 (0u)
#define Jaw_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define Jaw__B_PWM__DISABLED 0
#define Jaw__B_PWM__ASYNCHRONOUS 1
#define Jaw__B_PWM__SINGLECYCLE 2
#define Jaw__B_PWM__LATCHED 3
#define Jaw__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define Jaw__B_PWM__DBMDISABLED 0
#define Jaw__B_PWM__DBM_2_4_CLOCKS 1
#define Jaw__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define Jaw__B_PWM__ONE_OUTPUT 0
#define Jaw__B_PWM__TWO_OUTPUTS 1
#define Jaw__B_PWM__DUAL_EDGE 2
#define Jaw__B_PWM__CENTER_ALIGN 3
#define Jaw__B_PWM__DITHER 5
#define Jaw__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define Jaw__B_PWM__LESS_THAN 1
#define Jaw__B_PWM__LESS_THAN_OR_EQUAL 2
#define Jaw__B_PWM__GREATER_THAN 3
#define Jaw__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define Jaw__B_PWM__EQUAL 0
#define Jaw__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!Jaw_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!Jaw_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!Jaw_PWMModeIsCenterAligned) */
        #if (Jaw_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (Jaw_UseStatus) */

        /* Backup for Deadband parameters */
        #if(Jaw_DeadBandMode == Jaw__B_PWM__DBM_256_CLOCKS || \
            Jaw_DeadBandMode == Jaw__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(Jaw_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (Jaw_KillModeMinTime) */

        /* Backup control register */
        #if(Jaw_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (Jaw_UseControl) */

    #endif /* (!Jaw_UsingFixedFunction) */

}Jaw_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    Jaw_Start(void) ;
void    Jaw_Stop(void) ;

#if (Jaw_UseStatus || Jaw_UsingFixedFunction)
    void  Jaw_SetInterruptMode(uint8 interruptMode) ;
    uint8 Jaw_ReadStatusRegister(void) ;
#endif /* (Jaw_UseStatus || Jaw_UsingFixedFunction) */

#define Jaw_GetInterruptSource() Jaw_ReadStatusRegister()

#if (Jaw_UseControl)
    uint8 Jaw_ReadControlRegister(void) ;
    void  Jaw_WriteControlRegister(uint8 control)
          ;
#endif /* (Jaw_UseControl) */

#if (Jaw_UseOneCompareMode)
   #if (Jaw_CompareMode1SW)
       void    Jaw_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (Jaw_CompareMode1SW) */
#else
    #if (Jaw_CompareMode1SW)
        void    Jaw_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (Jaw_CompareMode1SW) */
    #if (Jaw_CompareMode2SW)
        void    Jaw_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (Jaw_CompareMode2SW) */
#endif /* (Jaw_UseOneCompareMode) */

#if (!Jaw_UsingFixedFunction)
    uint16   Jaw_ReadCounter(void) ;
    uint16 Jaw_ReadCapture(void) ;

    #if (Jaw_UseStatus)
            void Jaw_ClearFIFO(void) ;
    #endif /* (Jaw_UseStatus) */

    void    Jaw_WriteCounter(uint16 counter)
            ;
#endif /* (!Jaw_UsingFixedFunction) */

void    Jaw_WritePeriod(uint16 period)
        ;
uint16 Jaw_ReadPeriod(void) ;

#if (Jaw_UseOneCompareMode)
    void    Jaw_WriteCompare(uint16 compare)
            ;
    uint16 Jaw_ReadCompare(void) ;
#else
    void    Jaw_WriteCompare1(uint16 compare)
            ;
    uint16 Jaw_ReadCompare1(void) ;
    void    Jaw_WriteCompare2(uint16 compare)
            ;
    uint16 Jaw_ReadCompare2(void) ;
#endif /* (Jaw_UseOneCompareMode) */


#if (Jaw_DeadBandUsed)
    void    Jaw_WriteDeadTime(uint8 deadtime) ;
    uint8   Jaw_ReadDeadTime(void) ;
#endif /* (Jaw_DeadBandUsed) */

#if ( Jaw_KillModeMinTime)
    void Jaw_WriteKillTime(uint8 killtime) ;
    uint8 Jaw_ReadKillTime(void) ;
#endif /* ( Jaw_KillModeMinTime) */

void Jaw_Init(void) ;
void Jaw_Enable(void) ;
void Jaw_Sleep(void) ;
void Jaw_Wakeup(void) ;
void Jaw_SaveConfig(void) ;
void Jaw_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define Jaw_INIT_PERIOD_VALUE          (7000u)
#define Jaw_INIT_COMPARE_VALUE1        (1000u)
#define Jaw_INIT_COMPARE_VALUE2        (1000u)
#define Jaw_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    Jaw_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Jaw_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Jaw_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Jaw_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define Jaw_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  Jaw_CTRL_CMPMODE2_SHIFT)
#define Jaw_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  Jaw_CTRL_CMPMODE1_SHIFT)
#define Jaw_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (Jaw_UsingFixedFunction)
   #define Jaw_PERIOD_LSB              (*(reg16 *) Jaw_PWMHW__PER0)
   #define Jaw_PERIOD_LSB_PTR          ( (reg16 *) Jaw_PWMHW__PER0)
   #define Jaw_COMPARE1_LSB            (*(reg16 *) Jaw_PWMHW__CNT_CMP0)
   #define Jaw_COMPARE1_LSB_PTR        ( (reg16 *) Jaw_PWMHW__CNT_CMP0)
   #define Jaw_COMPARE2_LSB            (0x00u)
   #define Jaw_COMPARE2_LSB_PTR        (0x00u)
   #define Jaw_COUNTER_LSB             (*(reg16 *) Jaw_PWMHW__CNT_CMP0)
   #define Jaw_COUNTER_LSB_PTR         ( (reg16 *) Jaw_PWMHW__CNT_CMP0)
   #define Jaw_CAPTURE_LSB             (*(reg16 *) Jaw_PWMHW__CAP0)
   #define Jaw_CAPTURE_LSB_PTR         ( (reg16 *) Jaw_PWMHW__CAP0)
   #define Jaw_RT1                     (*(reg8 *)  Jaw_PWMHW__RT1)
   #define Jaw_RT1_PTR                 ( (reg8 *)  Jaw_PWMHW__RT1)

#else
   #if (Jaw_Resolution == 8u) /* 8bit - PWM */

       #if(Jaw_PWMModeIsCenterAligned)
           #define Jaw_PERIOD_LSB      (*(reg8 *)  Jaw_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define Jaw_PERIOD_LSB_PTR  ((reg8 *)   Jaw_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define Jaw_PERIOD_LSB      (*(reg8 *)  Jaw_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define Jaw_PERIOD_LSB_PTR  ((reg8 *)   Jaw_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (Jaw_PWMModeIsCenterAligned) */

       #define Jaw_COMPARE1_LSB        (*(reg8 *)  Jaw_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define Jaw_COMPARE1_LSB_PTR    ((reg8 *)   Jaw_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define Jaw_COMPARE2_LSB        (*(reg8 *)  Jaw_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define Jaw_COMPARE2_LSB_PTR    ((reg8 *)   Jaw_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define Jaw_COUNTERCAP_LSB      (*(reg8 *)  Jaw_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define Jaw_COUNTERCAP_LSB_PTR  ((reg8 *)   Jaw_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define Jaw_COUNTER_LSB         (*(reg8 *)  Jaw_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define Jaw_COUNTER_LSB_PTR     ((reg8 *)   Jaw_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define Jaw_CAPTURE_LSB         (*(reg8 *)  Jaw_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define Jaw_CAPTURE_LSB_PTR     ((reg8 *)   Jaw_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(Jaw_PWMModeIsCenterAligned)
               #define Jaw_PERIOD_LSB      (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define Jaw_PERIOD_LSB_PTR  ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define Jaw_PERIOD_LSB      (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define Jaw_PERIOD_LSB_PTR  ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (Jaw_PWMModeIsCenterAligned) */

            #define Jaw_COMPARE1_LSB       (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define Jaw_COMPARE1_LSB_PTR   ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define Jaw_COMPARE2_LSB       (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define Jaw_COMPARE2_LSB_PTR   ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define Jaw_COUNTERCAP_LSB     (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define Jaw_COUNTERCAP_LSB_PTR ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define Jaw_COUNTER_LSB        (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define Jaw_COUNTER_LSB_PTR    ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define Jaw_CAPTURE_LSB        (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define Jaw_CAPTURE_LSB_PTR    ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(Jaw_PWMModeIsCenterAligned)
               #define Jaw_PERIOD_LSB      (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define Jaw_PERIOD_LSB_PTR  ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define Jaw_PERIOD_LSB      (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define Jaw_PERIOD_LSB_PTR  ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (Jaw_PWMModeIsCenterAligned) */

            #define Jaw_COMPARE1_LSB       (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define Jaw_COMPARE1_LSB_PTR   ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define Jaw_COMPARE2_LSB       (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define Jaw_COMPARE2_LSB_PTR   ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define Jaw_COUNTERCAP_LSB     (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define Jaw_COUNTERCAP_LSB_PTR ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define Jaw_COUNTER_LSB        (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define Jaw_COUNTER_LSB_PTR    ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define Jaw_CAPTURE_LSB        (*(reg16 *) Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define Jaw_CAPTURE_LSB_PTR    ((reg16 *)  Jaw_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define Jaw_AUX_CONTROLDP1          (*(reg8 *)  Jaw_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define Jaw_AUX_CONTROLDP1_PTR      ((reg8 *)   Jaw_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (Jaw_Resolution == 8) */

   #define Jaw_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  Jaw_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define Jaw_AUX_CONTROLDP0          (*(reg8 *)  Jaw_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define Jaw_AUX_CONTROLDP0_PTR      ((reg8 *)   Jaw_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (Jaw_UsingFixedFunction) */

#if(Jaw_KillModeMinTime )
    #define Jaw_KILLMODEMINTIME        (*(reg8 *)  Jaw_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define Jaw_KILLMODEMINTIME_PTR    ((reg8 *)   Jaw_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (Jaw_KillModeMinTime ) */

#if(Jaw_DeadBandMode == Jaw__B_PWM__DBM_256_CLOCKS)
    #define Jaw_DEADBAND_COUNT         (*(reg8 *)  Jaw_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define Jaw_DEADBAND_COUNT_PTR     ((reg8 *)   Jaw_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define Jaw_DEADBAND_LSB_PTR       ((reg8 *)   Jaw_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define Jaw_DEADBAND_LSB           (*(reg8 *)  Jaw_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(Jaw_DeadBandMode == Jaw__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (Jaw_UsingFixedFunction)
        #define Jaw_DEADBAND_COUNT         (*(reg8 *)  Jaw_PWMHW__CFG0)
        #define Jaw_DEADBAND_COUNT_PTR     ((reg8 *)   Jaw_PWMHW__CFG0)
        #define Jaw_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << Jaw_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define Jaw_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define Jaw_DEADBAND_COUNT         (*(reg8 *)  Jaw_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define Jaw_DEADBAND_COUNT_PTR     ((reg8 *)   Jaw_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define Jaw_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << Jaw_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define Jaw_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (Jaw_UsingFixedFunction) */
#endif /* (Jaw_DeadBandMode == Jaw__B_PWM__DBM_256_CLOCKS) */



#if (Jaw_UsingFixedFunction)
    #define Jaw_STATUS                 (*(reg8 *) Jaw_PWMHW__SR0)
    #define Jaw_STATUS_PTR             ((reg8 *) Jaw_PWMHW__SR0)
    #define Jaw_STATUS_MASK            (*(reg8 *) Jaw_PWMHW__SR0)
    #define Jaw_STATUS_MASK_PTR        ((reg8 *) Jaw_PWMHW__SR0)
    #define Jaw_CONTROL                (*(reg8 *) Jaw_PWMHW__CFG0)
    #define Jaw_CONTROL_PTR            ((reg8 *) Jaw_PWMHW__CFG0)
    #define Jaw_CONTROL2               (*(reg8 *) Jaw_PWMHW__CFG1)
    #define Jaw_CONTROL3               (*(reg8 *) Jaw_PWMHW__CFG2)
    #define Jaw_GLOBAL_ENABLE          (*(reg8 *) Jaw_PWMHW__PM_ACT_CFG)
    #define Jaw_GLOBAL_ENABLE_PTR      ( (reg8 *) Jaw_PWMHW__PM_ACT_CFG)
    #define Jaw_GLOBAL_STBY_ENABLE     (*(reg8 *) Jaw_PWMHW__PM_STBY_CFG)
    #define Jaw_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) Jaw_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define Jaw_BLOCK_EN_MASK          (Jaw_PWMHW__PM_ACT_MSK)
    #define Jaw_BLOCK_STBY_EN_MASK     (Jaw_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define Jaw_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define Jaw_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define Jaw_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define Jaw_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define Jaw_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define Jaw_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define Jaw_CTRL_ENABLE            (uint8)((uint8)0x01u << Jaw_CTRL_ENABLE_SHIFT)
    #define Jaw_CTRL_RESET             (uint8)((uint8)0x01u << Jaw_CTRL_RESET_SHIFT)
    #define Jaw_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << Jaw_CTRL_CMPMODE2_SHIFT)
    #define Jaw_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << Jaw_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Jaw_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define Jaw_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << Jaw_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define Jaw_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define Jaw_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define Jaw_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define Jaw_STATUS_TC_INT_EN_MASK_SHIFT            (Jaw_STATUS_TC_SHIFT - 4u)
    #define Jaw_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define Jaw_STATUS_CMP1_INT_EN_MASK_SHIFT          (Jaw_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define Jaw_STATUS_TC              (uint8)((uint8)0x01u << Jaw_STATUS_TC_SHIFT)
    #define Jaw_STATUS_CMP1            (uint8)((uint8)0x01u << Jaw_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define Jaw_STATUS_TC_INT_EN_MASK              (uint8)((uint8)Jaw_STATUS_TC >> 4u)
    #define Jaw_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)Jaw_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define Jaw_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define Jaw_RT1_MASK              (uint8)((uint8)0x03u << Jaw_RT1_SHIFT)
    #define Jaw_SYNC                  (uint8)((uint8)0x03u << Jaw_RT1_SHIFT)
    #define Jaw_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define Jaw_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << Jaw_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define Jaw_SYNCDSI_EN            (uint8)((uint8)0x0Fu << Jaw_SYNCDSI_SHIFT)


#else
    #define Jaw_STATUS                (*(reg8 *)   Jaw_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define Jaw_STATUS_PTR            ((reg8 *)    Jaw_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define Jaw_STATUS_MASK           (*(reg8 *)   Jaw_PWMUDB_genblk8_stsreg__MASK_REG)
    #define Jaw_STATUS_MASK_PTR       ((reg8 *)    Jaw_PWMUDB_genblk8_stsreg__MASK_REG)
    #define Jaw_STATUS_AUX_CTRL       (*(reg8 *)   Jaw_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define Jaw_CONTROL               (*(reg8 *)   Jaw_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define Jaw_CONTROL_PTR           ((reg8 *)    Jaw_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define Jaw_CTRL_ENABLE_SHIFT      (0x07u)
    #define Jaw_CTRL_RESET_SHIFT       (0x06u)
    #define Jaw_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define Jaw_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define Jaw_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define Jaw_CTRL_ENABLE            (uint8)((uint8)0x01u << Jaw_CTRL_ENABLE_SHIFT)
    #define Jaw_CTRL_RESET             (uint8)((uint8)0x01u << Jaw_CTRL_RESET_SHIFT)
    #define Jaw_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << Jaw_CTRL_CMPMODE2_SHIFT)
    #define Jaw_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << Jaw_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define Jaw_STATUS_KILL_SHIFT          (0x05u)
    #define Jaw_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define Jaw_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define Jaw_STATUS_TC_SHIFT            (0x02u)
    #define Jaw_STATUS_CMP2_SHIFT          (0x01u)
    #define Jaw_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define Jaw_STATUS_KILL_INT_EN_MASK_SHIFT          (Jaw_STATUS_KILL_SHIFT)
    #define Jaw_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (Jaw_STATUS_FIFONEMPTY_SHIFT)
    #define Jaw_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (Jaw_STATUS_FIFOFULL_SHIFT)
    #define Jaw_STATUS_TC_INT_EN_MASK_SHIFT            (Jaw_STATUS_TC_SHIFT)
    #define Jaw_STATUS_CMP2_INT_EN_MASK_SHIFT          (Jaw_STATUS_CMP2_SHIFT)
    #define Jaw_STATUS_CMP1_INT_EN_MASK_SHIFT          (Jaw_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define Jaw_STATUS_KILL            (uint8)((uint8)0x00u << Jaw_STATUS_KILL_SHIFT )
    #define Jaw_STATUS_FIFOFULL        (uint8)((uint8)0x01u << Jaw_STATUS_FIFOFULL_SHIFT)
    #define Jaw_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << Jaw_STATUS_FIFONEMPTY_SHIFT)
    #define Jaw_STATUS_TC              (uint8)((uint8)0x01u << Jaw_STATUS_TC_SHIFT)
    #define Jaw_STATUS_CMP2            (uint8)((uint8)0x01u << Jaw_STATUS_CMP2_SHIFT)
    #define Jaw_STATUS_CMP1            (uint8)((uint8)0x01u << Jaw_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define Jaw_STATUS_KILL_INT_EN_MASK            (Jaw_STATUS_KILL)
    #define Jaw_STATUS_FIFOFULL_INT_EN_MASK        (Jaw_STATUS_FIFOFULL)
    #define Jaw_STATUS_FIFONEMPTY_INT_EN_MASK      (Jaw_STATUS_FIFONEMPTY)
    #define Jaw_STATUS_TC_INT_EN_MASK              (Jaw_STATUS_TC)
    #define Jaw_STATUS_CMP2_INT_EN_MASK            (Jaw_STATUS_CMP2)
    #define Jaw_STATUS_CMP1_INT_EN_MASK            (Jaw_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define Jaw_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define Jaw_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define Jaw_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define Jaw_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define Jaw_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* Jaw_UsingFixedFunction */

#endif  /* CY_PWM_Jaw_H */


/* [] END OF FILE */
