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
* File Name    : r_cg_pga_dsad.c
* Version      : Code Generator for RL78/I1E V1.02.01.02 [24 Feb 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for PGIA module.
* Creation Date: 2017/06/06
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_pga_dsad.h"
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
* Function Name: R_PGA_DSAD_Create
* Description  : This function initializes the PGA and DS A/D converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Create(void)
{
    AFEEN = 1U;     /* supplies AFE input clock */
    AFEPON = 1U;    /* power on AFE */
    /* Wait until AFE stabilize  */ 
    while(0U == AFESTAT);
    PGAEN = 1U;     /* supplies PGA and DS A/D  input clock */
    DSADMR = _00_DSAD_CONVMODE_NORMAL;
    AFECKS = _0E_AFE_CLOCK_DIVIDED8;
    VSBIAS = _0E_VSBIAS_OUTPUT_VOLTAGE_10;
    PGAPON = 1U;    /* power on PGA and DS A/D */
    /* Wait until PGA stabilize  */ 
    while(0U == PGASTAT);
    DSADMK = 1U;    /* disable INTDSAD interrupt */
    DSADIF = 0U;    /* clear INTDSAD interrupt flag */
    /* Set INTDSAD level 2 priority */
    DSADPR1 = 1U;
    DSADPR0 = 0U;
    DSADSMK = 1U;   /* disable INTDSADS interrupt */
    DSADSIF = 0U;   /* clear INTDSADS interrupt flag */
    /* Set INTDSADS low priority */
    DSADSPR1 = 1U;
    DSADSPR0 = 1U;
    DSADMR |= _00_DSAD_CONVTRI_SOFTWARE;
    DSADCTL = _00_DSAD_CONVERSION_STOP | _20_DSAD_SCAN_MODE_STOP | _10_DSAD_INPUT4_DISABLE | 
              _00_DSAD_INPUT3_ENABLE | _00_DSAD_INPUT2_ENABLE | _00_DSAD_INPUT1_ENABLE | _00_DSAD_INPUT0_ENABLE;
    PGA0CTL0 = _00_PGA_CHANNELn_SAMPLE_RATIO_64 | _10_PGA_CHANNELn_GSET1_8 | _00_PGA_CHANNELn_GSET2_1;
    PGA1CTL0 = _00_PGA_CHANNELn_SAMPLE_RATIO_64 | _10_PGA_CHANNELn_GSET1_8 | _00_PGA_CHANNELn_GSET2_1;
    PGA2CTL0 = _00_PGA_CHANNELn_SAMPLE_RATIO_64 | _10_PGA_CHANNELn_GSET1_8 | _00_PGA_CHANNELn_GSET2_1;
    PGA3CTL0 = _00_PGA_CHANNELn_SAMPLE_RATIO_64 | _10_PGA_CHANNELn_GSET1_8 | _00_PGA_CHANNELn_GSET2_1;
    PGA0CTL1 = _00_PGA_CHANNELn_DIFFERENTIAL | _10_PGA_CHANNELn_OFFSET_16;
    PGA1CTL1 = _00_PGA_CHANNELn_DIFFERENTIAL | _10_PGA_CHANNELn_OFFSET_16;
    PGA2CTL1 = _00_PGA_CHANNELn_DIFFERENTIAL | _10_PGA_CHANNELn_OFFSET_16;
    PGA3CTL1 = _00_PGA_CHANNELn_DIFFERENTIAL | _00_PGA_CHANNELn_PIN | _10_PGA_CHANNELn_OFFSET_16;
    PGA0CTL2 = _01_PGA_CHANNEL0_SAMPLE_COUNT;
    PGA1CTL2 = _01_PGA_CHANNEL1_SAMPLE_COUNT;
    PGA2CTL2 = _01_PGA_CHANNEL2_SAMPLE_COUNT;
    PGA3CTL2 = _01_PGA_CHANNEL3_SAMPLE_COUNT;
    PGA0CTL3 = _00_PGA_CHANNELn_MUMBER_MODE_8032 | _0C_PGA_CHANNELn_AVERAGING_MEAM_VALUE | 
               _00_PGA_CHANNELn_AVERAGING_UNIT_8;
    PGA1CTL3 = _00_PGA_CHANNELn_MUMBER_MODE_8032 | _0C_PGA_CHANNELn_AVERAGING_MEAM_VALUE | 
               _00_PGA_CHANNELn_AVERAGING_UNIT_8;
    PGA2CTL3 = _00_PGA_CHANNELn_MUMBER_MODE_8032 | _0C_PGA_CHANNELn_AVERAGING_MEAM_VALUE | 
               _00_PGA_CHANNELn_AVERAGING_UNIT_8;
    PGA3CTL3 = _00_PGA_CHANNELn_MUMBER_MODE_8032 | _0C_PGA_CHANNELn_AVERAGING_MEAM_VALUE | 
               _00_PGA_CHANNELn_AVERAGING_UNIT_8;
    PGABOD = _00_DSAD_DISCONNECTION_DETECTION_UNUSED;
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Start
* Description  : This function starts the PGA and DS A/D converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Start(void)
{
    DSADIF = 0U;    /* clear INTDSAD interrupt flag */
    DSADMK = 0U;    /* enable INTDSAD interrupt */
    DSADSIF = 0U;   /* clear INTDSADS interrupt flag */
    DSADSMK = 0U;   /* enable INTDSADS interrupt */
    DSADST = 1U;    /* start conversion */
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Stop
* Description  : This function stops the PGA and DS A/D converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Stop(void)
{
    DSADMK = 1U;    /* disable INTDSAD interrupt */
    DSADIF = 0U;    /* clear INTDSAD interrupt flag */
    DSADSMK = 1U;   /* disable INTDSADS interrupt */
    DSADSIF = 0U;   /* clear INTDSADS interrupt flag */
    DSADST = 0U;    /* stop conversion */
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Get_Result
* Description  : This function returns the DS A/D conversion result.
* Arguments    : bufferH -
*                    high 16 bits
*                bufferL -
*                    low 16 bits
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Get_Result(uint16_t * const bufferH,uint16_t * const bufferL)
{
	*bufferH = DSADCR1;
	*bufferL = DSADCR0;
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Get_AverageResult
* Description  : This function returns the DS A/D conversion average result.
* Arguments    : bufferH -
*                    high 16 bits
*                bufferL -
*                    low 16 bits
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Get_AverageResult(uint16_t * const bufferH,uint16_t * const bufferL)
{
	*bufferH = DSADMV1;
	*bufferL = DSADMV0;
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_CAMP_OffsetTrimming
* Description  : This function trims the offset for the configurable amplifier.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_CAMP_OffsetTrimming(void)
{
    int32_t     pga_ad_offset  = 0;             /* PGA offset data                           */
    int32_t     ampn_ad_offset = 0;             /* AMPn offset data                          */
    int32_t     offset;                         /* offset data                               */
    int8_t      bit_num;                        /* bit number of the trimming target         */
    uint8_t     amp_ch = 0;                     /* the variable for selecting CAMP channel   */
    uint16_t    w_count;                        /* the counter for waiting for stabilization */
    
    /* Base address for register of CONFIGURABLE AMPLIFIER */
    uint8_t     *p_address_ampncal = (uint8_t *) & AMP0CAL;
    uint8_t     *p_address_ampns0  = (uint8_t *) & AMP0S0;
    uint8_t     *p_address_ampns1  = (uint8_t *) & AMP0S1;
    uint8_t     *p_address_ampns2  = (uint8_t *) & AMP0S2;

    /* A set value for CONFIGURABLE AMPLIFIER */
    uint8_t     amp0s0_reg_value[3U]  = {_40_DSAD_OFFSET_TRIMMING_MONITOR_AMP0, _80_DSAD_OFFSET_TRIMMING_MONITOR_AMP1,
                                         _C0_DSAD_OFFSET_TRIMMING_MONITOR_AMP2};
    uint8_t     ampnpon_reg_value[3U] = {0x10U, 0x20U, 0x40U};

    /*  Variable for a backup of register setting */
    uint8_t     back_afepws;

    uint8_t     back_dsadctl;
    uint8_t     back_pga3ctl0;
    uint8_t     back_pga3ctl1;
    uint8_t     back_pga3ctl2;
    uint8_t     back_pga3ctl3;
    
    uint8_t     back_ampns0[3U];
    uint8_t     back_ampns1[3U];
    uint8_t     back_ampns2[3U];

    uint8_t     back_dacen;
    uint16_t    back_dacd;
    uint8_t     back_dacm0;
    uint8_t     back_dacm1;

    /* The temporary variable for preserving a result of A/D conversion. */
    union
    {
        int32_t     LONG;                           /* 32bit access */
        uint16_t    WORD[2];                        /* 16bit access */
    } temp_dsad;


    /*-----------------------------------------------------------------*
     * This procedure saves a variable that will be overwrited in API. *
     *-----------------------------------------------------------------*/
     
    /* An analog front end power supply selection register is saved. */
    back_afepws = AFEPWS;
    
    /* A set value of autoscan mode is saved. */
    back_dsadctl = DSADCTL;

    /* Set values of A/D converting of  Multiplexer3 are saved. */
    back_pga3ctl0 = PGA3CTL0;
    back_pga3ctl1 = PGA3CTL1;
    back_pga3ctl2 = PGA3CTL2;
    back_pga3ctl3 = PGA3CTL3;
   
    /* Set values of all the channel of CAMP are saved. */
    for (amp_ch = 0; amp_ch < 3U; amp_ch++)
    {
        back_ampns0[amp_ch] = *(p_address_ampns0 + amp_ch * 4U); /* for AMP0S0,AMP1S0,AMP2SO */
        back_ampns1[amp_ch] = *(p_address_ampns1 + amp_ch * 4U); /* for AMP0S1,AMP1S1,AMP2S1 */
        back_ampns2[amp_ch] = *(p_address_ampns2 + amp_ch * 4U); /* for AMP0S2,AMP1S2,AMP2S2 */
    }
    
    /* Set values of DAC are saved. */
    back_dacen = DACEN;
    back_dacd  = DACD;
    back_dacm0 = DACM0;
    back_dacm1 = DACM1;


    /*----------------------------------------------------------------------------------------------------*
     * Initialization                                                                                     *
     *----------------------------------------------------------------------------------------------------*
     * Notes:                                                                                             *
     *  Before calling this function, the following processing is done by the R_PGA_DSAD_Create function. *
     *   AFE circuit power-on setting                                                                     *
     *   Select the AFE operating clock.                                                                  *
     *----------------------------------------------------------------------------------------------------*/

    AMPEN  = 1U;  /* Enables input clock supply for AMP.           */
    DACEN  = 1U;  /* Enables input clock supply for 12-BIT D/A.    */
    DACPON = 0U;  /* The power supply of 12-BIT D/A is turned off. */
    
    AMP0S0 = 0x00U; /* Input multiplexer 3 is set to PGA offset self-diagnosis mode.  */

    /* OSR of input multiplexer 3 is set to 2048. Gain setting of the input multiplexer 3 is set to 32. */
    PGA3CTL0 = _A0_PGA_CHANNELn_SAMPLE_RATIO_2048 | _10_PGA_CHANNELn_GSET1_8 | _02_PGA_CHANNELn_GSET2_4;
    
    
    /* Input multiplexer 3 is set to differential input.           
       Input multiplexer 3 is set to PGA OFFSET SELF-DIAGNOSIS MODE.
       Offset voltage of input multiplexer 3 is set to 0.            */
    PGA3CTL1 = _00_PGA_CHANNELn_DIFFERENTIAL | _40_PGA_CHANNELn_OFFSET | _10_PGA_CHANNELn_OFFSET_16;
    

    /* Input multiplexer 3 does not average the A/D conversion results. */
    PGA3CTL2 = 0x00U;
    PGA3CTL3 = _00_PGA_CHANNELn_MUMBER_MODE_8032 | _00_PGA_CHANNELn_AVERAGING_NOT | _00_PGA_CHANNELn_AVERAGING_UNIT_8;
    
    /* A/D conversion of input multiplexer 3 is started. Input multiplexer 3 is set to single scan mode.   */
    DSADCTL = _00_DSAD_CONVERSION_STOP | _20_DSAD_SCAN_MODE_STOP | _10_DSAD_INPUT4_DISABLE | _00_DSAD_INPUT3_ENABLE
            | _04_DSAD_INPUT2_DISABLE  | _02_DSAD_INPUT1_DISABLE | _01_DSAD_INPUT0_DISABLE;
    
    
    /* The power supply of AMP0, AMP1, and AMP2 are turned off. 
       All of the connections of AMP0, AMP1, and AMP2 are disconnected. */
    for (amp_ch = 0; amp_ch < 3U; amp_ch++)
    {
        AFEPWS &= ~ampnpon_reg_value[amp_ch];
        *(p_address_ampns0 + amp_ch * 4U) = 0x00U;  /* for AMP0S0,AMP1S0,AMP2SO */
        *(p_address_ampns1 + amp_ch * 4U) = 0x00U;  /* for AMP0S1,AMP1S1,AMP2S1 */
        *(p_address_ampns2 + amp_ch * 4U) = 0x00U;  /* for AMP0S2,AMP1S2,AMP2S2 */
    }




    /*------------------------------------------------------------------*/
    /* This procedure measures PGA offset voltage.                      */
    /*------------------------------------------------------------------*/
    
    /* Start A/D conversion */
    DSADST = 1U;

    /*  This waits for the completion of  A/D conversion. */
    while (1U == DSADST)
    {
        /* Do Nothing */
    }

    /* This procedure gets a value of A/D conversion. */
    R_PGA_DSAD_Get_Result(&temp_dsad.WORD[1U], &temp_dsad.WORD[0U]);
    pga_ad_offset = temp_dsad.LONG & 0xFFFFFF00L;
    pga_ad_offset >>= 8U;


    /* --------------------------------------------------------------------*/
    /* This procedure starts of offset trimming of CONFIGURABLE AMPLIFIERn.*/
    /* --------------------------------------------------------------------*/
    for (amp_ch = 0; amp_ch < 3U; amp_ch++)
    {
        /* 1.0V is supplied to the terminal ANX0. Output voltage of 12-BIT D/A is set to 1.0V. */
        DACM0   = _00_DSAD_DAC_TRIG_SOFTWARE | _00_DSAD_DAC_BIT_12;
        DACM1   = _01_DSAD_DAC_VREFDA;

        DACD    = 0x0B08U;
        DACPON  = 1U;


        /* CONFIGURABLE AMPLIFIER is used for a output amplifier of 12-BIT D/A. */
        *(p_address_ampns1 + amp_ch * 4U) = 0x80U;  /* for AMP0S1,AMP1S1,AMP2S1 */
        *(p_address_ampns2 + amp_ch * 4U) = 0x80U;  /* for AMP0S2,AMP1S2,AMP2S2 */

        /* The power supply of AMP0/AMP1/AMP2 is turned on. */
        AFEPWS |= ampnpon_reg_value[amp_ch];

        /* The time of the stabilization waiting is 60 micro second or more. */
        for (w_count = 0U; w_count <= 1920U; w_count++)
        {
            NOP();
        }
        
        /* AMP0/AMP1/AMP2 is connected to PGA. */
        AMP0S0 = amp0s0_reg_value[amp_ch];

        /* The initial value of offset trimming is set to AMPnCAL register. */
        *(p_address_ampncal + amp_ch) = 0U;     /* for AMP0CAL,AMP1CAL,AMP2CAL */

        /* The measurement of the total voltage of the AMPn offset and PGA offset is begun. */
        DSADST = 1U;
        
        /* This waits for the completion of  A/D conversion. */
        while (1U == DSADST)
        {
            /* Do Nothing */
        }
        
        /* This procedure gets a value of A/D conversion. */
        R_PGA_DSAD_Get_Result(&temp_dsad.WORD[1U], &temp_dsad.WORD[0U]);
        ampn_ad_offset = temp_dsad.LONG & 0xFFFFFF00L;
        ampn_ad_offset >>= 8U;


        /* This calculates the AMP0/AMP1/AMP2 offset. */
        offset = ampn_ad_offset - pga_ad_offset;

        /* This confirms that the A/D conversion result is negative value. */
        if (offset < 0)
        {
            *(p_address_ampncal + amp_ch) |= 0x40U;    /* for AMP0CAL,AMP1CAL,AMP2CAL */
        }

        bit_num = 5;
        while (bit_num >= 0)
        {
            /* The bit of AMPnCAL register specified with bit_num is written to 1. */
            *(p_address_ampncal + amp_ch) |= (uint8_t)(1U << bit_num);  /* for AMP0CAL,AMP1CAL,AMP2CAL */

            /* The measurement of the total voltage of the AMPn offset and PGA offset is begun.  */
            DSADST = 1U;

            /* This waits for the completion of  A/D conversion. */
            while (1U == DSADST)
            {
                /* Do Nothing */
            }
            
            /* This procedure gets a value of A/D conversion. */
            R_PGA_DSAD_Get_Result(&temp_dsad.WORD[1U], &temp_dsad.WORD[0U]);
            ampn_ad_offset = temp_dsad.LONG & 0xFFFFFF00L;
            ampn_ad_offset >>= 8U;

            /*  This calculates the AMP0/AMP1/AMP2 offset. */
            offset = ampn_ad_offset - pga_ad_offset;

            /* This confirms that the A/D conversion result is negative value. */
            if (offset < 0)
            {
                /* The bit of AMPnCAL register specified with bit_num is written to 0.  */
                *(p_address_ampncal + amp_ch) &= (uint8_t)~(1U << bit_num); /* for AMP0CAL,AMP1CAL,AMP2CAL */
            }
            bit_num--;
        }
        /* The CONFIGURABLE AMPLIFIER is turned off. */
        *(p_address_ampns1 + amp_ch * 4U) = 0x00U;  /* for AMP0S1,AMP1S1,AMP2S1 */
        *(p_address_ampns2 + amp_ch * 4U) = 0x00U;  /* for AMP0S2,AMP1S2,AMP2S2 */
        
        /* The power supply of AMP0/AMP1/AMP2 is turned off. */
        AFEPWS &= ~ampnpon_reg_value[amp_ch];
    }
    

    /*--------------------------------------------------------------------------------------
     * This procedure recovers the data preserved in the variable to the relating register. 
     *--------------------------------------------------------------------------------------*/
     
    /* A set value of autoscan mode is recovered. */
    DSADCTL  = back_dsadctl;
    
    /* Set values of A/D converting of Multiplexer3 are recovered. */
    PGA3CTL0 = back_pga3ctl0;
    PGA3CTL1 = back_pga3ctl1;
    PGA3CTL2 = back_pga3ctl2;
    PGA3CTL3 = back_pga3ctl3;
    
    /* Set values of DAC are recovered. */
    DACD  = back_dacd;
    DACM0 = back_dacm0;
    DACM1 = back_dacm1;
    DACEN = back_dacen;

    /* Set values of all the channel of CAMP are recovered. */
    for (amp_ch = 0; amp_ch < 3U; amp_ch++)
    {
        *(p_address_ampns0 + amp_ch * 4U) = back_ampns0[amp_ch]; /* for AMP0S0,AMP1S0,AMP2SO */
        *(p_address_ampns1 + amp_ch * 4U) = back_ampns1[amp_ch]; /* for AMP0S1,AMP1S1,AMP2S1 */
        *(p_address_ampns2 + amp_ch * 4U) = back_ampns2[amp_ch]; /* for AMP0S2,AMP1S2,AMP2S2 */
    }
    
    /* An analog front end power supply selection register is recovered. */
    AFEPWS = back_afepws;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
