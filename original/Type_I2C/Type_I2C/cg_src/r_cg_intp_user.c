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
* File Name    : r_cg_intp_user.c
* Version      : Code Generator for RL78/I1E V1.02.01.02 [24 Feb 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for INTP module.
* Creation Date: 2017/06/14
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_intp.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/

#pragma interrupt r_intc5_interrupt(vect=INTP5)

#pragma interrupt r_intc6_interrupt(vect=INTP6)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern uint8_t i2c_state;
extern uint8_t i2c_dat[20];
extern uint8_t i2c_senddat[20];
extern uint8_t rcvlength;
extern uint8_t datcnt;
uint8_t MatchAddress=0;
uint8_t match=0;
extern uint8_t bitcnt;
extern uint8_t i2c_RW;
extern uint8_t i2c_address;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_intc5_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_intc5_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	//P14(intp5)=SDA  ,  P15(intp6)=SCL
	
	static int8_t PSCL,PDAT;
	PSCL=P1&0x20;
	PDAT=P1&0x10;
	if(PSCL!=0){
		switch(PDAT){
			case 0x00:
				i2c_state=1;
				datcnt=0;
				rcvlength=0;
				bitcnt=0;
				i2c_dat[0]=0;
				i2c_RW=1;
				MatchAddress=0;
				match=0;
			
			    EGN0 = 0x20;//FALLING_BOTH;
			    EGP0 = 0x60;//RISING_BOTH;
				break;
			case 0x10:
				i2c_state=2;
				if(i2c_RW==1)
					rcvlength=datcnt;
			    EGN0 = 0x60;//FALLING_BOTH;
			    EGP0 = 0x20;//RISING_BOTH;							
				match=0;
				break;
			}
	}		
	
	
	
	
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_intc6_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_intc6_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	
	
	
	
	static int8_t PDAT;
	
	PDAT=(P1&0x10);

	
	if(PDAT==0x10 && i2c_RW==1 && bitcnt<8){
		 i2c_dat[datcnt]|=1<<(7-bitcnt);
	}
	else if(i2c_RW==0 && bitcnt<7){
		if((i2c_senddat[datcnt-1]&(1<<(6-bitcnt)))==0){
			P1=P1&0xEF;		
		}
		else{
			P1=P1|0x10;
		}
	}
	bitcnt++;

	
	switch(bitcnt){
		case 8:
		
		switch(i2c_RW){
			case 1://if(i2c_RW==1){
			    EGN0 = 0x60;//FALLING_BOTH;
			    EGP0 = 0x20;//RISING_BOTH;			
				break;//}
			case 0://else if(i2c_RW==0){
				P1  &=0b11101111;		
				NOP();
				PM1 |=0b00010000;//SDA = input
			    EGN0 = 0x20;//FALLING_BOTH;
			    EGP0 = 0x60;//RISING_BOTH;		
				break;//}
		}
		break;
		case 9:
			if(i2c_RW==1){
				if(i2c_state==1 && (((i2c_dat[0]&0xFE) == (i2c_address<<1))||i2c_dat[0]==0)){
					P1  &=0b11101111;
					NOP();
					PM1 &=0b11101111;	//SDA = output
				}
			}
		else if(i2c_RW==0 && PDAT==0x10){
		    EGN0 = 0x20;//FALLING_BOTH;
		    EGP0 = 0x60;//RISING_BOTH;
				if(datcnt==9){
					i2c_state=2;
				}

			}
		else if(i2c_RW==0 && PDAT==0x00){
		    EGN0 = 0x60;//FALLING_BOTH;
		    EGP0 = 0x20;//RISING_BOTH;
				if(datcnt==9){
					i2c_state=2;
				}
		}
		break;
		case 10:
		if(i2c_RW==1){
			PM1 |=0b00010000;//SDA = input
		    EGN0 = 0x20;//FALLING_BOTH;
		    EGP0 = 0x60;//RISING_BOTH;
			
		}
		else if(i2c_RW==0 && EGP0 == 0x60 ){
			PM1 |=0b00010000;//SDA = input
		}
		else if(i2c_RW==0 && EGP0 == 0x20 ){
			PM1 &=0b11101111;	//SDA = output			
				if((i2c_senddat[datcnt]&(1<<7))==0){
					P1=P1&0xEF;
				}
				else{
					P1=P1|0x10;
				}			
		}

		datcnt++;
		if(datcnt>9){
				i2c_state=2;
				if(i2c_RW==1)
					rcvlength=datcnt;			
				P1=P1|0x30;
				NOP();
				PM1 |=0b00110000;//SDA = input
			    EGN0 = 0x60;//FALLING_BOTH;
			    EGP0 = 0x20;//RISING_BOTH;
				datcnt=0;
		}
		i2c_dat[datcnt]=0;
		bitcnt=0;	
		if(i2c_state==1&&datcnt==1 && (((i2c_dat[0]&0xFE) == (i2c_address<<1))||i2c_dat[0]==0)){
			match=1;
			if((i2c_dat[0]&0x01)==0 ){	//i2c_Read Mode
				i2c_RW=1;
			}
			else{						//i2c_Write Mode
				i2c_RW=0;
				PM1 &=0b1111101111;	//SDA = output
			    EGN0 = 0x60;//FALLING_BOTH;
			    EGP0 = 0x20;//RISING_BOTH;
				if((i2c_senddat[datcnt-1]&(1<<7))==0){
					P1=P1&0xEF;
				}
				else{
					P1=P1|0x10;
				}
			}
		}
		else if(datcnt==1){
			i2c_RW=0;
			i2c_state=2;
				PM1 |=0b00110000;//SDA = input			
	
		}
		break;
	}

	
	
	
	
	
	
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
