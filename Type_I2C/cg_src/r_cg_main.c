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
* File Name    : r_cg_main.c
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 27/07/2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_tau.h"
#include "r_cg_it.h"
#include "r_cg_pga_dsad.h"
#include "r_cg_camp.h"
#include "r_cg_dac.h"
#include "r_cg_intp.h"
/* Start user code for include. Do not edit comment generated here */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pfdl.h"
#include "pfdl_types.h"
//#include "fdl.h"                    /* FDL library header file               */
//#include "fdl_types.h"              /* FDL types definition header file      */
//#include "fdl_descriptor.h"         /* FDL descriptor header file            */
//#include "eel.h"                    /* EEL library header file               */
//#include "eel_types.h"              /* EEL types definition header file      */
//#include "eel_descriptor.h"         /* EEL descriptor header file            */
//#include "eel_user_types.h"         /* EEL user types definition header file */

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
uint16_t TxMode =0U;
MD_STATUS g_uart0_tx_end = 0U;         /* UART0 transmission end */
uint32_t dsad_end=0;
union dsad val_dsad[4];//ADC data
int32_t dsad_ival[4],dsad_ofs[4];//符号有時はint32_t
int32_t dsad_log[4][30];//ADC datalog;
int32_t adave[4];//ADC average data;
int32_t coef[3][4]={{0x8000+1000,0x8000,0x8000,0x8000},{0x8000,0x8000+1000,0x8000,0x8000},{0x8000,0x8000,0x8000+1000,0x8000}};//ADCdat to force Coefficients
uint8_t coefbuffer[24];
union ui32_8 coeff[3][4];
int32_t output[4]={0};//Fx,Fy,Fz,T
double dsad_dval[4];//delta sigma data
uint16_t ofsdac[4];//offset data
int ch_dsad;// delta sigma adc channel(for data sampling)
volatile int w_count;//wait counter
char test[200];//Send data buffer(for uatt debug)
uint8_t RxDat[20]={0};//Received data buffer(for uart)
uint8_t RxEnd=0U;//Received flag(for uart)
uint8_t GetOfs=0U;// Enable offset reset flag
uint8_t rcvlength=0;//received data length
uint8_t outdatatype=0; // Datatype:0=sensor data
uint8_t tempcoef=0;
volatile uint8_t slave_is_receiving=0;
volatile uint8_t datcnt=0;
volatile uint8_t bitcnt=0;
//PDFL
__near pfdl_descriptor_t pdfldesc;
pfdl_request_t  pfdlreq;
//__near pfdl_request_t pfdlwrite_i2c_address;//write_i2c_address
//__near pfdl_request_t pfdlread_i2c_address;//read_i2c_address;
//__near pfdl_request_t pfdlread_coef;//read_coef
//__near pfdl_request_t pfdlwrite_coef;//write_coef
pfdl_u08 PFDLstatus=0;




//#define SAM_WDT_RESET() WDTE = 0xAC /* Declare watchdog timer reset function */
//#define SAM_WRITE_SIZE 10           /* Declare write data size */

/* Definition of variables */
//eel_request_t   dtyEelReq;
//eel_u08         dubWriteBuffer[ SAM_WRITE_SIZE ] = {0,1,2,3,4,5,6,7,8,9};
//eel_u08         dubReadBuffer[ SAM_WRITE_SIZE ];
//fdl_status_t    fdlStatus = 0; 
//eel_u08         err_flag = 0;


uint16_t addresstest[2][4][4];



extern uint8_t MatchAddress;
#define avemax 10
#define	Divval 0x400000
static void GetOffset(void);
void PFDL_Write(pfdl_u16 , __near pfdl_u08*,pfdl_u16 );
void PFDL_Read(pfdl_u16 , __near pfdl_u08*,pfdl_u16 );
void PFDL_BlockErase(pfdl_u16 w_index);
//I2C
volatile uint8_t i2c_state=2;
uint8_t i2c_dat[20]={0};
uint8_t i2c_senddat[20]={0};

//PPS Hack
volatile uint8_t i2c_address=I2C_DEFAULT_ADDRESS;
 
typedef union {
 unsigned char ByteMem;
 struct
 {
     unsigned char no0:1;
     unsigned char no1:1;
     unsigned char no2:1;
     unsigned char no3:1;
     unsigned char no4:1;
     unsigned char no5:1;
     unsigned char no6:1;
     unsigned char no7:1;
 } BitfMem;
}Byte_Bitf_Member;

volatile Byte_Bitf_Member data;







/* End user code. Do not edit comment generated here */

static void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
	while (1U)
    {
/*使用機能、⊿Σアンプ、DAC（出力端子：AMP0)、通信(ソフトウェアI2C)*/
	static int cnt_ave,ch_ave,i,j;

	for(ch_ave=0;ch_ave<4;ch_ave++){
		switch(ch_ave){
			case 0:
				DSADCTL =0x1E;
			break;
			case 1:
				DSADCTL =0x1D;
			break;
			case 2:
				DSADCTL =0x1B;
			break;
			case 3:
				DSADCTL =0x17;
			break;
		}
		//DAC Control
		R_DAC_Change_OutputVoltage(ofsdac[ch_ave]);
	    for (w_count = 0U; w_count <= DA_WAITINGTIME+500; w_count++)
	    {
	        NOP();
	    }		
		
		//Delta Sigma ADC Control

		dsad_end=0U;
		
		R_PGA_DSAD_Start(); 
		while((DSADCTL&0x80)!=0U);
 		R_PGA_DSAD_Get_AverageResult(&val_dsad[ch_ave].bit.h, &val_dsad[ch_ave].bit.l);		
		adave[ch_ave]=val_dsad[ch_ave].sval/avemax;
		dsad_log[ch_ave][0]=val_dsad[ch_ave].sval/avemax;//(int32_t)dsad_ival[ch_ave]/avemax;
		for(cnt_ave=avemax-1;cnt_ave>0;cnt_ave--){
			dsad_log[ch_ave][cnt_ave]=(int32_t)dsad_log[ch_ave][cnt_ave-1];
			adave[ch_ave]+=dsad_log[ch_ave][cnt_ave];
		}
	}

	adave[0]=(adave[0]/Divval-dsad_ofs[0]);
	adave[1]=(adave[1]/Divval-dsad_ofs[1]);
	adave[2]=(adave[2]/Divval-dsad_ofs[2]);
	adave[3]=(adave[3]/Divval-dsad_ofs[3]);
	adave[0]=adave[0]-(coef[0][3]-0x8000)*adave[3]/1000;
	adave[1]=adave[1]-(coef[1][3]-0x8000)*adave[3]/1000;
	adave[2]=adave[2]-(coef[2][3]-0x8000)*adave[3]/1000;
	output[0]=-((coef[0][0]-0x8000)*adave[0]+(coef[0][1]-0x8000)*adave[1]+(coef[0][2]-0x8000)*adave[2])/1000+512;
	output[1]=-((coef[1][0]-0x8000)*adave[0]+(coef[1][1]-0x8000)*adave[1]+(coef[1][2]-0x8000)*adave[2])/1000+512;
	output[2]=((coef[2][0]-0x8000)*adave[0]+(coef[2][1]-0x8000)*adave[1]+(coef[2][2]-0x8000)*adave[2])/1000+512;
	output[3]=adave[3]+512;
	
	for(ch_ave=0;ch_ave<4;ch_ave++){
			if(output[ch_ave]<0)
				output[ch_ave]=0;
			else if(output[ch_ave]>1023)
				output[ch_ave]=1023;
	}
//for UART debug	
//	sprintf(test,"%02X,%02X,%02X,%02X,%02X,%02X,%02X\r\n",i2c_state,slave_is_receiving,datcnt,bitcnt,i2c_dat[0],P1,PM1);
//	g_uart0_tx_end =R_UART0_Send((uint8_t *)test,strlen(test));
//	while (g_uart0_tx_end==MD_OK)
//	{
//		NOP();
//	}	

	if(i2c_state!=1){
		if(outdatatype==0){
			i2c_senddat[0]=0x08;
			for(i=0;i<4;i++){
				i2c_senddat[i*2+1]=(output[i]>>8)&0xFF;
				i2c_senddat[i*2+2]=(output[i]&0xFF);
			}
		}
		if(rcvlength > 2 && i2c_dat[0]==0x00 && i2c_dat[1]==0x85){	// Change I2C address
			i2c_address=i2c_dat[2];
			if(i2c_address>=0x7f)
				i2c_address=0x7f;
			for(i=0;i<3;i++){
				for(j=0;j<4;j++){
					coefbuffer[8*i+j*2]=0x00FF&(coef[i][j]>>8);
					coefbuffer[8*i+j*2+1]=0x00FF&(coef[i][j]>>8);
				}
			}
			
			PFDLstatus=PFDL_Open(&pdfldesc);		  
			while(0x00!=PFDLstatus){
				PFDLstatus=PFDL_Handler();
			}
			PFDL_BlockErase(0);
			PFDL_Write(1, &i2c_address,1 );
			PFDL_Write(2, &coefbuffer[0],24 );		
			PFDL_Close();

			i2c_dat[1]=0x00 ;
			i2c_dat[2]=0x00 ;
			i2c_dat[3]=0x00 ;
			i2c_dat[4]=0x00 ;
			i2c_dat[5]=0x00 ;
			i2c_dat[6]=0x00 ;
			rcvlength=0;
		}
		
		else if(rcvlength > 6 && ((i2c_dat[0] & 0xFE) == (i2c_address<<1))){ 	//	change coefficients
			if(i2c_dat[1]==0x00 && i2c_dat[2]==0x02 && i2c_dat[3]==0x00 && i2c_dat[4]==0x05 && i2c_dat[5]==0x00 && i2c_dat[6]==0){
				GetOfs=1U;
			}
			else if(i2c_dat[1]==0x00 && i2c_dat[2]==0x01){
				if(i2c_dat[3]<=2 && i2c_dat[4] <= 3){
					coef[i2c_dat[3]][i2c_dat[4]]=0xFFFF&((i2c_dat[5]<<8)+i2c_dat[6]);

					for(i=0;i<3;i++){
						for(j=0;j<4;j++){
							coefbuffer[8*i+j*2]=0x00FF&(coef[i][j]>>8);
							coefbuffer[8*i+j*2+1]=0x00FF&(coef[i][j]>>8);
						}
					}
					
					PFDLstatus=PFDL_Open(&pdfldesc);		  
					while(0x00!=PFDLstatus){
						PFDLstatus=PFDL_Handler();
					}
					PFDL_BlockErase(0);
					PFDL_Write(1, &i2c_address,1 );
					PFDL_Write(2, &coefbuffer[0],24 );		
					PFDL_Close();
				}
			}
			i2c_dat[0]=0x00	;
			i2c_dat[1]=0x00 ;
			i2c_dat[2]=0x00 ;
			i2c_dat[3]=0x00 ;
			i2c_dat[4]=0x00 ;
			i2c_dat[5]=0x00 ;
			i2c_dat[6]=0x00 ;
			rcvlength=0;
		}
		if(GetOfs==1U){
			GetOffset();
			GetOfs=0U;
		}	
	}



    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
	uint8_t i,j;
		int cnt=0;

	pfdl_u08 temp;
	//initialize
    	EI();
	R_CAMP0_Start();// setup_amp0(DAC output)
	R_DAC_Start();// start_DAC

	
	//initialize pdfl structure
	pdfldesc.fx_MHz_u08=32;
	pdfldesc.wide_voltage_mode_u08=1U;
	
	//Load i2c address and coefficients
	PFDLstatus=PFDL_Open(&pdfldesc);		  
	while(0x00!=PFDLstatus){
		PFDLstatus=PFDL_Handler();
	}
	PFDL_Read(1, &i2c_address,1 );	
	// PPS hack
	if(i2c_address == 0x01)
		i2c_address = I2C_DEFAULT_ADDRESS;
	
	PFDL_Read(2, &coefbuffer[0],24 );
	PFDL_Close();		
	if(i2c_address==0xFF){
		//PPS Hack
		i2c_address=I2C_DEFAULT_ADDRESS;
		coef[0][0]=0x83e8;
		coef[1][1]=0x83e8;
		coef[2][2]=0x83e8;
		
		coef[0][1]=0x8000;
		coef[0][2]=0x8000;
		coef[0][3]=0x8000;
		
		coef[1][0]=0x8000;
		coef[1][2]=0x8000;
		coef[1][3]=0x8000;
		
		coef[2][0]=0x8000;
		coef[2][1]=0x8000;
		coef[2][3]=0x8000;
		for(i=0;i<3;i++){
			for(j=0;j<4;j++){
				coefbuffer[8*i+j*2]=0x00FF&(coef[i][j]>>8);
				coefbuffer[8*i+j*2+1]=0x00FF&(coef[i][j]>>8);
			}
		}
		PFDLstatus=PFDL_Open(&pdfldesc);		  
		while(0x00!=PFDLstatus){
			PFDLstatus=PFDL_Handler();
		}
		PFDL_BlockErase(0);
		PFDL_Write(1, &i2c_address,1 );
		PFDL_Write(2, &coefbuffer[0],24 );		
		PFDL_Close();
		while(0x00!=PFDLstatus){
			PFDLstatus=PFDL_Handler();
		}

		
	}
	
	for(i=0;i<3;i++){
		for(j=0;j<4;j++){
			coef[i][j]=0xFFFF&((coefbuffer[8*i+j*2]<<8)+coefbuffer[8*i+j*2+1]);
		}
	}
	
	R_INTC_Create();// setup interrupts
	R_INTC6_Start();// setup interrupts intc6 
	R_INTC5_Start();// setup interrupts intc5


  	POM1 = _20_POMn5_NCH_ON | _10_POMn4_NCH_ON ;//_04_POMn2_NCH_ON | _02_POMn1_NCH_ON ;// setting P1 Nch outputmode
 	PM1  = _80_PMn7_MODE_UNUSED  | _40_PMn6_MODE_UNUSED | _20_PMn5_MODE_INPUT  | _10_PMn4_MODE_INPUT | 
           _00_PMn3_MODE_OUTPUT  | _04_PMn2_MODE_UNUSED | _02_PMn1_MODE_UNUSED | _01_PMn0_MODE_UNUSED;// setting P1 mode 	
		  
	PGA0CTL2=0;
	PGA1CTL2=0;
	PGA2CTL2=0;
	PGA3CTL2=0;

 //  R_UART0_Start();//setup UART0
	GetOffset();
				
	


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

///////////////////////////////////////////////////////////////////////// GetOffset function ///////////////
static void GetOffset(void){
	uint16_t setdac=0;
	int8_t setcnt=0;
	int8_t setch=0;
	int8_t ofs_ctl1[3]={0};

//#define testdisp

	/***initializing offset******/	
	PGA0CTL1=PGA0CTL1&0xC0;
	PGA1CTL1=PGA1CTL1&0xC0;
	PGA2CTL1=PGA2CTL1&0xC0;
	/************************/

	/****Set DAC **********/
	R_DAC_Change_OutputVoltage(1240);
    for (w_count = 0U; w_count <= DA_WAITINGTIME+5000; w_count++)
    {
        NOP();
    }
	
/***********Gain changeing for offset control**********/	
	PGA0CTL0=PGA0CTL0&0xE0;
	PGA1CTL0=PGA1CTL0&0xE0;
	PGA2CTL0=PGA2CTL0&0xE0;
	
	PGA0CTL0=PGA0CTL0|0x10;
	PGA1CTL0=PGA1CTL0|0x10;
	PGA2CTL0=PGA2CTL0|0x10;
/************************************************/
	
		
	for(setch=0;setch<3;setch++){
		ofsdac[setch]=0;
		switch(setch){
			case 0:
				DSADCTL =0x1E;
			break;
			case 1:
				DSADCTL =0x1D;
			break;
			case 2:
				DSADCTL =0x1B;
			break;
			case 3:
				DSADCTL =0x17;
			break;
		}
		for(setcnt=11;setcnt>=0;setcnt--){
			ofsdac[setch]=ofsdac[setch]|(1<<setcnt);
			R_DAC_Change_OutputVoltage(ofsdac[setch]);
		    for (w_count = 0U; w_count <= DA_WAITINGTIME+5000; w_count++)
		    {
		        NOP();
		    }		
			dsad_end=0;
		//Delta Sigma ADC Control
		dsad_end=0U;

		R_PGA_DSAD_Start(); 
		while((DSADCTL&0x80)!=0U);
 		R_PGA_DSAD_Get_AverageResult(&val_dsad[setch].bit.h, &val_dsad[setch].bit.l);		
		dsad_ival[setch]=val_dsad[setch].sval;
#ifdef testdisp
			sprintf(test,"%u %u %8ld %u\r\n",setch,setcnt,dsad_ival[setch],DACD);
			g_uart0_tx_end =R_UART0_Send((uint8_t *)test,sizeof(test));
			while (g_uart0_tx_end==MD_OK)
			{
				NOP();
			}	
#endif
			if(dsad_ival[setch]<0){
				ofsdac[setch]=ofsdac[setch]-(1<<setcnt);
			}
		}
	}

	setdac=(ofsdac[0]+ofsdac[1]+ofsdac[2])/3;
	R_DAC_Change_OutputVoltage(setdac);
	dsad_end=0;
	R_PGA_DSAD_Start();
	while((DSADCTL&0x80)!=0U);
	

///////////////Gain setting/////////
	PGA0CTL0=PGA0CTL0&0xE0;
	PGA1CTL0=PGA1CTL0&0xE0;
	PGA2CTL0=PGA2CTL0&0xE0;

//64
/**
	PGA0CTL0=PGA0CTL0|0x13;
	PGA1CTL0=PGA1CTL0|0x13;
	PGA2CTL0=PGA2CTL0|0x13;
/**/
//32
/**/
	PGA0CTL0=PGA0CTL0|0x12;
	PGA1CTL0=PGA1CTL0|0x12;
	PGA2CTL0=PGA2CTL0|0x12;
/**/
//16
/**/
//	PGA0CTL0=PGA0CTL0|0x11;
//	PGA1CTL0=PGA1CTL0|0x11;
//	PGA2CTL0=PGA2CTL0|0x11;
/**/
///////////////////////////////////

	for(setch=0;setch<4;setch++){
		ofs_ctl1[setch]=0;
		switch(setch){
			case 0:
				DSADCTL =0x1E;
			break;
			case 1:
				DSADCTL =0x1D;
			break;
			case 2:
				DSADCTL =0x1B;
			break;
			case 3:
				DSADCTL =0x17;
			break;
		}	
		
		for(setcnt=4;setcnt>=0;setcnt--){
			ofs_ctl1[setch]=ofs_ctl1[setch]|(1<<setcnt);
			
			switch(setch){
				case 0:PGA0CTL1=PGA0CTL1&0xE0;PGA0CTL1=PGA0CTL1|ofs_ctl1[setch];break;
				case 1:PGA1CTL1=PGA1CTL1&0xE0;PGA1CTL1=PGA1CTL1|ofs_ctl1[setch];break;
				case 2:PGA2CTL1=PGA2CTL1&0xE0;PGA2CTL1=PGA2CTL1|ofs_ctl1[setch];break;
			}

			for (w_count = 0U; w_count <= DA_WAITINGTIME+2000; w_count++)
		    {
		        NOP();
		    }		
			dsad_end=0;
//			R_PGA_DSAD_Start();
//			while(dsad_end==0U);
//			R_PGA_DSAD_Start();
//			while(dsad_end==0U);
//			sprintf(test,"%u %u %8ld %u\r\n",setch,setcnt,dsad_ival[setch],ofs_ctl1[setch]);
//			g_uart0_tx_end =R_UART0_Send((uint8_t *)test,sizeof(test));
//			while (g_uart0_tx_end==MD_OK)
//			{
//				NOP();
//			}
		R_PGA_DSAD_Start(); 
		while((DSADCTL&0x80)!=0U);
 		R_PGA_DSAD_Get_AverageResult(&val_dsad[setch].bit.h, &val_dsad[setch].bit.l);
		dsad_ival[setch]=val_dsad[setch].sval;
#ifdef testdisp
			sprintf(test,"%u %u %8ld %u\r\n",setch,setcnt,dsad_ival[setch],DACD);
			g_uart0_tx_end =R_UART0_Send((uint8_t *)test,sizeof(test));
			while (g_uart0_tx_end==MD_OK)
			{
				NOP();
			}	
#endif
			if(dsad_ival[setch]>0){
				ofs_ctl1[setch]=ofs_ctl1[setch]-(1<<setcnt);
			}
		}
		switch(setch){
			case 0:PGA0CTL1=PGA0CTL1&0xE0;PGA0CTL1=PGA0CTL1|ofs_ctl1[setch];break;
			case 1:PGA1CTL1=PGA0CTL1&0xE0;PGA1CTL1=PGA1CTL2|ofs_ctl1[setch];break;
			case 2:PGA2CTL1=PGA0CTL1&0xE0;PGA2CTL1=PGA2CTL2|ofs_ctl1[setch];break;
		}		
	}
/**/







	for(setch=0;setch<4;setch++){
		ofsdac[setch]=0;
		switch(setch){
			case 0:
				DSADCTL =0x1E;
			break;
			case 1:
				DSADCTL =0x1D;
			break;
			case 2:
				DSADCTL =0x1B;
			break;
			case 3:
				DSADCTL =0x17;
			break;
		}			
		for(setcnt=11;setcnt>=0;setcnt--){
			ofsdac[setch]=ofsdac[setch]|(1<<setcnt);
			R_DAC_Change_OutputVoltage(ofsdac[setch]);
		    for (w_count = 0U; w_count <= DA_WAITINGTIME+500; w_count++)
		    {
		        NOP();
		    }		
			dsad_end=0;
		R_PGA_DSAD_Start(); 
		while((DSADCTL&0x80)!=0U);
 		R_PGA_DSAD_Get_AverageResult(&val_dsad[setch].bit.h, &val_dsad[setch].bit.l);		
		dsad_ival[setch]=val_dsad[setch].sval;
#ifdef testdisp
			sprintf(test,"%u %u %8ld %u\r\n",setch,setcnt,dsad_ival[setch],DACD);
			g_uart0_tx_end =R_UART0_Send((uint8_t *)test,sizeof(test));
			while (g_uart0_tx_end==MD_OK)
			{
				NOP();
			}	
#endif
			if(dsad_ival[setch]<0){
				ofsdac[setch]=ofsdac[setch]-(1<<setcnt);
			}
		}
	}
	
	
	for(setch=0;setch<4;setch++){
		
		switch(setch){
			case 0:
				DSADCTL =0x1E;
			break;
			case 1:
				DSADCTL =0x1D;
			break;
			case 2:
				DSADCTL =0x1B;
			break;
			case 3:
				DSADCTL =0x17;
			break;
		}
		//DAC Control
		R_DAC_Change_OutputVoltage(ofsdac[setch]);
	    for (w_count = 0U; w_count <= DA_WAITINGTIME+500; w_count++)
	    {
	        NOP();
	    }		
			dsad_end=0;
			R_PGA_DSAD_Start(); 
			while((DSADCTL&0x80)!=0U);
	 		R_PGA_DSAD_Get_AverageResult(&val_dsad[setch].bit.h, &val_dsad[setch].bit.l);		
			dsad_ival[setch]=val_dsad[setch].sval;	
		}
dsad_ofs[0]=dsad_ival[0]/Divval;
dsad_ofs[2]=dsad_ival[2]/Divval;
dsad_ofs[3]=dsad_ival[3]/Divval;
dsad_ofs[1]=dsad_ival[1]/Divval;	
	

	
	


//#ifdef testdisp
//	while(1);
//#endif	
	
}


void PFDL_Write(pfdl_u16 w_index, __near pfdl_u08*  w_pointer,pfdl_u16 w_bytes ){
	//Write//////////////////////////////////////////////////
	pfdlreq.index_u16=w_index;
	pfdlreq.data_pu08=w_pointer;
	pfdlreq.bytecount_u16=w_bytes;
	pfdlreq.command_enu=PFDL_CMD_WRITE_BYTES;	

	PFDLstatus=PFDL_Execute(&pfdlreq);	
	while(0x00!=PFDLstatus){
		PFDLstatus=PFDL_Handler();
	}
	//////////////////////////////////////////////////////////		
}

void PFDL_Read(pfdl_u16 w_index, __near pfdl_u08*  w_pointer,pfdl_u16 w_bytes ){
	pfdlreq.index_u16=w_index;
	pfdlreq.data_pu08=w_pointer;
	pfdlreq.bytecount_u16=w_bytes;
	pfdlreq.command_enu=PFDL_CMD_READ_BYTES;	
	PFDLstatus=PFDL_Execute(&pfdlreq);	
	while(0x00!=PFDLstatus){
		PFDLstatus=PFDL_Handler();
	}
}
void PFDL_BlockErase(pfdl_u16 w_index){
	pfdlreq.index_u16=w_index;
	pfdlreq.data_pu08=0;
	pfdlreq.bytecount_u16=0;
	pfdlreq.command_enu=PFDL_CMD_ERASE_BLOCK;
	PFDLstatus=PFDL_Execute(&pfdlreq);	
	while(0x00!=PFDLstatus){
		PFDLstatus=PFDL_Handler();
	}
}




/* End user code. Do not edit comment generated here */
