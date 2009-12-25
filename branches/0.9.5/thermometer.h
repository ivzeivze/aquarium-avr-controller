/** 
 * thermometer.h
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

#ifndef THERMOMETER_H
#define THERMOMETER_H


/**
 * thermometer.h
 * Contains functions for handling the thermometer chip on TWI bus.
 */

/**
 * We have a DS1621 thermometer device(chip) on TWI bus, 
 * it's TWI address (7 less significant bits) follows. 
 * The address must be set at the hardware level by 
 * connecting address pins of the termometer to GND and
 * VCC (see thermometer datasheet).
 **/
#define TERM_TWI_ADDR bits(0,1,0,0,1,1,0,1)



/**
 * Issues a comand for DS1621 thermometer to start
 * converting (measuring) it's temperature. It takes time, so
 * a separate method should be involved to ensure end of conversion
 * before getting temperature value
 * @return 0, if all right, different error codes, if not.
 */
char ask_termometer_start_temperature_conversion(void);




/**
 * Connects to the thermometer on TWI 
 * and reports it's temperatire in format, described below.
 * Thermometer need to be asked to start conversion separately, 
 * if it's needed.
 *
 * The method performs dumb busy-waiting while waiting
 * for TWI bus, using no interrupts. Keep it simple, 
 * oaklog (dubina!) 8P.
 * 
 * Temperature format: t[C^o]*2 (binary +1bit).
 * That is temperature in Celsium degrees, 
 * multiplied by two. This limits measurements range against that of
 * the chip, however, the diapasone borders are well beyond conditions
 * of a normal aquarium
 * In particular, real temperature over 63.5 C will yield 
 * 63.5 C measurement, the temperature won't go beneath -55.0, because
 * of hardware not able to measure so low temperatures.
 *
 * @return 0 in case if all right, various error codes otherwise.
 **/
char get_temperature(char*t);



/**
 * Reads the contents of DS1621 control register
 * @param c - pointer to one byte, contents of  control 
 * register are stored there.
 * @return 0, if all right; various return codes, if not.
 */
char get_term_control_register(char*c);



/**
 * Writes to DS1621 control register
 * @param c - new register value 
 *
 * @return 0, if all right; various return codes, if not.
 */
char set_term_control_register(char c);


/**
 * Looks at non-volatile termometer parameters and
 * fixes them, if they are configured improperly.
 * Proper configs is to make only one temperature conversion
 * on demand.
 *
 * @return 0, in case all config are correct after calling this method
 *			(e.g. they have been correct before or were wrong, but now 
 *			have been fixed)
 *			not 0, in case some error occurs, that don't allow us make sure,
 *			that termometer has proper setup
 */
char check_termo_configs(void);

#endif

