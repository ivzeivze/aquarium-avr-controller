/** 
 * locale.c
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


#include "locale.h"




void text(char* buffer, enum TEXT text_id, char language){
	switch(language){
		case LANG_RU: /*Russian*/
			switch (text_id){
				case TEXT_spaces:
					buffer[0]=32;
					buffer[1]=32;
					buffer[2]=32;
					buffer[3]=32;
					buffer[4]=32;
					buffer[5]=32;
					buffer[6]=32;
					buffer[7]=32;
					buffer[8]=0;
					break;
				case TEXT_oshibka:
					buffer[0]=111;
					buffer[1]=193;
					buffer[2]=184;
					buffer[3]=178;
					buffer[4]=186;
					buffer[5]=97;
					buffer[6]=0;
					break;
				case TEXT_izmsec:
					buffer[0]=184;
					buffer[1]=183;
					buffer[2]=188;
					buffer[3]=99;
					buffer[4]=101;
					buffer[5]=186;
					buffer[6]=94;
					buffer[7]=94;
					buffer[8]=0;
				break;
				case TEXT_izmmin:
					buffer[0]=184;
					buffer[1]=183;
					buffer[2]=188;
					buffer[3]=94;
					buffer[4]=94;
					buffer[5]=188;
					buffer[6]=184;
					buffer[7]=189;
					buffer[8]=0;
				break;
				case TEXT_izmhr:
					buffer[0]=94;
					buffer[1]=94;
					buffer[2]=184;
					buffer[3]=183;
					buffer[4]=188;
					buffer[5]=32;
					buffer[6]=192;
					buffer[7]=99;
					buffer[8]=0;
				break;
				case TEXT_tempchas:
					buffer[0]=191;
					buffer[1]=101;
					buffer[2]=188;
					buffer[3]=190;
					buffer[4]=47;
					buffer[5]=192;
					buffer[6]=97;
					buffer[7]=99;
					buffer[8]=0;
				break;
				case TEXT_lightchas:
					buffer[0]=99;
					buffer[1]=179;
					buffer[2]=101;
					buffer[3]=191;
					buffer[4]=47;
					buffer[5]=192;
					buffer[6]=97;
					buffer[7]=99;
					buffer[8]=0;
				break;
				case TEXT_esi:
					buffer[0]=101;
					buffer[1]=99;
					buffer[2]=191;
					buffer[3]=196;
					buffer[4]=47;
					buffer[5]=0;
				break;
				case TEXT_net:
					buffer[0]=189;
					buffer[1]=101;
					buffer[2]=191;
					buffer[3]=32;
					buffer[4]=47;
					buffer[5]=0;
				break;
				case TEXT_waitforter:
					buffer[0]=163;
					buffer[1]=227;
					buffer[2]=181;
					buffer[3]=188;
					buffer[4]=84;
					buffer[5]=101;
					buffer[6]=112;
					buffer[7]=45;
					buffer[8]=0;
				break;
				case TEXT_mometr:
					buffer[0]=188;
					buffer[1]=111;
					buffer[2]=188;
					buffer[3]=101;
					buffer[4]=191;
					buffer[5]=112;
					buffer[6]=33;
					buffer[7]=0;
				break;
				case TEXT_proshivka:
					buffer[0]=168;
					buffer[1]=112;
					buffer[2]=111;
					buffer[3]=193;
					buffer[4]=184;
					buffer[5]=179;
					buffer[6]=186;
					buffer[7]=97;
					buffer[8]=0;
				break;
				case TEXT_jazik:
					buffer[0]=177;
					buffer[1]=183;
					buffer[2]=195;
					buffer[3]=186;
					buffer[4]=58;
					buffer[5]=0;
				break;
				case TEXT_thislang:
					buffer[0]=80;
					buffer[1]=121;
					buffer[2]=99;
					buffer[3]=99;
					buffer[4]=186;
					buffer[5]=184;
					buffer[6]=185;
					buffer[7]=0;
				break;
			}
		break;
		case LANG_EN:/*English*/
			switch (text_id){
				case TEXT_spaces:/*Spaces are such spaces! In any language*/
					buffer[0]=32;
					buffer[1]=32;
					buffer[2]=32;
					buffer[3]=32;
					buffer[4]=32;
					buffer[5]=32;
					buffer[6]=32;
					buffer[7]=32;
					buffer[8]=0;
					break;
				case TEXT_oshibka:
					/*Error*/
					buffer[0]=69;
					buffer[1]=114;
					buffer[2]=114;
					buffer[3]=111;
					buffer[4]=114;
					buffer[5]=0;
					break;
				case TEXT_izmsec:
					/*chgsec^^*/
					buffer[0]=99;
					buffer[1]=104;
					buffer[2]=103;
					buffer[3]=115;
					buffer[4]=101;
					buffer[5]=99;
					buffer[6]=94;
					buffer[7]=94;
					buffer[8]=0;
				break;
				case TEXT_izmmin:
					/*chg^^min*/
					buffer[0]=99;
					buffer[1]=104;
					buffer[2]=103;
					buffer[3]=94;
					buffer[4]=94;
					buffer[5]=109;
					buffer[6]=105;
					buffer[7]=110;
					buffer[8]=0;
				break;
				case TEXT_izmhr:
					/*^^chg hr*/
					buffer[0]=94;
					buffer[1]=94;
					buffer[2]=99;
					buffer[3]=104;
					buffer[4]=103;
					buffer[5]=32;
					buffer[6]=104;
					buffer[7]=114;
					buffer[8]=0;
				break;
				case TEXT_tempchas:
					/*temp/ hr*/
					buffer[0]=116;
					buffer[1]=101;
					buffer[2]=109;
					buffer[3]=112;
					buffer[4]=47;
					buffer[5]=32;
					buffer[6]=104;
					buffer[7]=114;
					buffer[8]=0;
				break;
				case TEXT_lightchas:
					/*lght/ hr*/
					buffer[0]=108;
					buffer[1]=103;
					buffer[2]=104;
					buffer[3]=116;
					buffer[4]=47;
					buffer[5]=32;
					buffer[6]=104;
					buffer[7]=114;
					buffer[8]=0;
				break;
				case TEXT_esi:
					/*on*/
					buffer[0]=111;
					buffer[1]=110;
					buffer[2]=0;
				break;
				case TEXT_net:
					/*off*/
					buffer[0]=111;
					buffer[1]=102;
					buffer[2]=102;
					buffer[3]=0;
				break;
				case TEXT_waitforter:
					/*Waitfort*/
					buffer[0]=87;
					buffer[1]=97;
					buffer[2]=105;
					buffer[3]=116;
					buffer[4]=102;
					buffer[5]=111;
					buffer[6]=114;
					buffer[7]=116;
					buffer[8]=0;
				break;
				case TEXT_mometr:
					/*ermometr*/
					buffer[0]=101;
					buffer[1]=114;
					buffer[2]=109;
					buffer[3]=111;
					buffer[4]=109;
					buffer[5]=101;
					buffer[6]=116;
					buffer[7]=114;
					buffer[8]=0;
				break;
				case TEXT_proshivka:
					/*Firmware*/
					buffer[0]=70;
					buffer[1]=105;
					buffer[2]=114;
					buffer[3]=109;
					buffer[4]=119;
					buffer[5]=97;
					buffer[6]=114;
					buffer[7]=101;
					buffer[8]=0;
				break;			
				case TEXT_jazik:
					/*Lang:*/
					buffer[0]=76;
					buffer[1]=97;
					buffer[2]=110;
					buffer[3]=103;
					buffer[4]=58;
					buffer[5]=0;
				break;
				case TEXT_thislang:
					/*English*/
					buffer[0]=69;
					buffer[1]=110;
					buffer[2]=103;
					buffer[3]=108;
					buffer[4]=105;
					buffer[5]=115;
					buffer[6]=104;
					buffer[7]=0;
				break;
			}
		break;
	}
}








void rotate_lang(char *lang){
	switch (*lang){
		case LANG_EN:
			*lang=LANG_RU;
			break;
		case LANG_RU:
			*lang=LANG_EN;
			break;
		default:
			*lang=LANG_RU;/*fix, just in case...*/
	}
}



