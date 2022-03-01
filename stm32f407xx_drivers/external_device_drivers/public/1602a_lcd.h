/*
 * 1602a_lcd.h
 *
 *  Created on: Nov 30, 2021
 *      Author: John
 */

#ifndef PUBLIC_1602A_LCD_H_
#define PUBLIC_1602A_LCD_H_

#include "stm32f407xx.h"
/*
 * Public APIs
 */
void lcd_clear_display( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0,\
		GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, \
		GPIO_Handle_t *DB6, GPIO_Handle_t *DB7);

void lcd_return_home( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, \
		GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, \
		GPIO_Handle_t *DB6, GPIO_Handle_t *DB7);

void lcd_function_set( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, \
		GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, \
		GPIO_Handle_t *DB6, GPIO_Handle_t *DB7,uint8_t mode);

void lcd_write_string_to_lcd( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, \
		GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, \
		GPIO_Handle_t *DB6, GPIO_Handle_t *DB7, uint8_t * data, uint16_t len);

void lcd_display_status_set( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, \
		GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, \
		GPIO_Handle_t *DB6, GPIO_Handle_t *DB7, uint8_t state);

void lcd_write_character_to_lcd( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, \
		GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, GPIO_Handle_t *DB6, \
		GPIO_Handle_t *DB7,uint8_t data);

uint8_t lcd_check_busy_flag( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB7);

void lcd_set_ddram_address(GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, \
		GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, \
		GPIO_Handle_t *DB6, GPIO_Handle_t *DB7, uint8_t address);

void write_ddram_address_to_pins(GPIO_Handle_t *DB0, GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, \
		GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, GPIO_Handle_t *DB6, uint8_t address);



#endif /* PUBLIC_1602A_LCD_H_ */
