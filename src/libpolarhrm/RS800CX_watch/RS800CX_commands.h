/*
 * RS800CX_commands.h
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

#ifndef _RS800CXCOMMANDS_H
#define _RS800CXCOMMANDS_H



enum RS800CXcommands {
  RS800CX_GET_OVERVIEW = 0x15,
//  RS800CX_GET_USER = 0x06,
//  RS800CX_SET_USER = 0x05,
  RS800CX_CMD_FACTORYRESET = 0x09,
  RS800CX_CMD_WATCHMODE = 0x0A,
  RS800CX_CMD_DISCONNECT = 0x0A, //same as WATCHMODE - obsolate 
  //RS800CX_GET_SESSIONS = 0x0b, // does not work
  RS800CX_CMD_GETSESSION = 0x6a,
  RS800CX_DELETE_SESSION = 0x0c,
  RS800CX_CMD_GETSETTINGS = 0x5a, 
	//  commands sniffed when using ppp software 
	//   on clicking edit settings 
	
	// 5a 00 b1 01
	// 5a 00 d5 01
	// 5a 00 bf 01
	// 5a 00 85 02
	// 5a 01 4d d9
	// 5a 00 c3 01
	// 5a 00 ea 01
	// 5a 00 85 02
	// 5a 00 58 04
	// 5a 00 de 02
	// 5a 00 bc 02
	// 5a 00 6a 04
	// 5a 00 5a 01
	// 5a 00 e0 03
	// 5a 00 42 04 <- this after 6a 00 ... last
	// 5a 00 85 02
	// 5a 00 a0 04
	// 5a 00 5b 0a
	// 5a 00 ab 04
	// 5a 00 5d 0d 
	// 5a 00 4f 0e
	// 5a 00 b1 01
	// 5a 00 d2 3e

//  RS800CX_SET_REMINDER = 0x0d,
//  RS800CX_GET_RIMINDER = 0x0e,
  RS800CX_CMD_CONTINUE_TRANSMISSION = 0x16
//  RS800CX_SET_WATCH = 0x01,
//  RS800CX_GET_WATCH = 0x02,
//  RS800CX_SET_EXERCISE = 0x03,
//  RS800CX_GET_EXERCISE = 0x04,
//  RS800CX_SET_LOGO = 0x0F,
//  RS800CX_GET_LOGO = 0x10,
//  RS800CX_SET_BIKE = 0x13,
//  RS800CX_GET_BIKE = 0x14
};
#endif


/* 
  commands sniffed when using ppp software 
  on clicking just download sessions
 
 15			 -> overview
 5a 00 5a 01 -> ?
 6a 00 00 -> first session
 6a 01 00 -> second session
 6a 02 00 -> third session
 
 5a 00 42 04 -> ?
 0a			 -> disconnect
 
 */