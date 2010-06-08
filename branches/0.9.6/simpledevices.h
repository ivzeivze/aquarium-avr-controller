/** 
 * simpledevices.h
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



#ifndef SIMPLEDEVICES_H
#define SIMPLEDEVICES_H

/**
 * Contains functions to operate on simple devices, our microcontroller
 * has attached to it.  
 * Among them there are relays, a beeper, blinking 
 * led, LCD backlight. 
 * These are simple in comparisson with thermometer, e.g., because 
 * the termometer is on TWI with lots of code to comunicate properly to
 * it, while using the named devices involves only some pin voltage 
 * level manipulation.
 */



/** 
 * Turns LCD backlight on/off
 * @param state - boolean, whether to turn light on or off
 */
void lcd_light(char state);


/**
 * Led blinker, turns led indicator on.
 */
void ledon(void);

/**
 * Led blinker, turns led indicator off.
 */
void ledoff(void);



/**
 *Beeper, turns beeping on.
 */
void beepon(void);

/**
 *Beeper, turns beeping off.
 */
void beepoff(void);




/**
 * Device macros, containing it's address. Used in methods below.
 */
#define LAMP		2
#define HEATER		3
#define VENTILATOR	4

/** 
 * Turns relays on/off
 * @param device - char defining one of 3 relays, switching different
 * 					external devices
 *
 * @param state - boolean, whether to turn light on or off
 */
void relay(char device, char state);




/**
 * Gets device state
 * @param device - device to get state of
 * @return boolean, true if device is on, false if it is off.
 */ 
char relay_state(char device);


#endif
