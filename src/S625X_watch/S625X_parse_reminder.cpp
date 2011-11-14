/***************************************************************************
 *            s625X_parse_reminder.c
 *
 *  Thu May 20 17:15:05 2011
 *  Copyright  2011  Thomas Foyth
 *  <thomas.foyth@gmail.com>
 ****************************************************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 

#include <stdio.h>
#include <string.h>
#include "../polarhrm.h"
#include "../watch_struct.h"
#include "../watch_util.h"
#include "s625X_parse.h"


/*

Subtype 0x0e: "Get reminder" response: Length = 19 bytes (14 payload):
-----------------------------------------------------------------------------
Payload byte	Meaning
=============================================================================
0  		which one (0 = 1, etc up to 6 = 7)
1  		minute (BCD)
2  		hour (BCD)
3  		day of month (BCD)
4  		year (from 2000) (BCD)
5  		L: month 1 = jan, etc.
    		U: 0 = on, 2 = off
6  		text byte 1 (see section 4)
7  		text byte 2
8  		text byte 3
9  		text byte 4
10 		text byte 5
11 		text byte 6
12 		text byte 7
13 		L: exercise (0 = none, 1 = basic, 2-6 = ex set 1-5)
    	U: repeat (0 = off, hourly, daily, weekly, monthly, yearly)




set reminder 1
Reminder in use: TRUE
Date: 09.10.2011
Time: 8:30
Text: GRAZ-M 
Repeat: off
Exercise: none
00 0d 00 30 08 09 11 0a 11 1c 0b 24 3f 17 0a 00 
                    
set reminder 2
Reminder in use: TRUE
Date: 09.10.2011
Time: 8:30
Text: GRAZ-M 
Repeat: hourly
Exercise: none	

set reminder 2
Reminder in use: TRUE
Date: 09.10.2011
Time: 8:30
Text: GRAZ-M 
Repeat: hourly
Exercise: none
00 0d 01 30 08 09 11 0a 11 1c 0b 24 3f 17 0a 10        

get reminder 
length 15 bytes
E 1 30 8 9 11 A 11 1C B 24 3F 17 A 10 0 
 

set reminder 3 // Vienna Marathon 15.04.2012
Reminder in use: TRUE
Date: 15.04.2012
Time: 6:00
Text: ViennaM 
Repeat: monthly
Exercise: BasicUse
00 0d 02 00 06 15 12 04 20 2d 29 32 32 25 17 41                                                  ........ -)22%.A


set reminder 4 // Vienna Marathon 15.04.2012
Reminder in use: FALSE
Date: 15.04.2012
Time: 6:00
Text: ViennaM 
Repeat: monthly
Exercise: BasicUse
00 0d 03 00 06 15 12 24 20 2d 29 32 32 25 17 41


Reminder in use: TRUE
Date: 15.04.2012
Time: 6:00
Text: ViennaM 
Repeat: monthly
Exercise: E1
00 0d 03 00 06 15 12 04 20 2d 29 32 32 25 17 42

*/


watch_reminder s625X_get_watch_reminder_parse(unsigned char buf[],int len) {

watch_reminder w_rem;	
unsigned char rawdata[7];
unsigned char encdata[7];

// copy bytes to rawbuffer and encode 
	memcpy(rawdata, &buf[7], 7);
	
// DEBUG print before encoding 
#if defined(DEBUGPRINT)
	int i=0;
	printf("data before encoding\n");
	for (i=0;i<=7;i++)
	printf("%X ",rawdata[i]);
	printf("\n");
#endif
	
	watch_decode_string(encdata, rawdata);

// don't forget string terminator 
// do we need this here?
// looks like we don t need it! 22.05.2011 printf %s does well :-)
//	encdata[7]='\0';


// DEBUG print after encoding 
#if defined(DEBUGPRINT)
	printf("data after encoding\n");
	for (i=0;i<=7;i++)
	printf("%X ",encdata[i]);
#endif
	
	// reminder number 
	w_rem.rem_number=buf[1];
	
	w_rem.rem_on=UNIB(buf[6]);
	
    // not used 
	// w_rem.wdate.tm_sec;

	w_rem.wdate.tm_min=bcd2i(buf[2]);
	w_rem.wdate.tm_hour=bcd2i(buf[3]);
	w_rem.wdate.tm_mday=bcd2i(buf[4]);
	
	w_rem.wdate.tm_mon=LNIB(buf[6]);
	w_rem.wdate.tm_year=2000+bcd2i(buf[5]);

	// not used 
	//w_rem.wdate.tm_wday;
	
    // to <- form 
    strcpy(w_rem.rem_label,(char*) encdata);

	//U: repeat (0 = off, hourly, daily, weekly, monthly, yearly))	
	w_rem.rem_repeat=UNIB(buf[14]);	
	
	//L: exercise (0 = none, 1 = basic, 2-6 = ex set 1-5)
    w_rem.rem_exercise=LNIB(buf[14]);

return w_rem;

	
}