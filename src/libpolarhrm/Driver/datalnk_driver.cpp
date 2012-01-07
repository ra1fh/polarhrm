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
#include <cstring>
#include <assert.h>

// lsusb output 
// 0da4:0004 Polar Electro OY 


//
// init the device. after calling the fuction the red light 
// starts flashing
//
void DataLnk_driver::init(void){

	int ret;
	char buf[1024];
	int buflen = 1024;


	this->dev = find_device();

	if (this->dev != NULL) {

		dev_handle = usb_open(this->dev);


/*
		// http://www.linuxforums.org/forum/linux-tutorials-howtos-reference-material/10865-developing-usb-device-drivers-userspace-using-libusb.html
		// http://libusb.6.n5.nabble.com/Fwd-usb-get-string-simple-always-returns-1-td3316956.html
		printf("! display the usb device string\n"
		       "! this seems to require root!!\n");
		ret=usb_get_string_simple(dev_handle, 1, buf, buflen);
		if (ret > 0)
			printf("str -->%s <--\n", buf);
		ret=usb_get_string_simple(dev_handle, 2, buf, buflen);
		if (ret > 0)
			printf("str -->%s <--\n", buf);
*/

//		signal(SIGTERM, release_usb_device);

		ret = usb_get_driver_np(dev_handle, 0, buf, sizeof(buf));
		printf("usb_get_driver_np returned %d\n", ret);
		if (ret == 0) {
			printf("interface 0 already claimed by driver \"%s\", attempting to detach it\n", buf);
			ret = usb_detach_kernel_driver_np(dev_handle, 0);
			printf("usb_detach_kernel_driver_np returned %d\n", ret);
		}

/*
		ret = usb_claim_interface(dev_handle, 0);
		if (ret != 0) {
			printf("claim failed with error %d\n", ret);
			exit(1);
		}

		ret = usb_set_altinterface(dev_handle, 0);
//		assert(ret >= 0);


	ret = usb_get_descriptor(dev_handle, 0x0000001, 0x0000000, buf, 0x0000012);
	printf("1 get descriptor returned %d, bytes: ", ret);
	print_bytes(buf, ret);

//usleep(4*1000);

	ret = usb_get_descriptor(dev_handle, 0x0000002, 0x0000000, buf, 0x0000009);
	printf("2 get descriptor returned %d, bytes: ", ret);
	print_bytes(buf, ret);

//usleep(3*1000);

	ret = usb_get_descriptor(dev_handle, 0x0000002, 0x0000000, buf, 0x0000029);
	printf("3 get descriptor returned %d, bytes: ", ret);
	print_bytes(buf, ret);

//usleep(3*1000);

	ret = usb_release_interface(dev_handle, 0);
	if (ret != 0) 
		printf("failed to release interface before set_configuration: %d\n", ret);
*/
	ret = usb_set_configuration(dev_handle, 0x0000001);
	printf("4 set configuration returned %d\n", ret);

	ret = usb_claim_interface(dev_handle, 0);
	if (ret != 0) 
		printf("claim after set_configuration failed with error %d\n", ret);

/*
	ret = usb_set_altinterface(dev_handle, 0);
	printf("4 set alternate setting returned %d\n", ret);
//	usleep(41*1000);
*/

	ret = usb_control_msg(dev_handle, USB_TYPE_CLASS + USB_RECIP_INTERFACE, 0x000000a, 0x0000000, 0x0000000, buf, 0x0000000, 1000);
	printf("5 control msg returned %d", ret);

//  usleep(6*1000);
/*
	ret = usb_get_descriptor(dev_handle, 0x0000022, 0x0000000, buf, 0x0000076);
	printf("6 get descriptor returned %d, bytes: \n", ret);
	print_bytes(buf, ret);

*/

/*
//	usleep(5*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("7 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);


	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("8 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);
*/

//usleep(401*1000);
	memcpy(buf, "\x01\x07\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 0x0000100);
	ret = usb_interrupt_write(dev_handle, 0x00000003, buf, 0x0000100, 1000);
	printf("9 interrupt write returned %d, bytes: \n", ret);
	print_bytes(buf, ret);

//	usleep(14*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("10 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);


/*
//	usleep(2*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("11 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);

//	usleep(4*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("12 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);

//	usleep(1*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("13 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);

//	usleep(3*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("14 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);
*/


//	usleep(2*1000);
	memcpy(buf, "\x01\x40\x01\x00\x51\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 0x0000100);
	ret = usb_interrupt_write(dev_handle, 0x00000003, buf, 0x0000100, 1000);
	printf("15 interrupt write returned %d, bytes: \n", ret);
	print_bytes(buf, ret);




//	usleep(5*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("16 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);


/*
  //FIXME from this point the deamon of the windows software is 
	// polling for data
//	usleep(2*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("17 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);



//	usleep(2*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("18 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);

//	usleep(1*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("19 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);



//	usleep(19*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("20 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);

//	usleep(2500*1000);
	ret = usb_interrupt_read(dev_handle, 0x00000081, buf, 0x0000200, 1000);
	printf("21 interrupt read returned %d, bytes: \n", ret);
	print_bytes(buf, ret);

*/

/*

	assert(ret == 0);
	ret = usb_close(dev_handle);
	assert(ret == 0);
*/
}

}





//
// find a given device 
//
//
struct usb_device * DataLnk_driver::find_device(void){

	struct usb_bus *busses;

	// init the libiary according doc
	usb_init();
	usb_set_debug(255);
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
				printf("dev->descriptor.bLength %X\n", dev->descriptor.bLength);
				printf("dev->descriptor.bDescriptorType %X\n", dev->descriptor.bDescriptorType);
				printf("dev->descriptor.bcdUSB %X\n", dev->descriptor.bcdUSB);
				printf("dev->descriptor.bDeviceClass %X\n", dev->descriptor.bDeviceClass);
				printf("dev->descriptor.bDeviceSubClass %X\n", dev->descriptor.bDeviceSubClass);
				printf("dev->descriptor.bDeviceProtocol %X\n", dev->descriptor.bDeviceProtocol);
				printf("dev->descriptor.bMaxPacketSize0 %X\n", dev->descriptor.bMaxPacketSize0);
				printf("dev->descriptor.idVendor %X\n", dev->descriptor.idVendor);
				printf("dev->descriptor.idProduct %X\n", dev->descriptor.idProduct);
				printf("dev->descriptor.bcdDevice %X\n", dev->descriptor.bcdDevice);
				printf("dev->descriptor.iManufacturer %X\n", dev->descriptor.iManufacturer);
				printf("dev->descriptor.iProduct %X\n", dev->descriptor.iProduct);
				printf("dev->descriptor.iSerialNumber %X\n", dev->descriptor.iSerialNumber);
				printf("dev->descriptor.bNumConfigurations %X\n", dev->descriptor.bNumConfigurations);

				//FIXME I think we could end up here!
				// only 1 configuration
				 
				// Loop through all of the configurations 
				for (c = 0; c < dev->descriptor.bNumConfigurations; c++) {

					printf("dev->config[%d].bLength %X\n",c,dev->config[c].bLength);
					printf("dev->config[%d].bDescriptorType %X\n",c,dev->config[c].bDescriptorType);
					printf("dev->config[%d].wTotalLength %X\n",c,dev->config[c].wTotalLength);
					printf("dev->config[%d].bNumInterfaces %X\n",c,dev->config[c].bNumInterfaces);
					printf("dev->config[%d].bConfigurationValue %X\n",c,dev->config[c].bConfigurationValue);
					printf("dev->config[%d].iConfiguration %X\n",c,dev->config[c].iConfiguration);
					printf("dev->config[%d].bmAttributes %X\n",c,dev->config[c].bmAttributes);
					printf("dev->config[%d].MaxPower %X\n",c,dev->config[c].MaxPower);

					// Loop through all of the interfaces 
					for (i = 0; i < dev->config[c].bNumInterfaces; i++) {

						// Loop through all of the alternate settings
						for (a = 0; a < dev->config[c].interface[i].num_altsetting; a++) {

							printf("altsetting[%d].bLength %X\n",a,dev->config[c].interface[i].altsetting[a].bLength);
							printf("altsetting[%d].bDescriptorType %X\n",a,dev->config[c].interface[i].altsetting[a].bDescriptorType);
							printf("altsetting[%d].bInterfaceNumber %X\n",a,dev->config[c].interface[i].altsetting[a].bInterfaceNumber);
							printf("altsetting[%d].bAlternateSetting %X\n",a,dev->config[c].interface[i].altsetting[a].bAlternateSetting);
							printf("altsetting[%d].bNumEndpoints %X\n",a,dev->config[c].interface[i].altsetting[a].bNumEndpoints);
							printf("altsetting[%d].bInterfaceClass %X\n",a,dev->config[c].interface[i].altsetting[a].bInterfaceClass);
							printf("altsetting[%d].bInterfaceSubClass %X\n",a,dev->config[c].interface[i].altsetting[a].bInterfaceSubClass);
							printf("altsetting[%d].bInterfaceProtocol %X\n",a,dev->config[c].interface[i].altsetting[a].bInterfaceProtocol);
							printf("altsetting[%d].iInterface %X\n",a,dev->config[c].interface[i].altsetting[a].iInterface);

							// Check if this interface is a printer 
							if (dev->config[c].interface[i].altsetting[a].bInterfaceClass == 7) {
								// Open the device, set the alternate setting, claim the interface and do your processing
							}

							for (int e=0; e <= dev->config[c].interface[i].altsetting[a].bNumEndpoints; e++) {

								printf("endpoint[%d].bLength %X\n",e, dev->config[c].interface[i].altsetting[a].endpoint[e].bLength);
								printf("endpoint[%d].bDescriptorType %X\n",e, dev->config[c].interface[i].altsetting[a].endpoint[e].bDescriptorType);
								printf("endpoint[%d].bEndpointAddress %X\n",e, dev->config[c].interface[i].altsetting[a].endpoint[e].bEndpointAddress);
								printf("endpoint[%d].bmAttributes %X\n",e, dev->config[c].interface[i].altsetting[a].endpoint[e].bmAttributes);
								printf("endpoint[%d].wMaxPacketSize %X\n",e, dev->config[c].interface[i].altsetting[a].endpoint[e].wMaxPacketSize);
								printf("endpoint[%d].bInterval %X\n",e, dev->config[c].interface[i].altsetting[a].endpoint[e].bInterval);
								printf("endpoint[%d].bRefresh %X\n",e, dev->config[c].interface[i].altsetting[a].endpoint[e].bRefresh);
								printf("endpoint[%d].bSynchAddress %X\n",e, dev->config[c].interface[i].altsetting[a].endpoint[e].bSynchAddress);
							} //end for
						} //end for
					} //end for
				} //end for

			return dev;

			}//end if
		}//end for
	}//end for

	return NULL;
}






void DataLnk_driver::close(void){

	int ret;
	printf("hello from close\n");

	if (dev_handle != NULL) {
		ret = usb_release_interface(dev_handle, 0);
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


// for testing 
int DataLnk_driver::sendbytes(void){

//	const char bytes[] = {0x04,0x40,0x20,0x00,0x10,0x1e,0x34,0x4d,0x10,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x22,0xb8,0xa9,0x00,0xe7,0x02,0x04,0x01,0x09,0x08,0x06,0x06,0x4d,0x34,0x1e,0x2b,0x0b,0x1e,0x0f,0x27,0x15};

	const char bytes[] = {0x01, 0x07};
//	const char bytes[] = {0x01, 0x40, 0x01, 0x00, 0x51};
//	const char bytes[] = {0x01, 0x40, 0x01, 0x00, 0x55};
	
//	const char bytes[] = {0x80, 0x06, 0x00, 0x01, 0x00, 0x00, 0x12, 0x00};
//	const char bytes[] = {0x09, 0x02, 0x29, 0x00, 0x01, 0x01, 0x02, 0xa0, 0x32};

	if (dev_handle != NULL) {
		//for ( char x = 0; x <= 0xFF; x++)
		//usb_bulk_write(dev_handle, 0x00000003, bytes, 0x100, 3000);

	}
	return 0;
}


int DataLnk_driver::recvbytes(unsigned char buf[]){

	printf("hello from recvbytes\n");

	return 0;
} 





// show the content of a buffer in usbsnoop 
// output format (makes it easy to compare)
void DataLnk_driver::print_bytes(char *buf, int len){
	int i;
	printf ("%08d:",0);
	for(i=0; i<len; i++) {
		printf(" %.2x",(unsigned char)buf[i] );
		if((i+1)%16 == 0)
			printf ("\n%08d:",((i+1)/16)*10);
	}
	printf ("\n");
}
