/*
 * LCD1602.c
 *
 *  Created on: 17-Feb-2023
 *      Author: Yoganathan V
 */

/* Includes ------------------------------------------------------------------*/
#include "LCD1602.h"
#include "stdint.h"
#include "r_cg_userdefine.h"
#include "iodefine.h"

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
  * @brief send_to_lcd Function will Send data to LCD(parallel).
  * @param  data sending data
  * @param  rs register select output
  * @retval none
  */
void send_to_lcd (char data, int rs)
{
    REG_SELECT	= (rs & 0x01);

    /* write the data to the respective pin */
    DATA_PIN7	= ((data>>3) & 0x01);
    DATA_PIN6	= ((data>>2) & 0x01);
    DATA_PIN5	= ((data>>1) & 0x01);
    DATA_PIN4	= ((data>>0) & 0x01);

    /* Toggle EN PIN to send the data
    * if the HCLK > 100 MHz, use the  20 us delay
    * if the LCD still doesn't work, increase the delay to 50, 80 or 100..
    */
    ENABLE	= 1;
    Delay_Us(100);

    ENABLE	= 0;
    Delay_Us(100);

}

/**
  * @brief lcd_send_cmd Function will Send Command.
  * @param  cmd Display command
  * @retval none
  */
void lcd_send_cmd (char cmd)
{
    char datatosend;

    /* send upper nibble first
    RS Must be LOW while sending Command */
    datatosend = ((cmd>>4)&0x0f);
    send_to_lcd(datatosend,0);

    /* send Lower Nibble 
    RS Must be LOW while sending Command */
    datatosend = ((cmd)&0x0f);
    send_to_lcd(datatosend, 0);
}

/**
  * @brief lcd_send_data Function will Send Data.
  * @param  data Display Data
  * @retval none
  */
void lcd_send_data (char data)
{
    char datatosend;

    /* send higher nibble
    RS Must be HIGH while sending Data */
    datatosend = ((data>>4)&0x0f);
    send_to_lcd(datatosend, 1);

    /* send Lower nibble
    RS Must be HIGH while sending Data */
    datatosend = ((data)&0x0f);
    send_to_lcd(datatosend, 1);
}

/**
  * @brief lcd_clear Function will Clear Display.
  * @param  none
  * @retval none
  */
void lcd_clear (void)
{
    lcd_send_cmd(0x01);
    Delay_Ms(2);
}

/**
  * @brief lcd_put_cur Function will Set the Curser on 16x2 Display.
  * @param  row - Row number
  * @param  col - column number
  * @retval none
  */
void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}


/**
  * @brief lcd_init Function will initialize the LCD.
  * @param  none
  * @retval none
  */
void lcd_init (void)
{
    READ_DATA = 0;

    /* 4 bit initialisation */
    Delay_Ms(50);  /* wait for >40ms */
    lcd_send_cmd (0x30);
    Delay_Ms(5);  /* wait for >4.1ms */
    lcd_send_cmd (0x30);
    Delay_Ms(1);  /* wait for >100us */
    lcd_send_cmd (0x30);
    Delay_Ms(10);

    /* 4bit mode */
    lcd_send_cmd (0x20);
    Delay_Ms(10);

    /* dislay initialisation  */
    /* Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters) */
    lcd_send_cmd (0x28);
    Delay_Ms(1);

    /* Display on/off control --> D=0,C=0, B=0  ---> display off */
    lcd_send_cmd (0x08);
    Delay_Ms(1);

    /* clear display */
    lcd_send_cmd (0x01);
    Delay_Ms(1);
    Delay_Ms(1);

    lcd_send_cmd (0x06);
    Delay_Ms(1);

    /* Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits) */
    lcd_send_cmd (0x0C);
}

/**
  * @brief lcd_send_string Function will Send String data.
  * @param  str display data
  * @retval none
  */
void lcd_send_string (char *str)
{
    while (*str) lcd_send_data (*str++);
}


/*********************************END OF FILE*********************************/