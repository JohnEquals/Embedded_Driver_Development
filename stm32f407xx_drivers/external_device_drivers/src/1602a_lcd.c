/*
 *
 * 		Hardware Requirements:		1602a LCD module.   8 data pins (D0-D7) connected to 8 GPIO pins on MCU.  Vcc = 5V and GND = 0V connected to VDD/anode and VSS/cathode pins respectively.
 * 									10K pot connected to Vcc and GND and its wiper (middle terminal) to V0.
 *
 * 		Note:  This has dependencies on stm32f407xx_drivers as it calls the GPIO driver APIs to toggle the state of each of the data pins and enable pin.
 *
 *
 */

#include "1602a_lcd.h"
#include "stm32f407xx.h"

#define LOW		(0)
#define HIGH	(1)

#define MODE_8BIT 	8
#define MODE_4BIT 	4

static uint8_t cursor_pos_horiz = 0;
static uint8_t cursor_pos_vert = 0;

static void delay( uint32_t time);
static void pulseEnablePin( GPIO_Handle_t *E );


static void delay( uint32_t time)
{
	for( uint32_t i = 0; i < (time*2000); i++);
}





static void pulseEnablePin( GPIO_Handle_t *E )
{

	GPIO_WriteToOutputPin(E -> pGPIOx, E -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	delay(10);
	GPIO_WriteToOutputPin(E ->pGPIOx, E -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	delay(10);
}

/*
 *
 */


void lcd_clear_display( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, GPIO_Handle_t *DB1, \
		GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, GPIO_Handle_t *DB6, GPIO_Handle_t *DB7)
{
	GPIO_WriteToOutputPin(RS -> pGPIOx, RS -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(RW -> pGPIOx, RW -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB0 -> pGPIOx, DB0 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	GPIO_WriteToOutputPin(DB1 -> pGPIOx, DB1 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB3 -> pGPIOx, DB3 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB4 -> pGPIOx, DB4 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB5 -> pGPIOx, DB5 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB6 -> pGPIOx, DB6 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB7 -> pGPIOx, DB7 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	pulseEnablePin(E);
}

void lcd_return_home( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, GPIO_Handle_t *DB1, \
		GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, GPIO_Handle_t *DB6, GPIO_Handle_t *DB7)
{
	GPIO_WriteToOutputPin(RS -> pGPIOx, RS -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(RW -> pGPIOx, RW -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB1 -> pGPIOx, DB1 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB3 -> pGPIOx, DB3 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB4 -> pGPIOx, DB4 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB5 -> pGPIOx, DB5 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB6 -> pGPIOx, DB6 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB7 -> pGPIOx, DB7 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	pulseEnablePin(E);
}

void lcd_function_set( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, GPIO_Handle_t *DB1, \
		GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, GPIO_Handle_t *DB6, GPIO_Handle_t *DB7,uint8_t mode)
{
	GPIO_WriteToOutputPin(RS -> pGPIOx, RS -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(RW -> pGPIOx, RW -> GPIO_PinConfig.GPIO_PinNumber, LOW);

	GPIO_WriteToOutputPin(DB5 -> pGPIOx, DB5 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	GPIO_WriteToOutputPin(DB6 -> pGPIOx, DB6 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB7 -> pGPIOx, DB7 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	// Font size:  0 = 5x8, 1 = 5x11
	GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	// Number of lines: 0 = 1 line, 1 = 2 line
	GPIO_WriteToOutputPin(DB3 -> pGPIOx, DB3 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	// Interface data: 0 = 4 bits, 1 = 8 bits
	GPIO_WriteToOutputPin(DB4 -> pGPIOx, DB4 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	pulseEnablePin(E);
	delay(5);
}

void lcd_write_string_to_lcd( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, GPIO_Handle_t *DB1, \
		GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, GPIO_Handle_t *DB6, GPIO_Handle_t *DB7, uint8_t * data, uint16_t len)
{
	for(uint16_t i = 0; (i < len) && ( cursor_pos_horiz < 16); i++)
	{
		lcd_write_character_to_lcd(RS, RW, E, DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7, *data++);
		//delay(1);
	}
}

void lcd_display_status_set( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, GPIO_Handle_t *DB1, \
		GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, GPIO_Handle_t *DB6, GPIO_Handle_t *DB7, uint8_t state)
{
	GPIO_WriteToOutputPin(RS -> pGPIOx, RS -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(RW -> pGPIOx, RW -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB4 -> pGPIOx, DB4 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB5 -> pGPIOx, DB5 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB6 -> pGPIOx, DB6 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB7 -> pGPIOx, DB7 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB3 -> pGPIOx, DB3 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	// cursor position:  1: ON, 0: OFF
	GPIO_WriteToOutputPin(DB0 -> pGPIOx, DB0 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	// cursor on: 1:ON, 0: OFF
	GPIO_WriteToOutputPin(DB1 -> pGPIOx, DB1 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	if( state != 0)
	{

		GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	}
	else
	{
		GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	}
	pulseEnablePin(E);
}

void lcd_write_character_to_lcd( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, \
		GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, GPIO_Handle_t *DB6, \
		GPIO_Handle_t *DB7,uint8_t data)
{

	GPIO_Handle_t pins[] = {*DB0, *DB1, *DB2, *DB3, *DB4, *DB5, *DB6, *DB7};
	uint8_t data_size = 8;


	if((cursor_pos_horiz >= 16) && (cursor_pos_vert == 0))
	{
		lcd_set_ddram_address(RS, RW, E, DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7, 0x3F);
		cursor_pos_vert++;
		cursor_pos_horiz = 0;
	}
	else if((cursor_pos_horiz >= 16) && (cursor_pos_vert == 1))
	{
		lcd_clear_display(RS, RW, E, DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7);
		lcd_set_ddram_address(RS, RW, E, DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7, 0x00);
		cursor_pos_horiz = 0;
		cursor_pos_vert = 0;
	}
	//else
	//{
	//	lcd_set_cgram_address(RS, RW, E, DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7);
	//	cursor_pos = 1;
	//	delay(1);
	//}
	// set LCD to data input mode so we can write to LCD
	GPIO_WriteToOutputPin(RS -> pGPIOx, RS -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	// set LCD to write mode
	GPIO_WriteToOutputPin(RW -> pGPIOx, RW -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	/*
	 * manual way to set data for each pin
	GPIO_WriteToOutputPin(DB0 -> pGPIOx, DB0 -> GPIO_PinConfig.GPIO_PinNumber, ((data >> 0) & 0x01));
	GPIO_WriteToOutputPin(DB1 -> pGPIOx, DB1 -> GPIO_PinConfig.GPIO_PinNumber, ((data >> 1) & 0x01));
	GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 -> GPIO_PinConfig.GPIO_PinNumber, ((data >> 2) & 0x01));
	GPIO_WriteToOutputPin(DB3 -> pGPIOx, DB3 -> GPIO_PinConfig.GPIO_PinNumber, ((data >> 3) & 0x01));
	GPIO_WriteToOutputPin(DB4 -> pGPIOx, DB4 -> GPIO_PinConfig.GPIO_PinNumber, ((data >> 4) & 0x01));
	GPIO_WriteToOutputPin(DB5 -> pGPIOx, DB5 -> GPIO_PinConfig.GPIO_PinNumber, ((data >> 5) & 0x01));
	GPIO_WriteToOutputPin(DB6 -> pGPIOx, DB6 -> GPIO_PinConfig.GPIO_PinNumber, ((data >> 6) & 0x01));
	GPIO_WriteToOutputPin(DB7 -> pGPIOx, DB7 -> GPIO_PinConfig.GPIO_PinNumber, ((data >> 7) & 0x01));
	*/
	for(uint8_t i = 0; i < data_size; i++)
	{
		GPIO_WriteToOutputPin(pins[i].pGPIOx, pins[i].GPIO_PinConfig.GPIO_PinNumber, (data & 0x01));
		data >>= 1;
	}
	pulseEnablePin(E);
	cursor_pos_horiz++;
}


// if returns HIGH, means no new instructions can be accepted
uint8_t lcd_check_busy_flag( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB7)
{
	GPIO_WriteToOutputPin(RS -> pGPIOx, RS -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(RW -> pGPIOx, RW -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	pulseEnablePin(E);
	uint8_t busy_flag = GPIO_ReadFromInputPin(DB7 -> pGPIOx, DB7 -> GPIO_PinConfig.GPIO_PinNumber);
	return busy_flag;
}

void lcd_set_ddram_address(GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, \
		GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, \
		GPIO_Handle_t *DB6, GPIO_Handle_t *DB7, uint8_t address)
{
	GPIO_WriteToOutputPin(RS -> pGPIOx, RS -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(RW -> pGPIOx, RW -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB7 -> pGPIOx, DB7 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	//GPIO_WriteToOutputPin(DB6 -> pGPIOx, DB6 -> GPIO_PinConfig.GPIO_PinNumber, HIGH);

	// set cursor to line 2
	write_ddram_address_to_pins(DB0, DB1, DB2, DB3, DB4, DB5, DB6, address);


	pulseEnablePin(E);
}

void write_ddram_address_to_pins(GPIO_Handle_t *DB0, GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, \
		GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, GPIO_Handle_t *DB6, uint8_t address)
{
	// Note: these are the boundaries of the 1602a LCD module for 2-line display mode
	if(((address >= 0x00) && (address <= 0x0F)) || ((address >= 0x3F) && (address <= 0x4F)))
	{
		GPIO_WriteToOutputPin(DB0 -> pGPIOx, DB0 -> GPIO_PinConfig.GPIO_PinNumber, ((address >> 0) & 0x01));
		GPIO_WriteToOutputPin(DB1 -> pGPIOx, DB1 -> GPIO_PinConfig.GPIO_PinNumber, ((address >> 1) & 0x01));
		GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 -> GPIO_PinConfig.GPIO_PinNumber, ((address >> 2) & 0x01));
		GPIO_WriteToOutputPin(DB3 -> pGPIOx, DB3 -> GPIO_PinConfig.GPIO_PinNumber, ((address >> 3) & 0x01));
		GPIO_WriteToOutputPin(DB4 -> pGPIOx, DB4 -> GPIO_PinConfig.GPIO_PinNumber, ((address >> 4) & 0x01));
		GPIO_WriteToOutputPin(DB5 -> pGPIOx, DB5 -> GPIO_PinConfig.GPIO_PinNumber, ((address >> 5) & 0x01));
		GPIO_WriteToOutputPin(DB6 -> pGPIOx, DB6 -> GPIO_PinConfig.GPIO_PinNumber, ((address >> 6) & 0x01));
	}
}
