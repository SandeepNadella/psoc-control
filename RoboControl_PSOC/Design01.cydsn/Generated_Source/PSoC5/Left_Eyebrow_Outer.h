/*******************************************************************************
* File Name: Left_Eyebrow_Outer.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Left_Eyebrow_Outer_H) /* Pins Left_Eyebrow_Outer_H */
#define CY_PINS_Left_Eyebrow_Outer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Left_Eyebrow_Outer_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Left_Eyebrow_Outer__PORT == 15 && ((Left_Eyebrow_Outer__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Left_Eyebrow_Outer_Write(uint8 value);
void    Left_Eyebrow_Outer_SetDriveMode(uint8 mode);
uint8   Left_Eyebrow_Outer_ReadDataReg(void);
uint8   Left_Eyebrow_Outer_Read(void);
void    Left_Eyebrow_Outer_SetInterruptMode(uint16 position, uint16 mode);
uint8   Left_Eyebrow_Outer_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Left_Eyebrow_Outer_SetDriveMode() function.
     *  @{
     */
        #define Left_Eyebrow_Outer_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Left_Eyebrow_Outer_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Left_Eyebrow_Outer_DM_RES_UP          PIN_DM_RES_UP
        #define Left_Eyebrow_Outer_DM_RES_DWN         PIN_DM_RES_DWN
        #define Left_Eyebrow_Outer_DM_OD_LO           PIN_DM_OD_LO
        #define Left_Eyebrow_Outer_DM_OD_HI           PIN_DM_OD_HI
        #define Left_Eyebrow_Outer_DM_STRONG          PIN_DM_STRONG
        #define Left_Eyebrow_Outer_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Left_Eyebrow_Outer_MASK               Left_Eyebrow_Outer__MASK
#define Left_Eyebrow_Outer_SHIFT              Left_Eyebrow_Outer__SHIFT
#define Left_Eyebrow_Outer_WIDTH              1u

/* Interrupt constants */
#if defined(Left_Eyebrow_Outer__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Left_Eyebrow_Outer_SetInterruptMode() function.
     *  @{
     */
        #define Left_Eyebrow_Outer_INTR_NONE      (uint16)(0x0000u)
        #define Left_Eyebrow_Outer_INTR_RISING    (uint16)(0x0001u)
        #define Left_Eyebrow_Outer_INTR_FALLING   (uint16)(0x0002u)
        #define Left_Eyebrow_Outer_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Left_Eyebrow_Outer_INTR_MASK      (0x01u) 
#endif /* (Left_Eyebrow_Outer__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Left_Eyebrow_Outer_PS                     (* (reg8 *) Left_Eyebrow_Outer__PS)
/* Data Register */
#define Left_Eyebrow_Outer_DR                     (* (reg8 *) Left_Eyebrow_Outer__DR)
/* Port Number */
#define Left_Eyebrow_Outer_PRT_NUM                (* (reg8 *) Left_Eyebrow_Outer__PRT) 
/* Connect to Analog Globals */                                                  
#define Left_Eyebrow_Outer_AG                     (* (reg8 *) Left_Eyebrow_Outer__AG)                       
/* Analog MUX bux enable */
#define Left_Eyebrow_Outer_AMUX                   (* (reg8 *) Left_Eyebrow_Outer__AMUX) 
/* Bidirectional Enable */                                                        
#define Left_Eyebrow_Outer_BIE                    (* (reg8 *) Left_Eyebrow_Outer__BIE)
/* Bit-mask for Aliased Register Access */
#define Left_Eyebrow_Outer_BIT_MASK               (* (reg8 *) Left_Eyebrow_Outer__BIT_MASK)
/* Bypass Enable */
#define Left_Eyebrow_Outer_BYP                    (* (reg8 *) Left_Eyebrow_Outer__BYP)
/* Port wide control signals */                                                   
#define Left_Eyebrow_Outer_CTL                    (* (reg8 *) Left_Eyebrow_Outer__CTL)
/* Drive Modes */
#define Left_Eyebrow_Outer_DM0                    (* (reg8 *) Left_Eyebrow_Outer__DM0) 
#define Left_Eyebrow_Outer_DM1                    (* (reg8 *) Left_Eyebrow_Outer__DM1)
#define Left_Eyebrow_Outer_DM2                    (* (reg8 *) Left_Eyebrow_Outer__DM2) 
/* Input Buffer Disable Override */
#define Left_Eyebrow_Outer_INP_DIS                (* (reg8 *) Left_Eyebrow_Outer__INP_DIS)
/* LCD Common or Segment Drive */
#define Left_Eyebrow_Outer_LCD_COM_SEG            (* (reg8 *) Left_Eyebrow_Outer__LCD_COM_SEG)
/* Enable Segment LCD */
#define Left_Eyebrow_Outer_LCD_EN                 (* (reg8 *) Left_Eyebrow_Outer__LCD_EN)
/* Slew Rate Control */
#define Left_Eyebrow_Outer_SLW                    (* (reg8 *) Left_Eyebrow_Outer__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Left_Eyebrow_Outer_PRTDSI__CAPS_SEL       (* (reg8 *) Left_Eyebrow_Outer__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Left_Eyebrow_Outer_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Left_Eyebrow_Outer__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Left_Eyebrow_Outer_PRTDSI__OE_SEL0        (* (reg8 *) Left_Eyebrow_Outer__PRTDSI__OE_SEL0) 
#define Left_Eyebrow_Outer_PRTDSI__OE_SEL1        (* (reg8 *) Left_Eyebrow_Outer__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Left_Eyebrow_Outer_PRTDSI__OUT_SEL0       (* (reg8 *) Left_Eyebrow_Outer__PRTDSI__OUT_SEL0) 
#define Left_Eyebrow_Outer_PRTDSI__OUT_SEL1       (* (reg8 *) Left_Eyebrow_Outer__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Left_Eyebrow_Outer_PRTDSI__SYNC_OUT       (* (reg8 *) Left_Eyebrow_Outer__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Left_Eyebrow_Outer__SIO_CFG)
    #define Left_Eyebrow_Outer_SIO_HYST_EN        (* (reg8 *) Left_Eyebrow_Outer__SIO_HYST_EN)
    #define Left_Eyebrow_Outer_SIO_REG_HIFREQ     (* (reg8 *) Left_Eyebrow_Outer__SIO_REG_HIFREQ)
    #define Left_Eyebrow_Outer_SIO_CFG            (* (reg8 *) Left_Eyebrow_Outer__SIO_CFG)
    #define Left_Eyebrow_Outer_SIO_DIFF           (* (reg8 *) Left_Eyebrow_Outer__SIO_DIFF)
#endif /* (Left_Eyebrow_Outer__SIO_CFG) */

/* Interrupt Registers */
#if defined(Left_Eyebrow_Outer__INTSTAT)
    #define Left_Eyebrow_Outer_INTSTAT            (* (reg8 *) Left_Eyebrow_Outer__INTSTAT)
    #define Left_Eyebrow_Outer_SNAP               (* (reg8 *) Left_Eyebrow_Outer__SNAP)
    
	#define Left_Eyebrow_Outer_0_INTTYPE_REG 		(* (reg8 *) Left_Eyebrow_Outer__0__INTTYPE)
#endif /* (Left_Eyebrow_Outer__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Left_Eyebrow_Outer_H */


/* [] END OF FILE */
