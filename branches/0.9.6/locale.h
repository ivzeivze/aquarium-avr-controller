/** 
 * locale.h
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



#ifndef LOCALE_H
#define LOCALE_H


/**
 * Contains a function, that fills text in buffers togethre with 
 * language and text enums. Contains helpful language-rotating function.
 * 
 * If "bla-bla-bla"
 * strings were used in code instead, these would be placed in memory
 * by gcc, what is a waste of 1K of microcontroller memory. So the texts
 * lay as instructions, until needed. 
 */

/**
 * This enum specifies text strings for use in various situations.
 * The actual content of string sepends on the language, choosen
 */
enum TEXT{
	TEXT_spaces,
	TEXT_oshibka,
	TEXT_izmsec,
	TEXT_izmmin,
	TEXT_izmhr,
	TEXT_tempchas,
	TEXT_lightchas,
	TEXT_esi,
	TEXT_net,
	TEXT_waitforter,
	TEXT_mometr,
	TEXT_proshivka,
	TEXT_thislang,
	TEXT_jazik
};


/**
 * Defining two macros, that would be languages
 **/
#define	LANG_RU 0/*Russian*/
#define LANG_EN 1/*English*/
/**
 * Defining macros, telling how many locales we have (for variable checking)
 **/
#define NUM_OF_LANG 2

/**
 * Fills text in buffer, depending on language and string specification.
 * @param buffer - place to copy string to
 * @param text_id - text number, one of macros with explaining name shall 
 * be used
 * @param language - language identifier, 0 for native (This strongly depends
 * on LCD imbedded codepage, in this particular case, the native 
 * language is Russian), 1 for international English. It could have been
 * passed as a global variable, however, this is not generally good, 
 * because of being unclear.
 */
void text(char* buffer, enum TEXT text_id, char language);



/**
 * Rotates language variable. Sets language to be different. 
 * @param lang - pointer to language variable to change
 */
void rotate_lang(char *lang);

#endif
