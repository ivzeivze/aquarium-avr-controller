/** 
 * thermometer.c
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



#include <avr/io.h>
#include "bits.h"
#include "delays.h"
#include "thermometer.h"



/**
 * The helper method waits for TWCR bit 7 to 
 * become 1 by means of busy waiting. Hardware 
 * setting of this bit means TWI is ready 
 * to continue.
 */
static void wait_twi(void){
	while(!(TWCR&bits(1,0,0,0,0,0,0,0))){
		/*do nothing 8)*/
	}
}


char ask_termometer_start_temperature_conversion(){
	/* sending START condition */
	for(;;){/*loop, restarted if arbitration lost*/
		TWCR=bits(1,0,1,0,0,1,0,0);/*bit 6 - ack disable (no matter)*/
		/*bit 0 - TWIE=0, no interrupts!!*/
		/*We write 1 to 7 bit. It was surprizing to find out, that this sets 7 bit to zero (clears it).
		This bit can be set to 1 only from hardware. Hardware sets it high, when TWI gets ready with 
		some task. Generally, tasks are initiated with clearing this 7'th bit.*/
		wait_twi();		
		/*start sent, looking for return codes*/
		char status=TWSR&bits(1,1,1,1,1,0,0,0);
		if(!((status==0x08)|(status==0x10))){
			/*that's a trouble*/
			/*According to the ATMega16 ATMEL manual, this can't happen*/
			/*leaving TWI silently*/			
			return 1;
		}
		/*Hey, termometer, we want to write to you!*/
		TWDR=(TERM_TWI_ADDR<<1)&bits(1,1,1,1,1,1,1,0);/*0 bit left zero - write*/
		TWCR=bits(1,0,0,0,0,1,0,0);		
		wait_twi();				
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		/*lcd_num_print(0,(TERM_TWI_ADDR<<1)&bits(1,1,1,1,1,1,1,0));*/
		if(0x38==status){
			/*Arbitration lost while transmitting,
			another Master (where from??) owns the TWI bus.
			Let's return back to start-sending, this is permittable according to ATMEL's manual*/
			continue;			
		}
		if(0x20==status){			
			/*Slave address with W flag transmitted, but no ack recieved.
			May be, thermometer is dead x_x. Or disconnected.*/
			/*gracefully releasing TWI bus*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 2;
		}
		if(0x18!=status){
			/*Unknown status ??, that can't be according to Atmel's manual*/
			/*silently leaving*/
			return 3;
		}
		/*Status is 0x18, thermometer properluy acked at us, we can go on.*/
		/*Preparing one byte message for the termometer*/
		TWDR=0xee;/*Start temperature conversion*/
		TWCR=bits(1,0,0,0,0,1,0,0);
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if(0x38==status){
			/*Arbitration lost while transmitting (here and later, these events are Very imbelievable)*/			
			continue;			
		}
		if(0x30==status){			
			/*Data transferred - no ack :(*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 4;
		}
		if(0x28!=status){
			/*God knows, what's this!! Leaving things so.*/
			return 5;
		}		
		break;
	}
	TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/		
	return 0;
}





char get_temperature(char*t){	
	for(;;){/*Loop, that repeats, if arbitration gets lost.*/
		/*Start.*/
		TWCR=bits(1,0,1,0,0,1,0,0);
		wait_twi();		
		char status=TWSR&bits(1,1,1,1,1,0,0,0);		
		if(status!=0x10){
			/*that's a trouble*/
			/*According to the ATMega16 ATMEL manual, this can't happen*/
			/*Leaving things unmodified*/
			return 1;
		}
		TWDR=(TERM_TWI_ADDR<<1)&bits(1,1,1,1,1,1,1,0);/*7 bit left zero - write*/
		TWCR=bits(1,0,0,0,0,1,0,0);
		wait_twi();		
		/*something's ready!*/
		status=TWSR&bits(1,1,1,1,1,0,0,0);		
		if(0x38==status){
			/*Arbitration lost*/
			continue;			
		}
		if(0x20==status){			
			/*Slave address with W flag transmitted, but no ack recieved.
			May be, thermometer is dead x_x. Or disconnected.*/
			/*gracefully releasing TWI bus*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 2;
		}
		if(0x18!=status){
			/*Unknown status ??, that can't be according to Atmel's manual*/
			/*leaving the system so*/
			return 3;
		}
		/*address sent*/
		TWDR=0xaa;/*access temperature*/
		TWCR=bits(1,0,0,0,0,1,0,0);/*Fly, data!*/
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if(0x38==status){
			/*Arbitration lost while transmitting (here and later, these events are Very imbelievable)*/			
			continue;			
		}
		if(0x30==status){			
			/*Data transferred - no ack :(*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 4;
		}
		if(0x28!=status){
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 5;
		}
		TWCR=bits(1,0,1,0,0,1,0,0);/*Repeated start*/
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if(status!=0x10){/*repeated start*/			
			/*Leaving things so*/
			return 6;
		}
		TWDR=(TERM_TWI_ADDR<<1)|bits(0,0,0,0,0,0,0,1);/*0 bit = 1 - read*/
		TWCR=bits(1,0,0,0,0,1,0,0);
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if(0x38==status){
			/*Arbitration lost*/
			/*going to restart*/
			continue;			
		}
		if(0x48==status){			
			/*No Ack*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 7;
		}
		if(0x40!=status){			
			/*0_o what's this?*/
			return 8;
		}
		/*Ack recieved*/
		TWCR=bits(1,1,0,0,0,1,0,0);/*enabling ATmega acking to thermometer data, waitng for higher bits*/
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if (status!=0x50){
			/*0x50 - recieved data, sent ack*/
			/*if it is not so, that's wrong*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 9;
		}
		char msb=TWDR;
		TWCR=bits(1,0,0,0,0,1,0,0);/*disabling ATmega acking to thermometer data, waitng for lower bits*/
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if (status==0x38){
			/*arbitration lost while NACKing (it can't be lost while ACKing, 
			because ACK is low)*/
			continue;
		}
		if (status!=0x58){
			/*0x58 - recieved data, no ack sent*/
			/*if it is not so, that's wrong*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 10;
		}
		char lsb=TWDR;
		TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/		
		/*###that's all!!*/				
		/*
		if (msb&bits(1,0,0,0,0,0,0,0)){
			*t=(int)(0xff00|msb)*10+((int)(lsb>>7))*5;
		}else{
			*t=(int)(0x0000|msb)*10+((int)(lsb>>7))*5;
		}*/
		/*Thermometer produces temperatures from -55.0 to +125.0,
		taking it in consideration.*/

		if(msb<0){
			*t=(msb<<1)+(lsb>>7);
		}else{
			if(msb>=64){
				*t=bits(0,1,1,1,1,1,1,1);/*63.5*/
			}else{
				*t=(msb<<1)+(lsb>>7);
			}
		}		
		return 0;/*all right*/
	}	
}








char get_term_control_register(char*c){
	for(;;){/*Loop, that repeats, if arbitration gets lost.*/
		
		TWCR=bits(1,0,1,0,0,1,0,0);
		wait_twi();		
		char status=TWSR&bits(1,1,1,1,1,0,0,0);		
		if((status!=0x10)&(0x08!=status)){
			/*Neither start, nor reeated start, TWI unusable*/
			return 1;
		}
		TWDR=(TERM_TWI_ADDR<<1);/*0 bit left 0 - write*/
		TWCR=bits(1,0,0,0,0,1,0,0);
		wait_twi();		
		/*something's ready!*/
		status=TWSR&bits(1,1,1,1,1,0,0,0);		
		if(0x38==status){
			/*Arbitration lost*/
			continue;			
		}
		if(0x20==status){			
			/*no ack*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 2;
		}
		if(0x18!=status){
			/*Unknown status ??, that can't be according to Atmel's manual*/
			/*leaving the system so*/
			return 3;
		}
		/*address sent*/
		TWDR=0xac;/*access config register*/
		TWCR=bits(1,0,0,0,0,1,0,0);
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if(0x38==status){
			/*Arbitration lost while transmitting (here and later, these events are Very imbelievable)*/			
			continue;			
		}
		if(0x30==status){			
			/*Data transferred - no ack :(*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 4;
		}
		if(0x28!=status){
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 5;
		}
		TWCR=bits(1,0,1,0,0,1,0,0);/*Repeated start*/
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if(status!=0x10){/*repeated start*/			
			/*Leaving things so*/
			return 6;
		}
		TWDR=(TERM_TWI_ADDR<<1)|bits(0,0,0,0,0,0,0,1);/*7 bit = 1 - read*/
		TWCR=bits(1,0,0,0,0,1,0,0);
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if(0x38==status){
			/*Arbitration lost*/
			/*going to restart*/
			continue;			
		}
		if(0x48==status){			
			/*No Ack*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 7;
		}
		if(0x40!=status){			
			/*0_o what's this?*/
			return 8;
		}
		/*Ack recieved*/
		TWCR=bits(1,0,0,0,0,1,0,0);/*disabling ATmega acking to thermometer data, waitng for higher bits*/
		wait_twi();				
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if (status==0x38){
			/*arbitration lost while NACKing (it can't be lost while ACKing, 
			because ACK is low)*/
			continue;
		}
		if (status!=0x58){
			/*0x58 - recieved data, sent no ack*/
			/*if it is not so, that's wrong*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 9;
		}
		*c=TWDR;
		TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/		
		break;
	}	
	return 0;
}



char set_term_control_register(char c){
	for(;;){/*Loop, that repeats, if arbitration gets lost.*/
		
		TWCR=bits(1,0,1,0,0,1,0,0);
		wait_twi();		
		char status=TWSR&bits(1,1,1,1,1,0,0,0);		
		if((status!=0x10)&(0x08!=status)){
			/*Neither start, nor reeated start, TWI unusable*/
			return 1;
		}
		TWDR=(TERM_TWI_ADDR<<1);/*0 bit left 0 - write*/
		TWCR=bits(1,0,0,0,0,1,0,0);
		wait_twi();		
		/*something's ready!*/
		status=TWSR&bits(1,1,1,1,1,0,0,0);		
		if(0x38==status){
			/*Arbitration lost*/
			continue;			
		}
		if(0x20==status){			
			/*no ack*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 2;
		}
		if(0x18!=status){
			/*Unknown status ??, that can't be according to Atmel's manual*/
			/*leaving the system so*/
			return 3;
		}
		/*address sent*/
		TWDR=0xac;/*access config register*/
		TWCR=bits(1,0,0,0,0,1,0,0);
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if(0x38==status){
			/*Arbitration lost while transmitting (here and later, these events are Very imbelievable)*/			
			continue;			
		}
		if(0x30==status){			
			/*Data transferred - no ack :(*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 4;
		}
		if(0x28!=status){
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 5;
		}	
		TWDR=c;/*data*/
		TWCR=bits(1,0,0,0,0,1,0,0);
		wait_twi();		
		status=TWSR&bits(1,1,1,1,1,0,0,0);
		if(0x38==status){
			/*Arbitration lost while transmitting (here and later, these events are Very imbelievable)*/			
			continue;			
		}
		if(0x30==status){			
			/*Data transferred - no ack :(*/
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 6;
		}
		if(0x28!=status){
			TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/
			return 7;
		}	
		/*all!*/
		TWCR=bits(1,0,0,1,0,1,0,0);/*Stop, generated immediately*/		
		break;
	}	
	return 0;
}



char check_termo_configs(void){
	char b=0xff;
	char rv=get_term_control_register(&b);
	if(rv){
		return 1;
	}else{
		if(b&bits(0,0,0,0,0,0,0,1)){
			/*setups are OK, going to return 0*/
		}else{
			char rv=set_term_control_register(b|bits(0,0,0,0,0,0,0,1));			
			if(rv){						
				return 2;
			}else{
				/*waiting for termometer to stop being busy writng flash*/
				for(;;){
					char rv=get_term_control_register(&b);									
					if(rv){
						/*error :(*/
						sleep_millis(300);/*not to harm termometer*/
						return 4;
					}
					if(b&bits(0,0,0,1,0,0,0,0)){/*busy writing*/
						continue;
					}
					/*all right, writing done!*/							
					break;
				}
			}

		}
	}
	return 0;
}
