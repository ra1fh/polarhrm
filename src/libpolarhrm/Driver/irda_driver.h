/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * polarhrm
 * Copyright (C) Thomas Foyth 2011 <thomas.foyth@gmail.com>
 * 
 * polarhrm is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * polarhrm is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _IRDA_DRIVER_H_
#define _IRDA_DRIVER_H_

#include "driver.h"

#ifndef AF_IRDA
#define AF_IRDA 23
#endif /* AF_IRDA */

/* let's' assume that the user just puts one device in fornt of the interface */
#define MAX_DEVICES 1

/*FIXME identify polar devices, needs to be checked with more devices */
#define HINT_POLAR 0x82

// using a constant buffer size
#define MY_BUF_SIZE 4096


class Irda_driver:public Driver 
{
public:
	void init(void);
	int sendbytes(unsigned char query[], int size);
	int recvbytes(unsigned char buf[]);
	void close(void);

private:
	/* watch file descriptor */
	int wfd;

	int irscanf_discover_devices(void);
};

#endif // _IRDA_DRIVER_H_
