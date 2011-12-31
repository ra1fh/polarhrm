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

#include "datalnk_driver.h"


#include <usb.h>
#include <cstdio>



void DataLnk_driver::init(void){

	printf("hello from init\n");

	
//	usb_init();
/*
	usb_bus *busses;

	usb_init();
	usb_find_busses();
	usb_find_devices();

	busses = usb_get_busses();
*/

}






void DataLnk_driver::close(void){

	printf("hello from close\n");

}



int DataLnk_driver::sendbytes(unsigned char query[], int size){

	printf("hello from sendbytes\n");

	return 0;
}



int DataLnk_driver::recvbytes(unsigned char buf[]){

	printf("hello from recvbytes\n");

	return 0;
} 
