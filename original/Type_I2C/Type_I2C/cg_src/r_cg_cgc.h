/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_cgc.h
* Version      : Code Generator for RL78/I1E V1.02.01.02 [24 Feb 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for CGC module.
* Creation Date: 2017/06/14
***********************************************************************************************************************/
#ifndef CGC_H
#define CGC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Clock operation mode control register (CMC) 
*/
/* High-speed system clock pin operation mode (EXCLK,OSCSEL) */
#define _00_CGC_HISYS_PORT                       (0x00U) /* X1, X2 as I/O port */
#define _40_CGC_HISYS_OSC                        (0x40U) /* X1, X2 as crystal/ceramic resonator connection */
#define _80_CGC_HISYS_PORT1                      (0x80U) /* X1, X2 as I/O port */
#define _C0_CGC_HISYS_EXT                        (0xC0U) /* X1 as I/O port, X2 as external clock input */
#define _C0_CGC_HISYS_PIN                        (0xC0U) /* X1, X2 pin setting */
/* Control of X1 clock oscillation frequency (AMPH) */
#define _00_CGC_SYSOSC_UNDER10M                  (0x00U) /* 1MHz <= fX <= 10MHz */
#define _01_CGC_SYSOSC_OVER10M                   (0x01U) /* 10MHz < fX <= 20MHz */

/*
    System clock control register (CKC) 
*/
/* Main system clock (fMAIN) operation control (MCM0) */
#define _00_CGC_MAINCLK_SELFOCO                  (0x00U) /* selects high-speed on-chip oscillator clock (fHOCO) or PLL clock (fPLL) as the main system clock (fMAIN) */
#define _10_CGC_MAINCLK_SELHISYS                 (0x10U) /* selects the high-speed system clock (fMX) as the main system clock (fMAIN) */
/* Status of Main system clock (fMAIN) (MCS) */
#define _00_CGC_MAINCLK_FOCO                     (0x00U) /* High-speed on-chip oscillator clock (fIH) */
#define _20_CGC_MAINCLK_HISYS                    (0x20U) /* high-speed system clock (fMX) */

/*
    Clock operation status control register (CSC) 
*/
/* High-speed on-chip oscillator clock operation control (HIOSTOP) */
#define _00_CGC_HIO_OPER                         (0x00U) /* high-speed on-chip oscillator operating */
#define _01_CGC_HIO_STOP                         (0x01U) /* high-speed on-chip oscillator stopped */
/* High-speed system clock operation control (MSTOP) */
#define _00_CGC_HISYS_OPER                       (0x00U) /* X1 oscillator/external clock operating */
#define _80_CGC_HISYS_STOP                       (0x80U) /* X1 oscillator/external clock stopped */

/*
    Oscillation stabilization time counter status register (OSTC) 
*/
/* oscillation stabilization time status (MOST8,MOST9,MOST10,MOST11,MOST13,MOST15,MOST17,MOST18) */
#define _00_CGC_OSCSTAB_STA0                     (0x00U) /* 2^8/fX max. */
#define _80_CGC_OSCSTAB_STA8                     (0x80U) /* 2^8/fX */
#define _C0_CGC_OSCSTAB_STA9                     (0xC0U) /* 2^9/fX */
#define _E0_CGC_OSCSTAB_STA10                    (0xE0U) /* 2^10/fX */
#define _F0_CGC_OSCSTAB_STA11                    (0xF0U) /* 2^11/fX */
#define _F8_CGC_OSCSTAB_STA13                    (0xF8U) /* 2^13/fX */
#define _FC_CGC_OSCSTAB_STA15                    (0xFCU) /* 2^15/fX */
#define _FE_CGC_OSCSTAB_STA17                    (0xFEU) /* 2^17/fX */
#define _FF_CGC_OSCSTAB_STA18                    (0xFFU) /* 2^18/fX */

/*
    Oscillation stabilization time select register (OSTS) 
*/
/* oscillation stabilization time selection (OSTS2,OSTS1,OSTS0) */
#define _00_CGC_OSCSTAB_SEL8                     (0x00U) /* 2^8/fX */
#define _01_CGC_OSCSTAB_SEL9                     (0x01U) /* 2^9/fX */
#define _02_CGC_OSCSTAB_SEL10                    (0x02U) /* 2^10/fX */
#define _03_CGC_OSCSTAB_SEL11                    (0x03U) /* 2^11/fX */
#define _04_CGC_OSCSTAB_SEL13                    (0x04U) /* 2^13/fX */
#define _05_CGC_OSCSTAB_SEL15                    (0x05U) /* 2^15/fX */
#define _06_CGC_OSCSTAB_SEL17                    (0x06U) /* 2^17/fX */
#define _07_CGC_OSCSTAB_SEL18                    (0x07U) /* 2^18/fX */

/*
    PLL control register (DSCCTL) 
*/
/* Selection of division factor for PLL reference clock (DSFRDIV) */
#define _00_CGC_FPLL_DIVIDED_UNDIVIDED           (0x00U) /* Undivided (fMX) */
#define _04_CGC_FPLL_DIVIDED_2                   (0x04U) /* Divided by 2 (fMX/2) */
/* Selection of multiplication factor for PLL (DSCM) */
#define _00_CGC_FPLL_MULTIPLIED_12               (0x00U) /* Multiplied by 12 (x6) */
#define _02_CGC_FPLL_MULTIPLIED_16               (0x02U) /* Multiplied by 16 (x8) */
/* Control of PLL oscillation and clock output (DSCON) */
#define _00_CGC_FPLL_Stop                        (0x00U) /* Stop */
#define _01_CGC_FPLL_Start                       (0x01U) /* Starts oscillation and outputs the clock */

/*
    Main clock control register (MCKC) 
*/
/* Selection of PLL clock division ratio (RDIV1,RDIV0) */
#define _00_CGC_FIH_FPLL_DIVIED2                 (0x00U) /* Divided by 2 */
#define _02_CGC_FIH_FPLL_DIVIED4                 (0x02U) /* Divided by 4 */
#define _04_CGC_FIH_FPLL_DIVIED8                 (0x04U) /* Divided by 8 */
/* Selection of high-speed on-chip oscillator clock/PLL clock (CKSELR) */
#define _00_CGC_FIH_FHOCO                        (0x00U) /* High-speed on-chip oscillator clock (fHOCO) */
#define _01_CGC_FIH_FPLL                         (0x01U) /* PLL clock (fPLL) */

/*
    Peripheral clock control register (PCKC) 
*/
/* Selection of PLL clock division ratio (DSADCK1,DSADCK0) */
#define _00_CGC_FDSAD_FHOCO                      (0x00U) /* High-speed on-chip oscillator clock (fHOCO) */
#define _01_CGC_FDSAD_FPLL_DIVIED2               (0x01U) /* Clock obtained by dividing PLL clock (fPLL) by 2 */
#define _02_CGC_FDSAD_FMX                        (0x02U) /* High-speed system clock (fMX) */

/*
    RTC clock select register (RTCCL) 
*/
/* Selection of source of operating clock of RTC and interval timer (RTCCL7) */
#define _00_CGC_FRTC_FMX                         (0x00U) /* High-speed system clock (fMX) */
#define _80_CGC_FRTC_FHOCO                       (0x80U) /* High-speed on-chip oscillator clock (fHOCO) */
/* Selection of frequency divisor for operating clock of RTC and interval timer 1 (RTCCL6,RTCCL5) */
#define _00_CGC_FRTC_DIVIDED1_122                (0x00U) /* Divided by 122 */
#define _20_CGC_FRTC_DIVIDED1_305                (0x20U) /* Divided by 305 */
#define _40_CGC_FRTC_DIVIDED1_183                (0x40U) /* Divided by 183 */
/* Selection of frequency divisor for operating clock of RTC and interval timer 2 (RTCCKS1,RTCCKS0) */
#define _00_CGC_FRTC_DIVIDED2_UNDIVIDED          (0x00U) /* Undivided */
#define _01_CGC_FRTC_DIVIDED2_2                  (0x01U) /* Divided by 2 */
#define _02_CGC_FRTC_DIVIDED2_4                  (0x02U) /* Divided by 4 */
#define _03_CGC_FRTC_DIVIDED2_8                  (0x03U) /* Divided by 8 */

/*
    Subsystem clock supply mode control register (OSMC) 
*/
/* Selection of operating clock (fRTC) for real-time clock, interval timer, and timer RJ (WUTMMCK0) */
#define _00_CGC_RTC_IT_CLK_FRTC                  (0x00U) /* stop */
#define _10_CGC_TMRJ_IT_CLK_FIL                  (0x10U) /* low-speed on-chip oscillator clock */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CGC_FPLLWAITTIME            (80U)   /* change the waiting time according to the system */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    HIOCLK,
    SYSX1CLK,
    SYSEXTCLK,
    PLLCLK
} clock_mode_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_CGC_Create(void);
void R_CGC_Get_ResetSource(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
