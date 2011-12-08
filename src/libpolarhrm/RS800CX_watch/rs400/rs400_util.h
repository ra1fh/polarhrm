/************************************************************************
This file is part of RS400 Tools

RS400 Tools is Copyright (C) 2007 Matti Juvonen <mpj@iki.fi>
Portions Copyright (C) 2006 Jani Hursti <jani.hursti@hut.fi>,
Portions Copyright (C) 2001-2003 Tom Oliveira e Silva

RS400 Tools is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

RS400 Tools is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#ifndef _RS400_UTIL_H
#define _RS400_UTIL_H

int rs400_decode_string(unsigned char result[], unsigned char rawdata[]);
int rs400_encode_string(unsigned char result[], unsigned char string[]);

unsigned int bcd2i(int bcd);

double hms2sec(unsigned char h, unsigned char m, unsigned char s);
double hmst2sec(unsigned char h, unsigned char m, unsigned char s, unsigned char t);
double hmsc2sec(unsigned char h, unsigned char m, unsigned char s, unsigned char c);
double bcd_hms2sec(unsigned char pos[]);
double bcd_hmsc2sec(unsigned char pos[]);
double short2sec(unsigned char pos[]);
unsigned short toshort(unsigned char pos[]);
unsigned int pace(unsigned char pos[]);
void timetostr(char str[], double time);

#endif
