/*
 * RS800CX.h
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


#ifndef _RS800CX_H
#define _RS800CX_H


#include <iostream>
#include "Watch/Watch.h"
//#include "WatchData.h"
#include "RS800CX_comm.h"
#include "RS800CX_parse.h"
#include "Driver/driver.h"
#include "polarhrm_config.h"




/*FIXME need this yes or no */
//class Watch;
class RS800CXcomm;
class Driver;


class RS800CX:public Watch {

  private:
	static const int monitor=DEVICE_RS800CX;
	static const int version=107;	// with an existing hrm file

	//static const int packet_size = RS800CX_MAX_PACKET_SIZE;
	//int memory_size;

/* TODO */
//WatchData *watchdata;

	RS800CXcomm *watchcomm;
	RS800CXparse *parser;
	Driver *driver;


  public:
	RS800CX(void);
	void saveHRM(void);
	void eraseSessions(void);
	void openRaw(std::string);
};
#endif
