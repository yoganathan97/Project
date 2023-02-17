/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2011, 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G14 V2.05.05.01 [25 Nov 2020]
* Device(s)    : R5F104ML
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 2/17/2023
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */

#include "LCD1602.h"

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

volatile uint16_t DelayMsCnt = 0;
volatile uint16_t DelayUsCnt = 0;

int i=0, row=0, col=0;

/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

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
    
    lcd_clear();
    lcd_init ();
    lcd_put_cur(0, 1);
    lcd_send_string("Hello! ");
    lcd_send_string("THIS ");
    lcd_send_string("IS ");
    
    lcd_put_cur(1, 2);
    lcd_send_string("YOGANATHAN :)");
    Delay_Ms(3000);
    lcd_clear();

    while (1U)
    {
	    
	  for (i=0;i<128;i++)
	  {
		  lcd_put_cur(row, col);

		  lcd_send_data(i+48);

		  col++;

		  if (col > 15) {row++; col = 0;}
		  if (row > 1) row=0;

		  Delay_Ms(250);
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
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    
    /* Initializing Timers */
    R_TAU0_Channel0_Start();
    R_TAU0_Channel1_Start();	
    
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void Delay_Ms(uint16_t Cnt)
{
    DelayMsCnt = Cnt;
    while(1)
    {
	  if(DelayMsCnt != 0) {
	       __nop();
	  }else {
	       break;
	  }
    }
}


void Delay_Us(uint16_t Cnt)
{
    DelayUsCnt = Cnt;
    while(1)
    {
	  if(DelayUsCnt != 0) {
	       __nop();
	  }else {
	       break;
	  }
    }
}

/* End user code. Do not edit comment generated here */
