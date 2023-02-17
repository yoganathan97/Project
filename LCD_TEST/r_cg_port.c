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
* File Name    : r_cg_port.c
* Version      : CodeGenerator for RL78/G14 V2.05.05.01 [25 Nov 2020]
* Device(s)    : R5F104ML
* Tool-Chain   : CCRL
* Description  : This file implements device driver for PORT module.
* Creation Date: 2/17/2023
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
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
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
    P1 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn4_OUTPUT_0 |
         _00_Pn5_OUTPUT_0;
    P4 = _00_Pn3_OUTPUT_0;
    P5 = _00_Pn2_OUTPUT_0;
    P11 = _00_Pn0_OUTPUT_0;
    PMC1 = _00_PMCn2_DI_ON | _00_PMCn3_DI_ON | _40_PMCn6_NOT_USE | _80_PMCn7_NOT_USE;
    PM1 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT |
          _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _40_PMn6_NOT_USE | _80_PMn7_NOT_USE;
    PM4 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _00_PMn3_MODE_OUTPUT | _10_PMn4_NOT_USE |
          _20_PMn5_NOT_USE | _C0_PM4_DEFAULT;
    PM5 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _00_PMn2_MODE_OUTPUT | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE |
          _20_PMn5_NOT_USE | _C0_PM5_DEFAULT;
    PM11 = _00_PMn0_MODE_OUTPUT | _02_PMn1_NOT_USE | _FC_PM11_DEFAULT;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
