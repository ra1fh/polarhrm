/*
 * RCX5.h
 *
 * Copyright (C) 2012 - Thomas Foyth
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


#ifndef _RCX5_H
#define _RCX5_H


#include <iostream>
#include "../Watch/Watch.h"
//#include "WatchData.h"
#include "RCX5_comm.h"
#include "RCX5_parse.h"
#include "../Driver/driver.h"
#include "../supported_devices.h"




/*FIXME need this yes or no */
//class Watch;
class RCX5comm;
class Driver;


class RCX5:public Watch {

  private:
	static const int monitor=DEVICE_RCX5;
	static const int version=107;	// with an existing hrm file

	//static const int packet_size = RCX5_MAX_PACKET_SIZE;
	//int memory_size;

/* TODO */
//WatchData *watchdata;

	RCX5comm *watchcomm;
	RCX5parse *parser;
	Driver *driver;


  public:
	RCX5(void);
	void saveHRM(void);
	void eraseSessions(void);
	void openRaw(std::string);
};
#endif
