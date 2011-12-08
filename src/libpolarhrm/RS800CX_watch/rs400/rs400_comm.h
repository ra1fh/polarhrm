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

#ifndef _RS400_COMM_H
#define _RS400_COMM_H

#include "rs400_struct.h"

#define RS400_MAX_PACKET_SIZE 32768

#define RS400_CMD_FACTORYRESET 0x09
#define RS400_CMD_WATCHMODE 0x0a
#define RS400_CMD_GETOVERVIEW 0x15
#define RS400_CMD_CONTINUEXFER 0x16
#define RS400_CMD_GETSETTINGS 0x5a
#define RS400_CMD_SETTIME 0x5b
#define RS400_CMD_SETEVENT 0x5c
#define RS400_CMD_SETSETTINGS 0x66
#define RS400_CMD_SLEEPMODE 0x69
#define RS400_CMD_GETSESSION 0x6a

/* Initialise the tty socket */
int RS400_init(char socket[]);

int RS400_factoryreset();

int RS400_watchmode();

int RS400_sleepmode();

int RS400_get_overview();

int RS400_get_session(char id, unsigned char data[]);

int RS400_get_user(unsigned char data[]);

/* Send raw bytes to the device. Return the number of bytes written */
int RS400_sendbytes(const unsigned char query[], int size);

/* Receive raw bytes from the device. Returns the number of bytes read */
int RS400_recvbytes(unsigned char buf[]);

int RS400_close();

#endif
