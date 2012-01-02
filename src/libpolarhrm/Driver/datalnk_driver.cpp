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

// lsusb output 
// 0da4:0004 Polar Electro OY 
// lets try to find the device using c++ code


void DataLnk_driver::init(void){

	printf("hello from init\n");

	int retval;
	char buf[1024];
	int buflen = 1024;


	retval = find_device();

	if (retval == DEVICE_DISCOVERED) {

		//open it

		printf("Protocol %X\n",dev->descriptor.bDeviceProtocol);
		printf("vendor id %X\n",dev->descriptor.idVendor );
		printf("configurations %d\n",dev->descriptor.bNumConfigurations );

		dev_handle = usb_open(dev);

		// http://www.linuxforums.org/forum/linux-tutorials-howtos-reference-material/10865-developing-usb-device-drivers-userspace-using-libusb.html
		// http://libusb.6.n5.nabble.com/Fwd-usb-get-string-simple-always-returns-1-td3316956.html
		printf("! display the usb device string\n"
		       "! this seems to require root!!\n");

		retval=usb_get_string_simple(dev_handle, 1, buf, buflen);
		if (retval > 0)
			printf("str -->%s <--\n", buf);
		retval=usb_get_string_simple(dev_handle, 2, buf, buflen);
		if (retval > 0)
			printf("str -->%s <--\n", buf);
	}


}

int DataLnk_driver::find_device(void){

	struct usb_bus *busses;

	// init the libiary according doc
	usb_init();
	usb_find_busses();
	usb_find_devices();

	busses = usb_get_busses();


	struct usb_bus *bus;
	int c, i, a;

	for (bus = busses; bus; bus = bus->next) {

		for (dev = bus->devices; dev; dev = dev->next) {
			/* Check if this device is a printer */
			//if (dev->descriptor.bDeviceClass == 7) {
				/* Open the device, claim the interface and do your processing */
			//}
			if (dev->descriptor.idVendor == VENDOR_ID) {
				printf("found DataLnk usb device!\n");
				return DEVICE_DISCOVERED;
			}

			//FIXME I think we could end up here!
			// only 1 configuration
			/* 
			// Loop through all of the configurations 
			for (c = 0; c < dev->descriptor.bNumConfigurations; c++) {
				// Loop through all of the interfaces 
				for (i = 0; i < dev->config[c].bNumInterfaces; i++) {
					// Loop through all of the alternate settings
					for (a = 0; a < dev->config[c].interface[i].num_altsetting; a++) {

						// Check if this interface is a printer 
						if (dev->config[c].interface[i].altsetting[a].bInterfaceClass == 7) {
							// Open the device, set the alternate setting, claim the interface and do your processing
							
						}
					}
				}
			} */
		}
	}

	return DEVICE_NOT_DISCOVERED;

}






void DataLnk_driver::close(void){

	printf("hello from close\n");

	if (dev_handle != NULL) {
		usb_close(dev_handle);

	}
}



/*

int usb_bulk_write(usb_dev_handle *dev, int ep, const char *bytes, int size, int timeout);
int usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout);
int usb_interrupt_write(usb_dev_handle *dev, int ep, const char *bytes, int size, int timeout);
int usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout);

*/

int DataLnk_driver::sendbytes(unsigned char query[], int size){

	printf("hello from sendbytes\n");

	return 0;
}



int DataLnk_driver::recvbytes(unsigned char buf[]){

	printf("hello from recvbytes\n");

	return 0;
} 
