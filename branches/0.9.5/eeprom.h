/** 
 * eeprom.h
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



#ifndef EEPROM_H
#define EEPROM_H

/**
 * Copy-pasted EEPROM access methods from ATmel manual.
 * The methods must be called with all interrupts disabled, 
 * othewise, according to Atmel's manual, there will be mess.
 */

/**
 * Writes one byte of specified data to the specified address of
 * microcontroller on-board flash memory.
 * @param uiAddress - address of byte in flash
 * @param ucData - data byte to be put there
 */
void EEPROM_write(unsigned int uiAddress, unsigned char ucData);


/**
 * Reads one byte from the specified location of microcontroller 
 * on-board flash memory.
 * @param uiAddress - address of flash memory to read one byte from
 * @return byte, read from the specified location in flash memory
 */
unsigned char EEPROM_read(unsigned int uiAddress);




#endif
