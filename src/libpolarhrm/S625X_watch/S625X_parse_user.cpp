/***************************************************************************
 *            s625X_parse_user.c
 *
 *  Sun May 22 16:49:42 2011
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
#include "polarhrm.h"
#include "s625X_parse.h"
#include "watch_struct.h"
#include "watch_util.h"

/*

Subtype 0x06: "Get user info" response: Length = 26 bytes (21 payload):
-----------------------------------------------------------------------------
Payload byte	Meaning
=============================================================================
0           bit 7: (& 0x80) 0
	        bit 6: (& 0x40) 0
	        bit 5: (& 0x20) 0
	        bit 4: (& 0x10) 0
	        bit 3: (& 0x08) (Altimeter)
	        bit 2: (& 0x04) (Polar Fitness Test)
	        bit 1: (& 0x02) (Predict HR Max - also turns on fitness test)
	        bit 0: (& 0x01) (Energy Expenditure)
1           bit 7: (& 0x80) 0
	        bit 6: (& 0x40) 0
	        bit 5: (& 0x20) 0
	        bit 4: (& 0x10) 0
	        bit 3: (& 0x08) (Options Lock)
	        bit 2: (& 0x04) (Help)
	        bit 1: (& 0x02) (Units: 0 [kg/cm/km], 1 [lb/ft/mi])
	        bit 0: (& 0x01) (Activity/Button Sound)
2               L: Heart Touch: Show Lim = 0, Store Lap = 1, Switch Disp = 2)
                U: Recording Interval (5s = 0, 15s = 1, 60s = 2)
3       weight (units)
4  		height (units)
5 		birth day (BCD)
6 		birth year (BCD), relative to 1900
7 		L: birth month
        U: activity level:  0 => low,
	                        1 => medium
				   			2 => high
				   			3 => top
8 		VO2 max
9 		Max HR
10 		Sex (0x50 = male, 0x51 = female)
11	 	?
12 		?
13 		User ID (0-99)
14 		Name (char 1) (see section 4)
15 		Name (char 2)
16 		Name (char 3)
17 		Name (char 4)
18 		Name (char 5)
19 		Name (char 6)
20 		Name (char 7)


send command 6 
length 24 bytes
6 6 4 0 4D AD 19 83 12 33 C3 0 17 58 1 1E 2C 33 31 25 37 A 4 0 0 


DATA is not exact the same as S710. 
 male -> female ?

*/

void s625X_get_watch_user_parse(watch_user* w_user,unsigned char buf[],int len) {

	
if (len == 24) {
 /* ignore the first 2 byte and the last two */ 
	
    w_user->weight=buf[5];
    w_user->height=buf[6];
	
    w_user->birth_day=BCD(buf[7]);
    w_user->birth_month=LNIB(buf[9]);
    w_user->birth_year=1900+BCD(buf[8]);
    w_user->hr_max=buf[11];

    w_user->hr_rest=USER_HR_REST;

	w_user->vo2_max=buf[10];

	w_user->sex[7];
	w_user->sex_bool;

//	activity_level[9];

}
		
}
 