/** 
 * display.h
 * Copyright (C) 2009  ZelinskiyIS.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *    
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */




#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * display.h 
 * Contains methods for printing data on the LCD display.
 * All the methods are designed do be called from "main" loop,
 * not from interrupts, and interrupting the methods don't harm 
 * their correct execution.
 */


#include "delays.h" /*sleep...() functions*/
#include <avr/io.h> /*hardware access*/

/**
 * The ATmega has an LCD display, connected to it's 
* port A. The display is WH0802A-YGH-CT.
*
* The connection scheme follows:
*
*
* A0		-|_1k_|-		Data4
* A1		-|_1k_|- 		Data5
* A2		-|_1k_|- 		Data6
* A3		-|_1k_|- 		Data7
*
* A4		--------		R/S
* A5		--------		R/W
* A6		Not connected	---
* A7		--------		E(tact)
*
* By default, all PORTA pins must be configured as
* outputs. If a method needs to make them inputs, 
* it must then make them outputs back.
* 
* All port pins must be left LOW after using, so that if power occasianally
* gets down, there will be less time, during which high levels are applied to
* the LCD pins (Experiment: up to 30 ma of current has been detected to go
* through a powered-down LCD, when high levels are applied to it (even with 1K
* resistors). The phenomenon is explained by the fact that port input is
* connected to GND and VCC with two protecting diodes: 
* GND|----I>|----|port|---I>|-----|VCC   ( -I>|- means diode ).
*/

/**
 * All character outputting methods, listed here, attempt to 
 * reinitialise LCD, if they discover LCD freeze.
 */


/**
 * According to Winstar company manual, 
 * we need to make some initialisation
 * with the LCD display to bring it
 * to operation condition after power-on.
 * Here follows a method, that implements
 * this initialization.
 **/
void initialize_lcd(void);


/** 
 * Clears lcd.
 */
void clr_lcd(void);

/**
 * Puts a character 
 * to the LCD Display Data RAM.
 * @param addr - DDRAM address (7 bits are taken in account)
 * @param symbol - symbol code (all 8 bits)
 **/
void lcd_put_char(char addr, char symbol);
/**
 *Takes a null-terminated string str and feeds
 *it up to the lcd DDRAM, starting from address
 *display_pos. The display behaviour might depend 
 *on current display parameters (e.g. the screen will
 *shift up the memory on every wriring, if this is enabled)
 *
 *Input arguments handling:
 * <> The behaviour of this method in case str is not a null-terminated
 *string is undefined (may produce some garbage).
 * <>  Only lower 7 bits in memory address are taken
 *to define memory address.
 * @param display_pos - lcd address to start printing string from
 * @param str - pointer to string to print
 */
void lcd_print(char display_pos, char* str);

/**
 * Prints a number to the 
 * specified lcd address.
 * @param n - number to print (negative numbers possible)
 * @param addr - lcd address to start printing from
 * @return number of lcd positions used
*/
char lcd_num_print(char addr, int n);

/**
 * Displays time in hh:mm:ss format, starting from addr LCD position.
 * If hours, minutes, seconds are outside normal value diapason, the function 
 * will produce screen garbage (but nothing more dangerous).
 * @param hours, minutes, seconds - time to print
 * @param addr - lcd address to print line to
 */
void lcd_time_print(char hours, char minutes, char seconds, char addr);



/**
 * Prints temperature from within 10..40 diapason (what simplifies algorythm).
 * The resulting temperature value is guaranteed to occupy 4 lcd cells,
 * As everywhere, temperature is stored in (real value)*2 integers
 * @param temp - temperature value
 * @param lcdpos - LCD symbol position, from which to start writing
 */
void lcd_print_positive_temp_more_than_10_less_than_100(char lcdpos,char temp);



#endif
