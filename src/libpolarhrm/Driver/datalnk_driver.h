/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * polarhrm
 * Copyright (C) thomas foyth 2011 <thomas.foyth@gmail.com>
 * 
polarhrm is free software: you can redistribute it and/or modify it
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

#ifndef _DATALNK_DRIVER_H_
#define _DATALNK_DRIVER_H_

#define VENDOR_ID 0x0da4


#include <usb.h>

#include "driver.h"


class DataLnk_driver:public Driver{
public:
	void init(void);
	int sendbytes(unsigned char query[], int size);
	int sendbytes(void);
	int recvbytes(unsigned char buf[]);
	void close(void);

protected:

private:

	usb_dev_handle *dev_handle;
	struct usb_device *dev;


	//find the usb device connected 
	struct usb_device *find_device(void);

	void print_bytes(char *buf, int len);

};

#endif // _DATALNK_DRIVER_H_
