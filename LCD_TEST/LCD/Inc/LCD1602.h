/*
 * lcd1602.h
 *
 *  Created on: 17-Feb-2023
 *      Author: Yoganathan V
 */

/* Define to prevent recursive inclusion -----------------------------*/
#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/

/* Define ------------------------------------------------------------*/

/* Macro -------------------------------------------------------------*/

/* Typedef -----------------------------------------------------------*/

/* Variables ---------------------------------------------------------*/

/* Function prototypes -----------------------------------------------*/
void lcd_init (void);
void lcd_send_cmd (char cmd);
void lcd_send_data (char data);
void lcd_send_string (char *str);
void lcd_put_cur(int row, int col);
void lcd_clear (void);



#ifdef __cplusplus
}
#endif

#endif /* INC_LCD1602_H_ */
