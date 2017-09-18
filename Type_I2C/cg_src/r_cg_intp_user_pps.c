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

// int5 = SDA int
// int6 = SCL int
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

	// start condition: data falling when clock is high
	// stop condition:  data rising  when clock is high
	
	//SDA data line interrupt - on every falling & rising edge
	
	static int8_t PSCL,PDAT;
	PSCL=P1&0x20;									//PSCL == Port1.bit5
	PDAT=P1&0x10;									//PSCL == Port1.bit4		
	if(PSCL!=0){									//on a data transition && clock is high
		switch(PDAT){
			case 0x00:								//	clock is high when data went low: START condition
				i2c_state=1;						//		state => 1
				datcnt=0;							//		zero all counts
				rcvlength=0;
				bitcnt=0;
				i2c_dat[0]=0;
				i2c_RW=1;							// 		default to read
				MatchAddress=0;						//		
				match=0;
			
			    EGN0 = 0x20;//FALLING_BOTH;			//ENSURE NO INT ON FALLING CLOCK - External interrupt falling edge enable
			    EGP0 = 0x60;//RISING_BOTH;
				break;
			case 0x10:								//	clock is high when data went high ... think this is actually a STOP confdition?????
				i2c_state=2;						//		state => 2
				if(i2c_RW==1)						// if receiving ....
					rcvlength=datcnt;
			    EGN0 = 0x60;//FALLING_BOTH;			
			    EGP0 = 0x20;//RISING_BOTH;			//ENSURE NO INT ON RISING CLOCK				
				match=0;
				break;
			}
	}
//	else 
//	{ 
//       clock is low = normal for data to transition while clock is low 
//  }
	

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

	//SCL clock line interrupt - on rising or falling edges	- this is going to increment the bit count
	
	static int8_t PDAT;											 
	
	PDAT=(P1&0x10);												//poll the data

//Rx / TX data bits	
	if(PDAT==0x10 && i2c_RW==1 && bitcnt<8){					//data is high
		 i2c_dat[datcnt]|=1<<(7-bitcnt);						// if Rx set a 1 in the current byte
	}
	else if(i2c_RW==0 && bitcnt<7){								
		if((i2c_senddat[datcnt-1]&(1<<(6-bitcnt)))==0){
			P1=P1&0xEF;											// send a 0		
		}
		else{
			P1=P1|0x10;											// send a 1
		}
	}
	bitcnt++;													// in Rx this effectively sets a 0 in the current byte if dat is 0

	
	switch(bitcnt){
		case 8:													
		switch(i2c_RW){
			case 1://if(i2c_RW==1){
			    EGN0 = 0x60;//FALLING_BOTH;						//
			    EGP0 = 0x20;//RISING_BOTH;						// rising data only???			
				break;//}
			case 0://else if(i2c_RW==0){						// master transmitting
				P1  &=0b11101111;								// SDA = 0		
				NOP();
				PM1 |=0b00010000;//SDA = input					//???????
			    EGN0 = 0x20;//FALLING_BOTH;						//ENSURE NO INT ON FALLING CLOCK	
			    EGP0 = 0x60;//RISING_BOTH;
				break;//}
		}
		break;
		
		case 9:													
			if(i2c_RW==1)							// master reading, slave acks the data ...... 
			{										// if state = 1  and master reading from slave
				if( i2c_state == 1 
				&& ( ((i2c_dat[0]&0xFE) == (i2c_address<<1)) || (i2c_dat[0] == 0)))  //on our address or address 0
				{//0xFE test R_W bit of address byte?
					P1  &=0b11101111;							// ack
					NOP();											
					PM1 &=0b11101111;	//SDA = output			// output ack
				}
			}
			else if(i2c_RW==0 && PDAT==0x10)		//master writing, data bit = 1, master acks data - master sending RW = 1?
			{
				EGN0 = 0x20;//FALLING_BOTH;						//ENSURE NO INT ON FALLING CLOCK
				EGP0 = 0x60;//RISING_BOTH;
				if(datcnt==9){
					i2c_state = 2;								// after all data written
				}
			}
			else if(i2c_RW==0 && PDAT==0x00)		//master writing, data bit = 0, master acks data - master sending RW = 0?
			{
				EGN0 = 0x60;//FALLING_BOTH;
				EGP0 = 0x20;//RISING_BOTH;						//ENSURE NO INT ON RISING CLOCK
				if(datcnt==9){
					i2c_state=2;								// after all data written
				}
		}
		break;

		case 10:												//clears the ACK on next clock edge - not really a bit count
		if(i2c_RW==1){
			PM1 |=0b00010000;//SDA = input
		    EGN0 = 0x20;//FALLING_BOTH;								//ENSURE NO INT ON FALLING CLOCK
		    EGP0 = 0x60;//RISING_BOTH;
			
		}
		else if(i2c_RW==0 && EGP0 == 0x60 ){
			PM1 |=0b00010000;//SDA = input
		}
		else if(i2c_RW==0 && EGP0 == 0x20 ){
			PM1 &=0b11101111;	//SDA = output			
				if((i2c_senddat[datcnt]&(1<<7)) == 0){
					P1=P1&0xEF;									//clear Tx port = 0 .... ????
				}
				else{
					P1=P1|0x10;									//set Tx port = 1
				}			
		}

		datcnt++;												// byte sent / received
		if(datcnt>9){
				i2c_state=2;
				if(i2c_RW==1)
					rcvlength=datcnt;			
				P1=P1|0x30;
				NOP();
				PM1 |=0b00110000;//SDA = input
			    EGN0 = 0x60;//FALLING_BOTH;
			    EGP0 = 0x20;//RISING_BOTH;							//ENSURE NO INT ON RISING CLOCK							
				
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
			    EGP0 = 0x20;//RISING_BOTH;							//ENSURE NO INT ON RISING CLOCK
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
