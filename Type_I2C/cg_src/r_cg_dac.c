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
* File Name    : r_cg_dac.c
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for DAC module.
* Creation Date: 27/07/2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_dac.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_DAC_Create
* Description  : This function initializes the selectable power-on-reset circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC_Create(void)
{
    AFEEN = 1U;     /* supplies AFE input clock */
    AFEPON = 1U;    /* power on AFE */
    /* Wait until AFE stabilize  */ 
    while(0U == AFESTAT);
    DACEN = 1U;     /* supplies DAC input clock */
    DACM0 = _00_DAC_TRIG_SOFTWARE | _00_DAC_BIT_12;
    DACM1 = _00_DAC_AVDD;
    DACD = _0FFF_DAC_SAMPLE_COUNT;
}
/***********************************************************************************************************************
* Function Name: R_DAC_Start
* Description  : This function starts the DAC.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC_Start(void)
{	
    volatile uint16_t w_count;    
    DACPON = 1U;    /* power on DAC */
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= DA_WAITINGTIME; w_count++)
    {
        NOP();
    }
}
/***********************************************************************************************************************
* Function Name: R_DAC_Stop
* Description  : This function stops the DAC.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC_Stop(void)
{
    DACPON = 0U;    /* power off DAC */
}
/***********************************************************************************************************************
* Function Name: R_DAC_Change_OutputVoltage
* Description  : This function change the outputvoltage of DAC.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC_Change_OutputVoltage(uint16_t outputVoltage)
{
    DACD = outputVoltage;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
