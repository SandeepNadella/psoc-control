/*******************************************************************************
* File Name: Left_Neck.h  
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

#if !defined(CY_PINS_Left_Neck_H) /* Pins Left_Neck_H */
#define CY_PINS_Left_Neck_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Left_Neck_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Left_Neck__PORT == 15 && ((Left_Neck__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Left_Neck_Write(uint8 value);
void    Left_Neck_SetDriveMode(uint8 mode);
uint8   Left_Neck_ReadDataReg(void);
uint8   Left_Neck_Read(void);
void    Left_Neck_SetInterruptMode(uint16 position, uint16 mode);
uint8   Left_Neck_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Left_Neck_SetDriveMode() function.
     *  @{
     */
        #define Left_Neck_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Left_Neck_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Left_Neck_DM_RES_UP          PIN_DM_RES_UP
        #define Left_Neck_DM_RES_DWN         PIN_DM_RES_DWN
        #define Left_Neck_DM_OD_LO           PIN_DM_OD_LO
        #define Left_Neck_DM_OD_HI           PIN_DM_OD_HI
        #define Left_Neck_DM_STRONG          PIN_DM_STRONG
        #define Left_Neck_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Left_Neck_MASK               Left_Neck__MASK
#define Left_Neck_SHIFT              Left_Neck__SHIFT
#define Left_Neck_WIDTH              1u

/* Interrupt constants */
#if defined(Left_Neck__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Left_Neck_SetInterruptMode() function.
     *  @{
     */
        #define Left_Neck_INTR_NONE      (uint16)(0x0000u)
        #define Left_Neck_INTR_RISING    (uint16)(0x0001u)
        #define Left_Neck_INTR_FALLING   (uint16)(0x0002u)
        #define Left_Neck_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Left_Neck_INTR_MASK      (0x01u) 
#endif /* (Left_Neck__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Left_Neck_PS                     (* (reg8 *) Left_Neck__PS)
/* Data Register */
#define Left_Neck_DR                     (* (reg8 *) Left_Neck__DR)
/* Port Number */
#define Left_Neck_PRT_NUM                (* (reg8 *) Left_Neck__PRT) 
/* Connect to Analog Globals */                                                  
#define Left_Neck_AG                     (* (reg8 *) Left_Neck__AG)                       
/* Analog MUX bux enable */
#define Left_Neck_AMUX                   (* (reg8 *) Left_Neck__AMUX) 
/* Bidirectional Enable */                                                        
#define Left_Neck_BIE                    (* (reg8 *) Left_Neck__BIE)
/* Bit-mask for Aliased Register Access */
#define Left_Neck_BIT_MASK               (* (reg8 *) Left_Neck__BIT_MASK)
/* Bypass Enable */
#define Left_Neck_BYP                    (* (reg8 *) Left_Neck__BYP)
/* Port wide control signals */                                                   
#define Left_Neck_CTL                    (* (reg8 *) Left_Neck__CTL)
/* Drive Modes */
#define Left_Neck_DM0                    (* (reg8 *) Left_Neck__DM0) 
#define Left_Neck_DM1                    (* (reg8 *) Left_Neck__DM1)
#define Left_Neck_DM2                    (* (reg8 *) Left_Neck__DM2) 
/* Input Buffer Disable Override */
#define Left_Neck_INP_DIS                (* (reg8 *) Left_Neck__INP_DIS)
/* LCD Common or Segment Drive */
#define Left_Neck_LCD_COM_SEG            (* (reg8 *) Left_Neck__LCD_COM_SEG)
/* Enable Segment LCD */
#define Left_Neck_LCD_EN                 (* (reg8 *) Left_Neck__LCD_EN)
/* Slew Rate Control */
#define Left_Neck_SLW                    (* (reg8 *) Left_Neck__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Left_Neck_PRTDSI__CAPS_SEL       (* (reg8 *) Left_Neck__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Left_Neck_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Left_Neck__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Left_Neck_PRTDSI__OE_SEL0        (* (reg8 *) Left_Neck__PRTDSI__OE_SEL0) 
#define Left_Neck_PRTDSI__OE_SEL1        (* (reg8 *) Left_Neck__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Left_Neck_PRTDSI__OUT_SEL0       (* (reg8 *) Left_Neck__PRTDSI__OUT_SEL0) 
#define Left_Neck_PRTDSI__OUT_SEL1       (* (reg8 *) Left_Neck__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Left_Neck_PRTDSI__SYNC_OUT       (* (reg8 *) Left_Neck__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Left_Neck__SIO_CFG)
    #define Left_Neck_SIO_HYST_EN        (* (reg8 *) Left_Neck__SIO_HYST_EN)
    #define Left_Neck_SIO_REG_HIFREQ     (* (reg8 *) Left_Neck__SIO_REG_HIFREQ)
    #define Left_Neck_SIO_CFG            (* (reg8 *) Left_Neck__SIO_CFG)
    #define Left_Neck_SIO_DIFF           (* (reg8 *) Left_Neck__SIO_DIFF)
#endif /* (Left_Neck__SIO_CFG) */

/* Interrupt Registers */
#if defined(Left_Neck__INTSTAT)
    #define Left_Neck_INTSTAT            (* (reg8 *) Left_Neck__INTSTAT)
    #define Left_Neck_SNAP               (* (reg8 *) Left_Neck__SNAP)
    
	#define Left_Neck_0_INTTYPE_REG 		(* (reg8 *) Left_Neck__0__INTTYPE)
#endif /* (Left_Neck__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Left_Neck_H */


/* [] END OF FILE */
