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

#include <stdio.h>
#include <math.h>

/* TODO: Which one of these is correct */

#ifdef RS400_CHARSET_ALT
static const char rs400_charset[] =
    "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?:./*-+%()";
#else
static const char rs400_charset[] =
    "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-%/()*+.:?";
#endif

/* Decode from Polar's character set.
Returns the length of the string. */
int rs400_decode_string(unsigned char result[], unsigned char rawdata[]) {
    int i = 0;
    do {
        /* The top bit signals the last byte */
        result[i] = rs400_charset[rawdata[i] & 0x7f];
    } while (!(rawdata[i++] & 0x80));
    result[i + 1] = 0;          /* Terminate string */
    return (i + 1);
}

/* Encode to Polar's character set.
This is really slow but it's not really a problem.
Returns the length of the string. */
int rs400_encode_string(unsigned char result[], unsigned char string[]) {
    int i;
    for (i = 0; string[i] != 0; i++) {
        int j;
        for (j = 0; j < (sizeof(rs400_charset) / sizeof(char)); j++) {
            if (string[i] == rs400_charset[j]) {
                result[i] = j;
                break;
            }
        }
    }
    return i;
}

/* Convert from binary coded decimal */
unsigned int bcd2i(int bcd) {
    return ((bcd >> 4) * 10 + (bcd & 0x0f));
}

double hms2sec(unsigned char h, unsigned char m, unsigned char s) {
    /* Horner rules */
    return (((h * 60) + m) * 60) + s;
}

double hmst2sec(unsigned char h, unsigned char m, unsigned char s, unsigned char t) {
    return ((((h * 60) + m) * 60) + s) + (0.1 * t);
}

double hmsc2sec(unsigned char h, unsigned char m, unsigned char s, unsigned char c) {
    return ((((h * 60) + m) * 60) + s) + (0.01 * c);
}

double bcd_hms2sec(unsigned char pos[]) {
    unsigned char h = bcd2i(pos[2]);
    unsigned char m = bcd2i(pos[1]);
    unsigned char s = bcd2i(pos[0]);
    return hms2sec(h, m, s);
}

double bcd_hmsc2sec(unsigned char pos[]) {
    unsigned char h = bcd2i(pos[3]);
    unsigned char m = bcd2i(pos[2]);
    unsigned char s = bcd2i(pos[1]);
    unsigned char c = bcd2i(pos[0]);
    return hmsc2sec(h, m, s, c);
}

double short2sec(unsigned char pos[]) {
    unsigned char hours = pos[2] & 0x7f;
    unsigned char minutes = pos[1] & 0x3f;
    unsigned char seconds = pos[0] & 0x3f;
    /* We can just fit the tents in the extra bits */
    unsigned char tenths = ((pos[1] & 0xc0) >> 4) | ((pos[0] & 0xc0) >> 6);
    return hmst2sec(hours, minutes, seconds, tenths);
}

unsigned short toshort(unsigned char pos[]) {
    return ((pos[1] << 8) | pos[0]);
}

unsigned int pace(unsigned char pos[]) {
    int sixteenths = ((pos[1] & 0xf0) << 2) + pos[0];
    return sixteenths * 600.0 / 16.0;
}

void timetostr(char str[], double time) {
    unsigned int inttime = time;
    unsigned int tenths = round((time - inttime) * 10.0);
    unsigned int hours = inttime / 3600;
    inttime %= 3600;
    unsigned int minutes = inttime / 60;
    inttime %= 60;
    unsigned int seconds = inttime;
    sprintf(str, "%02d:%02d:%02d.%d", hours, minutes, seconds, tenths);
}

