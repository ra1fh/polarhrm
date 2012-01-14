/*
 * supported_devices.h
 *
 * Copyright (C) Thomas Foyth 2011 <thomas.foyth@gmail.com>
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

#ifndef _SUPPORTED_DEVICES_H_
#define _SUPPORTED_DEVICES_H_

#include <iostream>

/* witch devices are supported by now? */
enum dev {
	DEVICE_S625X=22, //use monitor number
	DEVICE_S725X, //www says it is exactly the same as S625X just has cycling gear packed instead of s1 footpod
	DEVICE_RS800CX=38,
	DEVICE_RCX5=105, //lets set a random number
	DEVICE_NOT_SUPPORTED = 99
};


typedef struct suppDevices{
	dev id;
	char name[20];// device name
	std::string description; 
} suppDevices;



static suppDevices supportedDevices[] = 
{
	{ DEVICE_S625X, "S625X", "hrm writting, run speed, altituede is working"},
	{ DEVICE_S725X, "S725X", "same as S625X"},
	{ DEVICE_RS800CX, "RS800CX", "current developing"},
	{ DEVICE_RCX5, "RCX5", "current driver developing"}
};



#endif // _SUPPORTED_DEVICES_H_
