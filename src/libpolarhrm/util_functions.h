/*
 * util_functions.h
 *
 * Copyright (C) 2011 - Thomas Foyth
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _UTIL_FUNCTIONS_H
#define _UTIL_FUNCTIONS_H


	unsigned char unbcd(unsigned char x);
	unsigned char unib(unsigned char x);
	unsigned char lnib(unsigned char x);
	unsigned char bcd(unsigned char dec);

	unsigned short toshort(unsigned char pos[]);

	char * toUpperCase (char *s);

	double toGpsDec(unsigned char pos[]);

	// show the content of a buffer in usbsnoop 
	// output format (makes it easy to compare)
	void print_bytes(char *buf, int len);

#endif