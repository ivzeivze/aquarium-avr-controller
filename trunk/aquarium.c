/**
 * Aquarium.c, firmware for ATMega16L - based aquarium - controling device.
 * Version:
 **/
#define Ver0 0
#define Ver1 9
#define Ver2 7
/**
 * Copyright (C) 2009, 2010, 2012  ZelinskiyIS.
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
 * Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 **/


/*The program has been developed using avr-gcc compilator.*/
/**
 * Let the device be clcoked by 1MHz frequency
 */

/**
 * ATMega16L fuses dump from a working instance follows:
 *
 *
 * # uisp -dprog=dapa --rd_fuses
 * Atmel AVR ATmega16 is found.
 *
 * Fuse Low Byte      = 0xa1
 * Fuse High Byte     = 0xd9
 * Fuse Extended Byte = 0xff
 * Calibration Byte   = 0xa4  --  Read Only
 * Lock Bits          = 0xff
 *     BLB12 -> 1
 *     BLB11 -> 1
 *     BLB02 -> 1
 *     BLB01 -> 1
 *       LB2 -> 1
 *       LB1 -> 1
 *
 * Programming fuses is to be done at the time of uploading firmware.
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#include "bits.h"
#include "delays.h"			/*Busy-waiting functions*/
#include "display.h"		/*Here are loaded functions for working with display*/
#include "thermometer.h"	/*Functions for communicating with termometer*/
#include "eeprom.h"			/*EEPROM I/O functions*/
#include "locale.h"			/*Language and text related features*/
#include "simpledevices.h"	/*relays, lamps*/
#include "crc32.h"			/*crc32 engine*/




/*##Prototypes(of some functions)##*/
char get_temperature(char*);
void temperature_studying_reset(void);
void set_avg_temp_invalid(void);
void eeprom_backup_or_restore_configs(char,char);
/*--Prototypes--*/

/**
 * 9 byte buffer for text to be put to when drawing things on LCD.
 * This occupies these 9 bytes statically.
 **/
char str[9];



/*################*/
/*Buttons section*/

/*Prototypes of event dispatchers,
that will be called from interruption
thread, if the respetife button press
detected. These are executed within "sei"
environment, what means, they may be interrupted.*/
void button0_pressed(void);
void button1_pressed(void);
void button2_pressed(void);
void button3_pressed(void);


/*Technical: Buttons attached to:
Port	ButtonN
---------------
PD7			0
PD4			1
PD5			2
PD6			3
---------------
When any button is pressed, int0 pin becomes high (some simple
construction with diodes).
By defaul, ATMega starts with ports configured for input
and pullup resistors disabled. So, we don't need to change
things from their defaults when starting.
*/

/**
 * Reads values from three buttons,
 * saving them as bits, starting from lower, and
 * returns one byte with the data.
 **/
char get_button_levels(void){
	char val=PIND;
	/*return 	((val&bits(0,0,0,1,0,0,0,0))>>4)  +
			((val&bits(1,1,0,0,0,0,0,0))>>5)  ;*/
	return	((val&bits(1,0,0,0,0,0,0,0))>>7) +
	((val&bits(0,1,1,1,0,0,0,0))>>3);
}

/**
 * Disables button interrupt, thus disabling all buttons.
 * Useful while there is no AC power.
 */
void disable_buttons(void){
	GICR&=~(1<<INT0);
}

/**
 * Enables button interrupt, thus enabling all buttons back.
 */
void enable_buttons(void){
	GICR|=(1<<INT0);
}

#define BTNNOISEREDCTMILLIS 100
/*^Noise-reduction delay time^
The button interrupt responce trashes
all subsequent interrupt calls within
approximately this time interval.*/


/**
 * A variable, needed to handle button clicks
 * It's zero, when the user comes to click the button and it remains non-zero
 * while waiting BTNNOISEREDCTMILLIS milllis to eliminate noise.
 * It shall be noted that the data type is atomic - accessed in one instruction.
 * (I have seen disassembly and guarantee this!)
 */
volatile char button_state=0;

/**
 * Setting up pin interruption vector
 */
ISR(INT0_vect){
	if (button_state/*!=0*/){
		/*Secondary calls, just doing nothing
		to flush secondary interruptions*/
	}else{
		/*first run*/
		button_state=8;/*not default*/
		asm("sei");/*enabling interrupts to make all
		other button noise interrupts be flushed*/
		/*This also allows precise clock interrupts, that happen every second
		and should not be missed (othrwise, clock will go wrong)*/
		/*Waiting some time, flushing interruts and ackquiring data, ensurig stability*/
		char levels[3];
		sleep_millis(BTNNOISEREDCTMILLIS/4);
		levels[0]=get_button_levels();
		sleep_millis(BTNNOISEREDCTMILLIS/4);
		levels[1]=get_button_levels();
		sleep_millis(BTNNOISEREDCTMILLIS/4);
		levels[2]=get_button_levels();
		sleep_millis(BTNNOISEREDCTMILLIS/4);
		/*What have we got?*/
		char l=levels[0]&levels[1]&levels[2];/*If a button is not well-pressed,
											the user might have done it by mistake*/
		/*Calling event dispatchers from within interruption ~thread (it can't be called thread strictly)*/
		if(l&1)button0_pressed();
		if(l&2)button1_pressed();
		if(l&4)button2_pressed();
		if(l&8)button3_pressed();
		/*Returning button state to zero, so that next interrupt occurings won't be flushed any more*/
		asm("cli");/*disabling interruptions*/
		button_state=0;
	}
}



volatile char flag0=0;
volatile char flag1=0;
volatile char flag2=0;
volatile char flag3=0;
/*these flags are set, when respective buttons are detected to have been pressed.*/

void button0_pressed(){
	flag0=0xff;
}
void button1_pressed(){
	flag1=0xff;
}
void button2_pressed(){
	flag2=0xff;
}
void button3_pressed(){
	flag3=0xff;
}





















/*#__########Clock section################*/
volatile char seconds=0;
volatile char minutes=0;
volatile char hours=12;


void hours_up(void){
	hours+=1;
	if (hours>=24){
		hours=0;
	}
}

void minutes_up(void){
	minutes+=1;
	if (minutes>=60){
		minutes=0;
		hours_up();
	}
}

void seconds_up(void){
	seconds+=1;
	if (seconds>=60){
		seconds=0;
		minutes_up();
	}
}


/**
 * Setting up timer/counter2 interruption vector.
 */
ISR(TIMER2_OVF_vect){
	seconds_up();
}

/**
 * The method accurately copies current time to the given
 * locatin. The method is ~thread-safe, is supposed to be called
 * from main loop and uses interruption disable.
 * The method leaves interrupts enabled.
 * @param o_hours, o_minutes, o_seconds - places to put answer to
 */
void get_time(char* o_hours, char* o_minutes, char* o_seconds){
	asm("cli");/*Disabling interrupts*/
	*o_hours=hours;
	*o_minutes=minutes;
	*o_seconds=seconds; /*cli and sei used so that during copyind no
	interrupts would happen and change some values, what would bring
	mess*/
	asm("sei");/*Enabling interrupts*/
}

/*##^^######################################*/















/*############UI####################*/
/*User interface, this behaves somewhat like a state machine, despite
*the equivalence is not strict*/
enum UI_st {/*Main UI modes*/
	ui_default,
	ui_changesec,
	ui_changemin,
	ui_changehr,
	ui_changetemp,
	ui_changelight,
	ui_changelang,
	ui_showfirmware
};

/*Here come internal states, seclared as global variables*/
unsigned char changehour=0;/*internal state, mattering with ui_changetemp and ui_changelight, it
answers, which hour values we are now changing*/
unsigned char beep_state=0;/*when some trouble happens, the UI should beep at men, asking for help
this state is used to make beeps periodic and, thus, more attracting*/
enum UI_st ui_state = ui_default;

/*when not null, backlight is on, and it decrements every werde call.
*when null, there is no light
*/
char backlightstepsleft=0;

char lang=LANG_RU;/*Default*/

/*For profilactical needs, let's reinitialize LCD every*/
#define lcd_reinit_period 774
/*~seconds (really, werde calls)*/
int lcd_reinit_counter=0;

char temp		[24	]; /*One day per hour temperature map*/
char lightdata	[3	]; /*bit-by-bit per hour light map (LSB - 0, MSB - 23)*/

/*last average temperature
*set from outside the ui, the freshness determines, how many times the value can be shown*/
char avg_temp;
char avg_temp_invalid=1;/*set true, if abg_temp holds value, that's not yet adequate*/
char temp_meas_error=0;/*set externally, if we experience troubles getting temperature info*/

void set_avg_temp_invalid(){
	avg_temp_invalid=1;
}



/**
 * The function evolve User Interface by given logical button states
 * and makes a proper LCD output, that depends on current internal UI
 * state. It shall be called every time, even if no buttons have been
 * detected to be pressed.
 * The method may leave interrupts enabled, or change nothing regarding
 * interrupts.
 * @param up,down,change0,change1 - booleans, identifying, if the
 *        respective buttons have been pressed
 */
void werde(char up, char down, char change0, char change1){
	if(up|down|change0|change1){
		backlightstepsleft=20;/*lighting for approx 10 seconds*/
	}
	/*light rotater*/
	if(backlightstepsleft){
		backlightstepsleft--;
		lcd_light(backlightstepsleft);/*on 1 in, it will make 0 and turn light off*/
		if(backlightstepsleft==0){
			ui_state=ui_default; /*returning state back to initial, after time have passed*/
			eeprom_backup_or_restore_configs(0,0xff);/*backup configs to EEPROM, leaving interrupts enabled*/
		}
	}
	/*The change-changing Switch :P*/
	switch(ui_state){
	case ui_default:
		if(up|down){/*Fix LCD buttons - restart LCD*/
			initialize_lcd();
		}
		if(change1){
			ui_state=ui_changesec;
		}
		break;
	case ui_changesec:
		if(up|down){
			asm("cli");
			seconds+=1;
			if (seconds>30){
				seconds=0;
				minutes+=1; /*rounding to the nearest*/
			}else{
				seconds=0;
			}

			asm("sei");/*sei,cli - not to collide with time updating interrupt responce call*/
		}
		if(change0){
			ui_state=ui_changemin;
		}
		if(change1){
			ui_state=ui_changetemp; /*jump at once*/
		}
		break;
	case ui_changemin:
		if(up){
			asm("cli");
			minutes+=1;
			if (minutes>59){
				minutes=0;
			}
			asm("sei");
		}
		if(down){
			asm("cli");
			if(0==minutes){
				minutes=59;
			}else{
				minutes-=1;
			}
			asm("sei");
		}
		if(change0){
			ui_state=ui_changehr;
		}
		if(change1){
			ui_state=ui_changetemp; /*jump at once*/
		}
		break;
	case ui_changehr:
		if(up){
			asm("cli");
			hours+=1;
			if (hours>23){
				hours=0;
			}
			asm("sei");/*sei,cli - not to collide with time updating interrupt responce call*/
		}
		if(down){
			asm("cli");
			if(0==hours){
				hours=23;
			}else{
				hours-=1;
			}
			asm("sei");
		}
		if(change0){
			ui_state=ui_changesec;
		}
		if(change1){
			ui_state=ui_changetemp; /*jump at once*/
		}
		break;
	case ui_changetemp:/*reminding: temperature is stored in (real value)*2 integer*/
		if(up){
			temp[changehour]+=1;
			if (temp[changehour]>40*2){/*more is going to look like a fish soup :]*/
				temp[changehour]=20*2;
			}
		}
		if(down){
			temp[changehour]-=1;
			if (temp[changehour]<10*2){
				temp[changehour]=20*2;
			}
		}
		if(change0){
			changehour+=1;
			if (changehour>23){
				changehour=0;
			}
		}
		if(change1){
			ui_state=ui_changelight;
		}
		break;
	case ui_changelight:
		if(up|down){
			int byte=changehour/8;
			int pos=changehour%8;
			lightdata[byte]=(lightdata[byte]^(1<<pos));/*xor-ing the respective bit*/
		}
		if(change0){
			changehour+=1;
			if (changehour>23){
				changehour=0;
			}
		}
		if(change1){
			ui_state=ui_changelang;
		}
		break;
	case ui_changelang:
		if(up|down){/*user wanted to change something here*/
			rotate_lang(&lang);/*Changes one to the other*/
		}
		if(change1){
			ui_state=ui_showfirmware;
		}
		break;
	case ui_showfirmware:
		if(change1){
			ui_state=ui_default;
		}
		break;
	}
	/*LCD drawing switchm draws appropriate things, depending on ui state*/
	switch(ui_state){
		char h,m,s;/*hours,minutes,seconds to get*/
	case ui_default:{
			clr_lcd();/*Clearing LCD*/
			get_time(&h,&m,&s);/*carefully getting time, preventing collisions with time updating interrupt responce (see function description)*/
			lcd_time_print(h,m,s,0);/*0 - upper line*/
			if(temp_meas_error){
				text(str, TEXT_oshibka, lang);
				lcd_print(0x40,str);
			}else if(avg_temp_invalid){
				/*just printing nothing*/
			}else{
				lcd_put_char(0x46,14*16+15);/* ^0 */
				lcd_put_char(0x47,67);/* C */
				char lampsymbol, tempsymbol; /*characters, that show whether lamp is on or off and is temperature being brought down or being taken up*/
				if(relay_state(LAMP)){
					lampsymbol=42; /*starlet*/
				}else{
					lampsymbol=32; /*space*/
				}
				if(relay_state(HEATER)){
					tempsymbol=13*16+9; /*arrow up*/
				}else if (relay_state(VENTILATOR)){
					tempsymbol=13*16+10; /*arrow down*/
				}else{
					tempsymbol=15*16+4;/*just a nice symbol :)*/
				}
				lcd_put_char(0x44,lampsymbol);
				lcd_put_char(0x45,tempsymbol);
				lcd_print_positive_temp_more_than_10_less_than_100(0x40, avg_temp);/*this occupies 4 positions (see function description)*/
			}
		}
		break;
	case ui_changesec:{
			get_time(&h,&m,&s);
			lcd_time_print(h,m,s,0);
			text(str,TEXT_izmsec, lang);
			lcd_print(0x40,str);
		}
		break;
	case ui_changemin:{
			get_time(&h,&m,&s);
			lcd_time_print(h,m,s,0);
			text(str, TEXT_izmmin, lang);
			lcd_print(0x40,str);
		}
		break;
	case ui_changehr:{
			get_time(&h,&m,&s);
			lcd_time_print(h,m,s,0);
			text(str, TEXT_izmhr, lang);
			lcd_print(0x40,str);
		}
		break;
	case ui_changetemp:{
			text(str, TEXT_tempchas, lang);
			lcd_print(0,str);
			lcd_print_positive_temp_more_than_10_less_than_100(0x40,temp[changehour]);
			lcd_put_char(0x44,47);/*slash*/
			lcd_put_char(0x45,32);/*space*/
			if(changehour<10){
				lcd_put_char(0x46,48);/*zero*/
				lcd_num_print(0x47,changehour);
			}else{
				lcd_num_print(0x46,changehour);
			}
		}
		break;
	case ui_changelight:{
			clr_lcd();/*Clearing LCD*/
			text(str, TEXT_lightchas, lang);
			lcd_print(0,str);
			int byte=changehour/8;
			int pos=changehour%8;
			if(lightdata[byte]&(1<<pos)){
				text(str, TEXT_esi, lang);
				lcd_print(0x40,str);
			}else{
				text(str, TEXT_net, lang);
				lcd_print(0x40,str);
			}
			lcd_put_char(0x45,32);/*space*/
			if(changehour<10){
				lcd_put_char(0x46,48);/*zero*/
				lcd_num_print(0x47,changehour);
			}else{
				lcd_num_print(0x46,changehour);
			}
		}
		break;
		case ui_changelang:{
			clr_lcd();/*Clearing LCD*/
			text(str, TEXT_jazik, lang);
			lcd_print(0,str);
			text(str, TEXT_thislang, lang);
			lcd_print(0x40,str); /*Whis will print language info, because
			it itself depends on language, choosen*/
		}
		break;
		case ui_showfirmware:{
			clr_lcd();/*Clearing LCD*/
			text(str,TEXT_proshivka, lang);
			lcd_print(0x00,str);/*That was title in the upper line*/
			/*printing firmware version*/
			char pos=0x40;/*Next empty screen position*/
			pos+=lcd_num_print(pos, Ver0);
			lcd_put_char(pos,46);/*dot*/
			pos+=1;
			pos+=lcd_num_print(pos, Ver1);
			lcd_put_char(pos,46);/*dot*/
			pos+=1;
			lcd_num_print(pos, Ver2);
		}
		break;
	}

	/*Trouble beeper*/
	if(temp_meas_error){
		beep_state+=1;
		if(beep_state>1)beep_state=0;
		if(beep_state){
			beepon();
		}else{
			beepoff();
		}
	}else{
		beepoff();
	}

	/*LCD reinitializator (This is a hack - it is needed because the
	*piece of hardware - LCD, attached to the only device, present at
	*the moment (14.08.2009), happen to get nuts upon AC disturbances).*/
	lcd_reinit_counter+=1;
	if(lcd_reinit_period==lcd_reinit_counter){
		lcd_reinit_counter=0;
		initialize_lcd();
	}
}
/*##################################*/












/* ######VV###### therm quasi-object ##########*/
/*here come definitions of a structure and some methids, that
*work with this structure in object-like fashion.
*together, they implement temperature measuring*/

#define STATE_NEED_TO_START_CONVERSION 0
#define STATE_TEMP_CONVERTING 1
/*State 0 means, that we need to ask termometer measure temperature to
download it's value. If an error occurs, state is set to this one, because
we have lost the track of thermometer states .An example, justifying this
design, is a situation, when thermometer is physically unplugged,
while measuring temperature. Any temperature value, if one has been
measured, is lost in this case.

State 1 means, that we have asked termometer to measure temperature,
and nothing suspicious has happened to it until now.*/

char thermo_state=STATE_NEED_TO_START_CONVERSION;/*set to zero at ATmega reset/start*/
/**
 * This is a high-level method for checking temperature.
 * Unlike it's low-level analog get_temperature(),
 * nothing except this method is needed to call to acquire temperature value.
 * This method reports a temperature value, if one is ready for yielding.
 * If it is not, the method starts temperature yielding process and returns
 * special return code, signalizing that the user should come later to check the value.
 *
 * @param	*come_later - set to not 0, if the user need to check the temperature later,
 * 			set to 0, if a temperature value has been reported
 * @param	*t - temperature in degrees C, multiplied by 2.
 * @return 0 if OK,
 *           1,2,3,4,... on hardware errors.
 *
 * If return code is not null, other values are undefined,
 * otherwise if come_later is not null, t undefined,
 * otherwise t is set.
 *
 */

char thermo_get_temperature(char* come_later, char* t){
	char r;
	char c;
	char t_;
	switch(thermo_state){
		case(STATE_NEED_TO_START_CONVERSION):/*We have some old value, need to start getting a new one*/
		r=ask_termometer_start_temperature_conversion();
		if(r){
			/*errors :(*/
			return r;
		}else{
			*come_later=1;
			thermo_state=STATE_TEMP_CONVERTING;
			return 0;
		}
		case(STATE_TEMP_CONVERTING):/*we have asked getting a new temp value,
									but we don't know, if we are ready*/
		r=get_term_control_register(&c);/*getting termometer control register*/
		if(r){
			thermo_state=STATE_NEED_TO_START_CONVERSION;
			return r;/*trouble*/
		}
		if(!(c&bits(1,0,0,0,0,0,0,0))){
			/*not ready yet*/
			*come_later=2;
			return 0;
		}
		/*getting real temperature*/
		r=get_temperature(&t_);
		if(r){
			/*errors*/
			thermo_state=STATE_NEED_TO_START_CONVERSION;
			return r;
		}else{
			/*all right*/
			*come_later=0;
			*t=t_;
			/*value read and now obsolete*/
			thermo_state=STATE_NEED_TO_START_CONVERSION;
			return 0;
		}
	}
	thermo_state=STATE_NEED_TO_START_CONVERSION;
	return 0xff;/*0_o who could have written something strange to thermo_state*/
}



/*##^^################# end of section ##########*/



/*#__######Power-down protection############*/
/**
 * This section contains functions and variables, designed
 * to cope with a situation, of AC power going down. The schema
 * of the device has a reserve power source, 3x1.5V
 * battery, that will provide power for some time
 * (70ma*h battery - about 2 days at 3 ma). Our task is to try to
 * minimize power consumption during no-power period.
 **/

/**
 * This variable is normally 1, meaning that we are
 * running on AC power. As soon as we detect that the AC power has
 * gone down, we set this variable to 0. When we detect the opposite,
 * we set it back to 1. The following subsystem, that control power
 * state and switches the wariable. This variable is set and used from
 * main ~thread, not from interrupts: it need to be volatile not.
 **/
char power_on = 0xff;


/**
 * We have a MOSFET transistor with gate, connected directly to
 * AC power adaptor out, source connected to GND, drain,
 * connected to PD0 and PD1 via 10K resistors. The device is good
 * for measuring power level. In addition, it isolates microcontroller
 * logics from power supply, as it would be, if we connected PD0
 * directly to power adapter out. This is done for protection.
 * The method checks, whether we are running on external power or not.
 * @return boolean answer to the question
 */
char is_on_external_power(void){
	PORTD=PORTD|bits(0,0,0,0,0,0,1,0);/*power to D1 pin*/
	/*100 KHz time interval must be enough for levels to establish - characteristical capacity of the device is 50 pf*/
	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");/*10 times no operation instruction*/
	char answer=~PIND&bits(0,0,0,0,0,0,0,1);
	PORTD=PORTD&bits(1,1,1,1,1,1,0,1);/*off power on D1*/
	return answer;
}


/**
 * Power-controlling state-machine has three main group states (excluding
 * countd-down substates):
 */
enum power_manager_state {
	PMS_OFF, /* No power */
	PMS_ON, /* Power */
	PMS_COUNTDOWN /* Power came reecntly, waiting */
};

/**
 * Power manager behaves like a state machine. It issues commands after
 * each pass, here they are:
 */
enum power_manager_command {
	PMC_IDLE,/* Do nothing */
	PMC_SUSPEND,/* Prepare for sleeping without external power */
	PMC_RESUME/* Resume from sleeping */
};


/**
 * When external power returns, it's unwise to resume operation immediately.
 * So, let's wait this number of cycles before wake-up.
 */
#define POWERON_DELAY 10 /* less than 255 since 'char' is used to store the counter */
/**
 * Power manager state machine states
 */
enum power_manager_state pms = PMS_ON;
/**
 * This variable is set to POWERON_DELAY, when loosing power, and counted
 * down to zero before returning to normal operation after power comes
 * back.
 */
char poweron_delay_cycles_left;

/**
 * This function implements power_manager state machine
 * It uses 'pms', 'poweron_delay_cycles' global variables as state
 * machine state
 * @param power_on - external power on or off (boolean)
 * @return command of power management
 */
enum power_manager_command power_manager_werde(char ext_power){
	enum power_manager_command command = PMC_IDLE; /* If no actions specified, IDLE */
	switch(pms){
		case PMS_OFF:
			if(ext_power){
				pms = PMS_COUNTDOWN;
				poweron_delay_cycles_left = POWERON_DELAY;
			}
		break;
		case PMS_ON:
			if(!ext_power){
				pms = PMS_OFF;
				command = PMC_SUSPEND;
			}
		break;
		case PMS_COUNTDOWN:
			if(ext_power){
				if(poweron_delay_cycles_left){
					poweron_delay_cycles_left -= 1;
				}else{
					command = PMC_RESUME;
					pms = PMS_ON;
				}
			}else{
				/* Power went away when counting down :( */
				pms = PMS_OFF;
			}
		break;
	}
	return command;
}


/**
 * The method checks whether we are running on external power or not,
 * and takes appropriate actions.
 * It s supposed to be called from main loop (in main() method).
 */
void check_ac_power(void){
	char ext_power = is_on_external_power();
	/* Asking power manager state machine what to do */
	enum power_manager_command command =
		power_manager_werde( ext_power );
	/* Doing it */
	if(PMC_SUSPEND == command){
		thermo_state=STATE_NEED_TO_START_CONVERSION;/*After the power comes back, the last
		termometer reading will be obsolete.*/
		temperature_studying_reset();/*resetting temperature measurements collector, because
		collected values may be obsolete, as power comes back*/
		set_avg_temp_invalid();/*don't display old temperature info*/
		/*Disabling heater, ventillator and lamp - we don't know, when the power will be back,
		and what states the devicess will need to have. If we don't do this, the devces will turn on
		just after the power comes back and stay so, until enough temperature measurements will have been collected.*/
		relay(LAMP,0);
		relay(HEATER,0);
		relay(VENTILATOR,0);
		ledoff();
		beepoff();
		lcd_light(0);
		MCUCR=(MCUCR|bits(0,0,1,1,0,0,0,0))&bits(0,1,1,1,1,1,1,1);/*setting a deep sleep mode (see ATmel manual)*/
		power_on = 0;
	}else if(PMC_RESUME == command){
		/* last time it was off */
		initialize_lcd();
		MCUCR = MCUCR & bits(0,1,0,0,1,1,1,1);/*setting a non-deep sleep mode (see ATmel manual)*/
		power_on = 0xff;
	}else{
		/* Do nothing 8) */
	}
}/*Ja malenkiy chervaczek  __/\/\/00  {########} \_/   */
/*#^^#######################################*/



/**
 * A purely mathematical routine, that rounds the given integer up to specified number of zeroes
 * and takes away these zeroes.
 * @param
 *			n - processed integer
 *			by - number from {10,100,1000,10000...}
 * @return
 *			processed number
 */
long round_and_remove_zeroes(long n,long by){
	long delim=n/by*by+by/2;
	if(n>delim){
		return n/by+1;
	}else{
		return n/by;
	}
}


/*########Temperature-studying structure##########*/
/*The structures are devoted to reducing possible termometer noise
by providing means of calculating average measurement*/
/*8 measurements produce one output*/
#define T_N 32
char measurements[T_N]; /*array, that will hold the values*/
unsigned char next_empty=0;/*cells 0..next_empty-1 hold valid experimental values*/
unsigned char valid[4]; /*Bits of this array identify, whether the
mesaurement has been correct. The array have 32 bits, enough for T_N=32*/
/**
 * Looks into 'valid' array and returns boolean value of the requested bit.
 * @param n_bit - bit offset, value in 0..T_N-1
 * @return 0 if the bit = 0, some non-zero value, if bit = 1
 */
char is_valid(char n_bit){
	unsigned char byte_offset = n_bit/8;
	unsigned char local_bit_offset=n_bit % 8;
	return valid[byte_offset] & (1<<local_bit_offset);/*masked properly*/
}

/**
 * Sets 'valid' array bit to the specified boolean value.
 * @param val - boolean value to be set
 * @param n_bit - bit to be set, value from 0..T_N-1
 */
void set_valid(char val, char n_bit){
	unsigned char byte_offset = n_bit/8;
	unsigned char local_bit_offset=n_bit % 8;
	if(val){/*setting bit to 1*/
		valid[byte_offset]|=1<<local_bit_offset;
	}else{/*setting bit to 0*/
		valid[byte_offset]&=~(1<<local_bit_offset);
	}
}


/*if there have been more than T_ERR faulty experimental results, the whole serie is invalid*/
#define T_ERR 4

/**
 * resets studier
 **/
void temperature_studying_reset(void){
	next_empty=0;
}

/**
 * Appends an experimental value to values heap
 * @param in_value - new termperature reading (must be from 5C to 45 C,
 * 		otherwise, this is believed to be an error, because we are buisy with
 * 		an aquarium)
 * @param inc_valid - if the measurement has been valid or not
 * 		(if not, in_value may be random)
 *
 * @return
 *		 0 - experimental data loaded, awaiting more
 *		 1 - experimental data now enough, the value can't be added
 */
char t_stud_next(char in_value, char inc_valid){
	if(T_N==next_empty){/*enough data stored*/
		return 1;
	}else{/*need more data*/
		if(inc_valid){
			if((in_value<=40*2)&&(in_value>=5*2)){
				measurements[next_empty]=in_value;
				set_valid(1,next_empty);
			}else{/*suspicious reading*/
				set_valid(0,next_empty);
			}
		}else{
			set_valid(0,next_empty);
		}
		next_empty+=1;
		return 0;
	}
}

/**
 * Yields average, if it is ready. After yielding, resets the structures.
 * @param *t - output temperature
 * @param *fault - whether or not the whole serie has been correct
 * 		(disconnected termometer is supposed to lead to an incorrect
 * 		serie yielding)
 * @return 	0 - result yielded
 *			1 - need more data (t and fault undefined)
 */
char t_get_average(char* t, char* fault){
	if(T_N>next_empty){
		return 1;
	}else{
		next_empty=0;
		unsigned char j;
		char faulty_counter=0;
		long summ=0;
		for(j=0;j<T_N;j++){
			if(is_valid(j)){/*good measurement, good measurements are from 5*2 to 45*2 - see some lines below , that's why we may not care about signs dC*/
				summ+=measurements[j];
			}else{/*faulty measurement*/
				faulty_counter+=1;
			}
		}
		if(faulty_counter>T_ERR){
			/*Achtung! Too many faulty measurements!*/
			*fault=1;
			return 0;
		}
		*t=(char)round_and_remove_zeroes(1000*(summ)/(T_N-faulty_counter),1000);
		*fault=0;
		return 0;
	}
}

/*################################################*/


#define DEFAULT_OK_TEMPERATURE (20*2) /*20 C*/
/**
 * Loads default good values into configurable state variables
 **/
void config_load_defaults(void){
	/* light */
	lightdata[0] = bits(0,0,0,0,0,0,0,0); /* 0-7 */
	lightdata[1] = bits(1,1,1,1,1,1,0,0); /* 8-15 */
	lightdata[2] = bits(0,0,1,1,1,1,1,1); /* 16-23 */
	/* temperature */
	unsigned char ksi;
	for(ksi=0;ksi<24;ksi+=1){
		temp[ksi] = DEFAULT_OK_TEMPERATURE;
	}
	/* language */
	lang = 0;
}


#define CRC32_POLYNOMIAL 0x04C11DB7
/**
 * Back-ups/restores temperature, light configurations and language
 * global variables to/from EEPROM.
 *
 * @param direction: 0 - save to EEPROM,
 *					any other - load data from EEPROM.
 * @param interr: after completion interrupts shall be left true - enabled, false - disabled.
 **/
void eeprom_backup_or_restore_configs(char direction, char interr){
	asm("cli");
	struct CRC32 crc;
	crc32_init(&crc, CRC32_POLYNOMIAL);
	if(direction){
		/*restore from EEPROM*/
		unsigned int addr=0;/*next empty address*/
		unsigned char ksi;
		/* light */
		for(ksi=0;ksi<3;ksi++){
			unsigned char c = EEPROM_read(addr);
			crc32_update(&crc, c);
			lightdata[ksi]=c;
			addr+=1;
		}
		/* temperature */
		for(ksi=0;ksi<24;ksi++){
			char val=EEPROM_read(addr);
			if((val>40*2)||(val<10*2)){
				/*We've just read something strange from EEPROM. (May be it's too old and beaten and can't store data?? Or there is just some random data in the place.)*/
				temp[ksi]=DEFAULT_OK_TEMPERATURE;
			}else{
				temp[ksi]=val;
			}
			crc32_update(&crc, val);
			addr+=1;
		}
		/* locale */
		unsigned char lang_e = EEPROM_read(addr);
		addr+=1;
		crc32_update(&crc, lang_e);
		if(lang_e<NUM_OF_LANG){/*That's a good value*/
			lang=lang_e;
		}else{
			/*Some garbage in EEPROM*/
			lang=0;
		}
		/* crc32 */
		for(ksi=0;ksi<4;ksi+=1){
			unsigned char c = EEPROM_read(addr);
			addr += 1;
			crc32_update(&crc, c);
		}
		if(!crc32_check_zero(&crc)){
			/* bad crc! */
			config_load_defaults();
			eeprom_backup_or_restore_configs(0,0); /* saving the new OK values */
		}
	}else{
		/*back up to EEPROM*/
		unsigned int addr=0;/*next empty address*/
		unsigned int ksi;
		for(ksi=0;ksi<3;ksi++){
			char val = EEPROM_read(addr);
			char nval = lightdata[ksi];
			if(val!=nval)EEPROM_write(addr,nval);
			crc32_update(&crc, nval);
			addr+=1;
		}
		for(ksi=0;ksi<24;ksi++){
			char val = EEPROM_read(addr);
			char nval = temp[ksi];
			if(val!=nval)EEPROM_write(addr,nval);
			crc32_update(&crc, nval);
			addr+=1;
		}
		/*saving locale*/
		char lang_e=EEPROM_read(addr);
		if(lang!=lang_e){/*if value there differs...*/
			EEPROM_write(addr,lang);
		}
		crc32_update(&crc, lang);
		addr+=1;
		/* saving crc32 */
		crc32_finalize(&crc);
		for(ksi=0;ksi<4;ksi+=1){
			char val = EEPROM_read(addr);
			char nval = crc32_get_reversed(&crc, ksi);
			if(val!=nval)EEPROM_write(addr,nval); /* this might just save us some tiny EEPROM durability */
			addr += 1;
		}
	}

	if(interr){/* Depending on where we have been asked to leave interrupts
				* enabled or not */
		asm("sei");
	}
}






/**
*This method checks, if each external device should be turned off or on, and does it.
*In addition, it updates displayed temperature values @ UI
*/
void switch_external_devices(void){
	char h,m,s; /*hours, minutes and seconds*/
	get_time(&h,&m,&s);

	if(lightdata[h/8]&(1<<(h%8))){
		relay(LAMP,1);
	}else{
		relay(LAMP,0);
	}
	/*termometer - switching section*/
	char t,fault;
	char res=t_get_average(&t,&fault);/*t is positive and varies from ~5 to ~45 C, this is ensured in averager*/
	if(!res){
		/*Some data ready!*/
		if(fault){/*Achtung! We could not surely determine temperature data.*/
			temp_meas_error=1;/*Let UI warn people around about this trouble*/
			relay(HEATER,0);
			relay(VENTILATOR,0);
		}else{/*Temperature yielded*/
			avg_temp=t;
			avg_temp_invalid=0;
			temp_meas_error=0;
			{/*switching relays*/
				unsigned char h_ind=(unsigned char)h;/*hours now, cast is made to keep gcc happy*/
				if  (temp[h_ind]==t){
					/*Temperature OK*/
					relay(HEATER,0);
					relay(VENTILATOR,0);
				}else if(temp[h_ind]+1==t){/*one above OK*/
					/*hesteresis*/
					if(relay_state(VENTILATOR)){
						/*let it rotate*/
					}else{
						relay(HEATER,0);
						relay(VENTILATOR,0);/*ensuring, all is shut down*/
					}
				}else if(temp[h_ind]-1==t){
					/*hesteresis*/
					if(relay_state(HEATER)){
						/*let it heat*/
					}else{
						relay(HEATER,0);
						relay(VENTILATOR,0);/*ensuring, all is shut down*/
					}
				}else if(temp[h_ind]+1<t){
					relay(HEATER,0);
					relay(VENTILATOR,1);
				}else if(temp[h_ind]-1>t){
					relay(HEATER,1);
					relay(VENTILATOR,0);
				}
			}/*switching relays*/
		}

	}
	/*termometer - obtaining a new value section*/
	/*we don't worry about adding to full t_stud_next, because if it were full, it would be emptied in the section before (and we have overflow protection in t_stud_next)*/
	char come_later;
	char retc=thermo_get_temperature(&come_later,&t);
	if(retc){
		t_stud_next(0,0);/*0_o some error*/
	}else if(come_later){
		/*don't add anything*/
	}else{
		t_stud_next(t,1);
	}
}



/**
* Perform some microcontroller setups. Called at microcontroller setup.
* This is an Ancient method. It was created just after main().
*/
void configure_mu(void){
	MCUCR|=(3<<ISC00);/*rising edge triggers 0 interruption*/
	GICR|=1<<INT0;/*enabling 0 interrupt*/

	MCUCR|=1<<SE;/*Enabling sleep by setting sleepy bit*/
	/*Setting TWI bit rate*/
	TWBR=10;
	TWSR=(TWSR&bits(1,1,1,1,1,1,0,0))+3;

	/*setting timer_counter 2, in compliance with ATmel manual (setting ATmega to
	work in asynchronous mode section)*/
	TIMSK&=bits(0,0,1,1,1,1,1,1);/*clearing interrupts enable flags
	(not to trigger them occasionally, while configuring the counter).*/
	ASSR|=1<<AS2;/*setting asynchronious operation mode*/
	TCNT2=0x00;
	OCR2=0x00;
	TCCR2=bits(0,0,0,0,0,1,0,1);/*count up to 0xff value; OC2 pin works normal, as port; divide clock by 128*/
	while(ASSR&((1<<TCR2UB)+(1<<OCR2UB)+(1<<TCN2UB))){/*do nothing*/};/*wait until these become zero - counter is not busy*/
	TIFR&=bits(0,0,1,1,1,1,1,1);/*clearing interrupt flags*/
	TIMSK=(bits(0,0,1,1,1,1,1,1)&TIMSK)+(1<<TOIE2);/*enabling overflow interruption*/

	/*DDRA configuerd in initialize_lcd()*/
	DDRB=bits(0,0,0,1,1,1,1,1);
	DDRD=bits(0,0,0,0,0,0,1,0);/*PORTD 1 is an output, used to power the device, measuring the presence of external AC power*/
	DDRC=1<<2;/*LED blinker*/
	/*The whole PORTD is configured for inut*/

	/*configuring Watchdog

	/-\
	|
	\=-----^^00__
		/////       D
	| ///  ---^^
	II  II
	*/
	WDTCR=bits(0,0,0,0,1,1,1,1);/*a bit more that 2 seconds of dog tolerance*/
}





int main(void){
	configure_mu();	/*setting up
					*some microcontroller features*/
	initialize_lcd();/*setting up LCD*/
	beepon();
	if(check_termo_configs()){
		/* Could not initialize thermometer! */
		/* Let's say something and go for reset */
		clr_lcd();
		char lng = LANG_RU;
		while(1){
			text(str, TEXT_waitforter, lng);
			lcd_print(0,str);
			text(str, TEXT_mometr, lng);
			lcd_print(0x40,str);
			rotate_lang(&lng);
			sleep_millis(400);
		}
		/* We will be biten by the watchdog now - the best solution */
	}
	eeprom_backup_or_restore_configs(1,0);/* load from EEPROM */
	beepoff();
	{
		/* delay + melody =) */
		unsigned int delay_left = 1000;
		unsigned int delay_next = 200;
		while(delay_left){
			unsigned int delay = delay_next > delay_left ? delay_left : delay_next;
			delay_left -= delay;
			delay_next = delay_next * 7 / 8;
			beepon();
			sleep_millis(delay/2);
			beepoff();
			sleep_millis(delay/2);
		}
	}
	asm("sei");
	/*This is the main loop. This thread executes in "sei" mode, except
	little patches, where interruptions are disabled*/
	for(;;){
		check_ac_power();/*look on Ualpha level, look, if it used to change before, if it's now OK, set power_on variable, restart LCD, if necessary.*/
		if(power_on){/*If there is no power, we must not do anything. Just react to the only enabled interruption - every-second clock
						from the asynchronous timer with a 32768Hz quartz attached*/
			ledon();/*light on*/
			switch_external_devices();/*issue commands to relays, most time-consuming (because of TWI termometer)*/
			char cf0,cf1,cf2,cf3;
			asm("cli");
			cf0=flag0;/*copying current flag values*/
			cf1=flag1;
			cf2=flag2;
			cf3=flag3;
			flag0=0;
			flag1=0;
			flag2=0;
			flag3=0;
			asm("sei");

			werde(cf0,cf1,cf2,cf3);/*Change main interface states, depending on button data, and
									 issue some commands depending on the state and input*/
			ledoff();
		}/*if power_on flag*/
		asm("wdr");/*resetting watchdog*/
		asm("sleep");/*sleeps, until timer overflows or button is pressed, or both*/
	}/*main loop*/
	return 0;/*never reached upon normal operation*/
}

