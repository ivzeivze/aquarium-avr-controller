/** 
 * delays.h
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




#ifndef DELAYS_H
#define DELAYS_H

/**
 * Different time-wasting functions, that perform busy-waiting.
 * All the timings here depends on the fact that the mu frequency 
 * is around 1 MHz.
 */

/**
 * Sleeps approximately the given number of 
 * milliseconds.
 * If mu clock were precise, the
 * error in really sleeped time 
 * would be about 30%.
 * @param millis - N milliseconds to sleep
*/
void sleep_millis(unsigned int millis);


/**
 * Does nothing 50 clock tacts thus
 * making a 50 microsecond delay.
 * The actual time, taken is a bit longer:
 * additional 8 tacts come from calling time
 **/
void sleep_50micros(void);

/**
 * Very same to the previous. 
 */
void sleep_10micros(void);


#endif
