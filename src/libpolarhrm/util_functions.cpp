// util_functions.cpp
//
// Copyright (C) 2011 - Thomas Foyth
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <cctype>

#include "util_functions.h"



unsigned char bcd(unsigned char dec)
{
	return ((dec/10)<<4)+(dec%10);
}


unsigned char unbcd(unsigned char x){
	return (unib(x)*10 + lnib(x));
}


unsigned char unib(unsigned char x){
	return ((x)>>4);
}


unsigned char lnib(unsigned char x){
	return ((x)&0x0f);
}

unsigned short toshort(unsigned char pos[]) {
	return ((pos[1] << 8) | pos[0]);
}



//makes string to upper case
char * toUpperCase (char *s){
	for (int i=0; i<sizeof(s); i++) {
		if(isalpha(s[i])) {
			s[i]=(char)toupper(s[i]);
		}
	}
	return s;
}


// make decimal gps value out of 4 bytes
double toGpsDec(unsigned char pos[]){

	double coordinate;

	//FIXME not handling all data
	// what is the purpose of the upper nibble at pos[2]
	coordinate += lnib(pos[2])*0x10000;
	coordinate += pos[1]*0x100;
	coordinate += pos[0]*0x1;
	coordinate  = coordinate / 600000;
	coordinate += pos[3];

	return coordinate;
}