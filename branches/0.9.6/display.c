/** 
 * display.c
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




#include "display.h"
#include "bits.h"



/**
 * Tact makes A7 ping generate a tact impulse 
 * It shall be noted that this method provides
 * a necessary delay for data levels to establish
 * (there are protecting 1K resistors there). This means
 * that tact() may be called just after PORTA=something.
 * Also, tact provides some after-delay, thus improving 
 * stability. Interruptions, possibly occuring during the method 
 * execution, only prolong delay periods, thus not hurting the
 * tacting process.
*/
static void tact(void){       
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");/*11 microsecond delay - let the data levels establish*/
        PORTA|=1<<7;            
        asm("nop");/*there are no resistors on E line, so*/
        asm("nop");/*it must be working faster*/        
        PORTA&=~(1<<7); 
        asm("nop");
}



void initialize_lcd(void){
	PORTA=0;
	/*Port-for output!*/
	DDRA=0xff;
	/*let's wait 40 ms, according to LCD manual*/
	/*Here and later we will make some time reserve to 
	cope with the possibility of RC frequency swing-out.
	We'll make a 20% reserve.*/
	sleep_millis(50);
	/*PORTA=...;
	tact();
	is a correct method to write 4 bits to the LCD.
	Out microcontroller is slow enough, that even
	a single-tact interval is wider than all the
	down timing limits of the LCD. In other words, 
	even if we (in the ATmega) make no pauses, 
	the LCD won't miss them, because of being 
	hasteful.*/
	/*helper bits(tact,n/c,R/W,R/S,d7,d6,d5,d4)*/
	PORTA=bits(0,0,0,0,0,0,1,1);
	tact();		
	sleep_50micros();
	sleep_50micros();
	char i;
	for(i=0;i<2;i++){
		/*wait 39 mus*/
		asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
		PORTA=bits(0,0,0,0,0,0,1,0);		 
		tact();
		PORTA=bits(0,0,0,0,1,0,0,0);/*2 lines (1 at a3), 5x8 letter (0 at a2)*/
		tact();		
	}/*two times, according to the manual*/
	/*wait 37 micros*/
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	char D=1;/*Display on*/
	char C=1;/*Underline cursor*/
	char B=0;/*Black blinking box cursor*/
	PORTA=bits(0,0,0,0,0,0,0,0);	 
	tact();
	PORTA=bits(0,0,0,0,1,D,C,B);/*display,cursor,blinking bits (2,1,0)*/	 
	tact();
	/*wait 37 micros*/
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	PORTA=bits(0,0,0,0,0,0,0,0);	 
	tact();
	PORTA=bits(0,0,0,0,0,0,0,1);/*display clear*/	 
	tact();
	/*wait 1.53 millisecond*/
	sleep_millis(2);
	char SH=0;/*Disabling screen walking on DDRAM input*/
	char ID=1;/*Cursor moving direction*/
	PORTA=bits(0,0,0,0,0,0,0,0);
	
	tact();	
	PORTA=bits(0,0,0,0,0,1,ID,SH);
	
	tact();
	/*39 micros*/
	sleep_50micros();
	sleep_50micros();
	PORTA=0;
}


/**
 * The method continuously reads lcd busy flag, until
 * it signalises that lcd is ready. After this method call
 * all PORTA lines are left as outputs. 
 * 
 * Note, that 
 * high level in BS means NOT busy, while low level is
 * busy. This is especially helpful, because if lcd gets powered off, 
 * the reading of busy bit will be 0, what means ready.
 * 
 * If the method fails to get answer from the LCD in some time (see code),
 * it returns fail.
 * @return 0	-	OK
 * 		   0xff	-	failed to get answer from the LCD
*/
static char wait_lcd_ready(void){
	char in=0xff;
	DDRA=bits(1,1,1,1,0,0,0,0);/*tact,r/s,r/w - outputs, 
								data lines - inputs*/
	/*helper bits(tact,n/c,R/W,R/S,d7,d6,d5,d4)*/
	PORTA=bits(0,0,1,0,0,0,0,0);/*R/S=9 R/W=1, 	
	low 4 zeros mean disabling pullup resistors*/
	asm("nop");asm("nop");
	asm("nop");
	asm("nop");asm("nop");
	/**
	 * Totally, all this loop takes a bit more that 22 microseconds.
	 * It may happen so that the LCD hangs (poor surge protection).
	 * If it fails to respong OK in proper time, the whole will get reset 
	 * from the watchdog. Fail.
	 * In order to keep our device resistant against such fails, 
	 * let's skip waiting after 
	 */
	int responce_tolerance = 5000; 
	 /** 
	  * tries.
	  * This gives 0.11 seconds of waiting assuming that it takes 22 microseconds to execute one loop pass.
	  * 
	  **/
	int fail_counter = 0;
	char answer_to_return = 0;/*unless set to 0xff in toleranse checker,
	this value will be returned at function end*/
	while(in)/*not zero-busy or turned off*/{
		fail_counter += 1;
		if(fail_counter > responce_tolerance){
			/*LCD has probably freezed :(*/
			answer_to_return = 0xff;
			break;/*finishing waiting process*/
		}
		PORTA|=1<<7;/*tact up!*/		 
		asm("nop");asm("nop");
		asm("nop");asm("nop");
		asm("nop");asm("nop");
		asm("nop");asm("nop");
		asm("nop");asm("nop");/*11 microseconds delay*/
		/*What's come??*/
		in=PINA&bits(0,0,0,0,1,0,0,0);/*7 bit - busy flag*/
		PORTA&=~(1<<7);/*tact down!*/
		asm("nop");asm("nop");
		PORTA|=1<<7;/*tact up!--------\     */
		asm("nop");asm("nop");    /*   \    */
		PORTA&=~(1<<7);/*tact down!-----> that was 4 bits skipping*/
		asm("nop");asm("nop");		
	}	
	
	DDRA=bits(1,1,1,1,1,1,1,1);/*setting DDRA pins back	to be outputs*/
	PORTA=0;/*Zero all levels*/
	
	return answer_to_return;
}


void clr_lcd(void){	
	PORTA=bits(0,0,0,0,0,0,0,0);
	tact();
	PORTA=bits(0,0,0,0,0,0,0,1);
	tact();
	PORTA=0;
	char fail=wait_lcd_ready();
	if(fail){
		initialize_lcd();/*LCD dead, reititialising it*/
		/*And LCD is clear, because it's just from reboot. TaDa!*/
	}
	
}


void lcd_put_char(char addr, char symbol){/*puts a character 
	to the LCD Display Data RAM.
	@param addr - DDRAM address (7 bits are taken in account)
	@param symbol - symbol code (all 8 bits)*/
	/*helper bits(tact,n/c,R/W,R/S,d7,d6,d5,d4)*/	
	for(;;){
		PORTA=(0<<7)+(0<<5)+(0<<4)+(1<<3)+(7&(addr>>4));
		/*let the levels establish*/
		tact();
		PORTA=(0<<7)+(0<<5)+(0<<4)+(0x0f&addr);
		/*let the levels establish*/
		tact();		
		char fail=wait_lcd_ready();
		if(fail){
			/*0_o the LCD's freezed.*/
			initialize_lcd();
			continue;/*again*/
		}
		PORTA=(0<<7)+(0<<5)+(1<<4)+(0x0f&(symbol>>4));
		/*let the levels establish*/
		tact();	
		PORTA=(0<<7)+(0<<5)+(1<<4)+(0x0f&symbol);
		/*let the levels establish*/
		tact();
		fail = wait_lcd_ready();
		if(fail){		
			initialize_lcd();
			continue;
		}
		PORTA=0;
		break;
	}
}

void lcd_print(char display_pos, char* str){
	while(*str){
		lcd_put_char(display_pos, *str);
		display_pos++;
		str++;
	}
}


char lcd_num_print(char addr, int n){
	char num_of_pos_used=0;
	char npass=0;
	int m;
	if(n<0) m=-n; else m=n;
	for(;;){
		m=m/10;
		npass+=1;
		if (!m) break;
	}
	if(n<0){
		lcd_put_char(addr,bits(0,0,1,0,1,1,0,1));/*minus*/
		num_of_pos_used+=1;
		addr+=1;
		m=-n;
	}else{
		m=n;
	}
	addr+=(npass-1);		
	for(;;){
		char this_cif=m%10;		
		lcd_put_char(addr,bits(0,0,1,1,0,0,0,0)+this_cif);
		num_of_pos_used+=1;
		m=m/10;
		addr-=1;
		if (!m) break;
	}
	return num_of_pos_used;
}


void lcd_time_print(char hours, char minutes, char seconds, char addr){	
	char position=addr;/*this stores next free position address*/
	if (hours<10){
		lcd_put_char(position,48);/*zero*/
		position+=1;
	}
	position+=lcd_num_print(position,hours);
	lcd_put_char(position,58);/*semicolon*/
	position+=1;
	if(minutes<10){
		lcd_put_char(position,48);
		position+=1;
	}
	position+=lcd_num_print(position,minutes);
	lcd_put_char(position,58);/*semicolon*/
	position+=1;
	if(seconds<10){
		lcd_put_char(position,48);
		position+=1;
	}
	lcd_num_print(position,seconds);	
}


void lcd_print_positive_temp_more_than_10_less_than_100(char lcdpos,char temp){	
	lcdpos+=lcd_num_print(lcdpos,(temp/2));
	lcd_put_char(lcdpos,44);/*comma*/
	lcdpos+=1;
	lcd_num_print(lcdpos,(temp%2)*5);
}
