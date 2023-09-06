/*
 * LCD.c
 *
 *  Created on: Aug 20, 2023
 *      Author: win 10
 */

#include "DIO.h"
void LED_vInit(unsigned char portname,unsigned char pinnumber)
{
	DIO_vsetPINDir(portname,pinnumber,1);//Set the given pin in the given port as an output
}
void LED_vTurnOn(unsigned char portname,unsigned char pinnumber)
{
	DIO_write(portname,pinnumber,1);//Set the given pin in the given port to one(on)
}
void LED_vTurnOff(unsigned char portname,unsigned char pinnumber)
{
	DIO_write(portname,pinnumber,0);//Set the given pin in the given port to zero(off)
}
void LED_vToggle(unsigned char portname,unsigned char pinnumber)
{
	DIO_toggle(portname,pinnumber);//Set the given pin in the given port to zero if it is one or set it to one if it is zero
}
