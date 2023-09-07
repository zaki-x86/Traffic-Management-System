/*
 * LCD..c
 *
 *  Created on: Aug 22, 2023
 *      Author: win 10
 */
#include "LCD.h"
#define  F_CPU 8000000UL
#include <util/delay.h>

void LCD_vInit(void)
{
	_delay_ms(200);
	#if defined eight_bits_mode
	DIO_vsetPINDir(LCD_DATA_PORT, 0, 1);
	DIO_vsetPINDir(LCD_DATA_PORT, 1, 1);
	DIO_vsetPINDir(LCD_DATA_PORT, 2, 1);
	DIO_vsetPINDir(LCD_DATA_PORT, 3, 1);
	DIO_vsetPINDir(LCD_DATA_PORT, 4, 1);
	DIO_vsetPINDir(LCD_DATA_PORT, 5, 1);
	DIO_vsetPINDir(LCD_DATA_PORT, 6, 1);
	DIO_vsetPINDir(LCD_DATA_PORT, 7, 1);
	DIO_vsetPINDir(LCD_CTRL_PORT, EN, 1);
	DIO_vsetPINDir(LCD_CTRL_PORT, RW, 1);
	DIO_vsetPINDir(LCD_CTRL_PORT, RS, 1);
	DIO_write(LCD_CTRL_PORT, RW, 0);
	LCD_vSend_cmd(EIGHT_BITS); //8 bit mode
	_delay_ms(1);
	LCD_vSend_cmd(CURSOR_ON_DISPLAN_ON);//display on cursor on
	_delay_ms(1);
	LCD_vSend_cmd(CLR_SCREEN);//clear the screen
	_delay_ms(10);
	LCD_vSend_cmd(ENTRY_MODE); //entry mode
	_delay_ms(1);

	#elif defined four_bits_mode
	DIO_vsetPINDir(LCD_DATA_PORT, 4, 1);
	DIO_vsetPINDir(LCD_DATA_PORT, 5, 1);
	DIO_vsetPINDir(LCD_DATA_PORT, 6, 1);
	DIO_vsetPINDir(LCD_DATA_PORT, 7, 1);
	DIO_vsetPINDir(LCD_CTRL_PORT,EN,1);
	DIO_vsetPINDir(LCD_CTRL_PORT,RW,1);
	DIO_vsetPINDir(LCD_CTRL_PORT,RS,1);
   	DIO_write(LCD_CTRL_PORT, RW, 0);
	LCD_vSend_cmd(RETURN_HOME); //return home
	_delay_ms(10);
	LCD_vSend_cmd(FOUR_BITS); //4bit mode
	_delay_ms(1);
	LCD_vSend_cmd(CURSOR_ON_DISPLAN_ON);//display on cursor on
	_delay_ms(1);
	LCD_vSend_cmd(CLR_SCREEN);//clear the screen
	_delay_ms(10);
	LCD_vSend_cmd(ENTRY_MODE); //entry mode
	_delay_ms(1);
	#endif
}


static void send_falling_edge(void)
{
	DIO_write(LCD_CTRL_PORT, EN, 1);
	_delay_ms(2);
	DIO_write(LCD_CTRL_PORT, EN, 0);
	_delay_ms(2);
}

void LCD_vSend_cmd(char cmd)
{
	#if defined eight_bits_mode
	DIO_write_port(LCD_DATA_PORT, cmd);
	DIO_write(LCD_CTRL_PORT, RS, 0);
	send_falling_edge();

	#elif defined four_bits_mode
	write_high_nibble(LCD_DATA_PORT,cmd>>4);
	DIO_write(LCD_CTRL_PORT,RS,0);
	send_falling_edge();
	write_high_nibble(LCD_DATA_PORT,cmd);
	DIO_write(LCD_CTRL_PORT,RS,0);
	send_falling_edge();
	#endif
	_delay_ms(1);
}

void LCD_vSend_char(char data)
{
	#if defined eight_bits_mode
	DIO_write_port(LCD_DATA_PORT, data);
	DIO_write(LCD_CTRL_PORT, RS, 1);
	send_falling_edge();

	#elif defined four_bits_mode
	write_high_nibble(LCD_DATA_PORT,data>>4);
	DIO_write(LCD_CTRL_PORT,RS,1);
	send_falling_edge();
	write_high_nibble(LCD_DATA_PORT,data);
	DIO_write(LCD_CTRL_PORT,RS,1);
	send_falling_edge();
	#endif
	_delay_ms(1);
}


void LCD_vSend_string(const char *data)
{
	while((*data)!='\0')
	{
		LCD_vSend_char(*data);
		data++;
	}
}

void LCD_clearscreen()
{
	LCD_vSend_cmd(CLR_SCREEN);
	_delay_ms(10);
}

void LCD_movecursor(char row, char coloumn)
{
	char data ;
	if(row > 2||row < 1||coloumn > 16||coloumn < 1)
	{
		data = 0x80;
	}
	else if(row == 1)
	{
		data = 0x80 + coloumn - 1 ;
	}
	else if (row == 2)
	{
		data = 0xc0 + coloumn - 1 ;
	}

	LCD_vSend_cmd(data);
	_delay_ms(1);
}
