/** 
 * simpledevices.c
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



#include "simpledevices.h"
#include <avr/io.h>

void lcd_light(char state){
	if (state){
		PORTB|=1<<1;
	}else{
		PORTB&=~(1<<1);
	}
}


void ledon(void){
	PORTC|=1<<2;
}
void ledoff(void){
	PORTC&=~(1<<2);
}


void beepon(void){
	PORTB|=1<<0;
}
void beepoff(void){
	PORTB&=~(1<<0);
}



void relay(char device, char state){
	if (state){
		PORTB|=(1<<device);/*on*/
	}else{		
		PORTB&=~(1<<device);/*off*/
	}
}


char relay_state(char device){
	return PORTB&(1<<device);
}

