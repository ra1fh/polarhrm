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

//0da4:0004 Polar Electro OY 
#define VENDOR_ID 0x0da4


void DataLnk_driver::init(void){

	printf("hello from init\n");

	

	struct usb_bus *busses;

	usb_init();
	usb_find_busses();
	usb_find_devices();

	busses = usb_get_busses();


	struct usb_bus *bus;
	int c, i, a;



	for (bus = busses; bus; bus = bus->next) {
		struct usb_device *dev;

		for (dev = bus->devices; dev; dev = dev->next) {
			/* Check if this device is a printer */
			if (dev->descriptor.bDeviceClass == 7) {
				/* Open the device, claim the interface and do your processing */
				
			}
			if (dev->descriptor.idVendor == VENDOR_ID) {
				printf("found DataLnk usb device!\n");
				printf("Protocol %X\n",dev->descriptor.bDeviceProtocol);
				printf("vendor id %X\n",dev->descriptor.idVendor );
			}

			/* Loop through all of the configurations */
			for (c = 0; c < dev->descriptor.bNumConfigurations; c++) {
				/* Loop through all of the interfaces */
				for (i = 0; i < dev->config[c].bNumInterfaces; i++) {
					/* Loop through all of the alternate settings */
					for (a = 0; a < dev->config[c].interface[i].num_altsetting; a++) {

						/* Check if this interface is a printer */
						if (dev->config[c].interface[i].altsetting[a].bInterfaceClass == 7) {
							/* Open the device, set the alternate setting, claim the interface and do your processing */
							
						}
					}
				}
			}
		}
	}


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
