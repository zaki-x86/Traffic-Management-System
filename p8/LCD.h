
#ifndef LCD_H_
#define LCD_H_

#include "DIO.h"
#include "LCD_config.h"


#define CLR_SCREEN 0x01
#define CURSOR_ON_DISPLAN_ON 0x0c
#define RETURN_HOME 0x02
#define ENTRY_MODE 0x06

#define LCD_DATA_PORT 'C'
#define LCD_CTRL_PORT 'D'
#define EN 7
#define RS 5
#define RW 6

#if defined four_bits_mode
    #define FOUR_BITS 0x28
#elif defined eight_bits_mode
    #define EIGHT_BITS 0x38
#endif

void LCD_vInit(void);

static void send_falling_edge(void);

void LCD_vSend_cmd(char cmd);

void LCD_vSend_char(char data);

void LCD_vSend_string(const char *data);

void LCD_clearscreen();

void LCD_movecursor(char row,char coloumn);

#endif /* LCD_H_ */
