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



#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/irda.h>

#include <unistd.h>
#include <string.h>


#include "driver.h"
#include "irda_driver.h"

#include "../../cli/polarhrm_config.h"



// init connection
//
void Irda_driver::init(void) {

   int daddr = 0;
   struct sockaddr_irda peer;

        // Create socket 
        wfd = socket(AF_IRDA, SOCK_STREAM, 0);
        if (wfd < 0) {
                perror("socket");
                exit(-1);
        }

	    // Find a peer 
        daddr = irscanf_discover_devices();
        if (daddr == -1)
               exit (-1);


		// seems that I need to go into detail at TinyTP
		//
		// http://www.alanjmcf.me.uk/comms/infrared/IrDA%20faq.html#_Toc128227619
		//
		// For the technically interested, this mode appears to use a TinyTP connection with Service Name “HRM”, 
		// with the connection initiated by the PPP software on the PC. 

        peer.sir_family = AF_IRDA;
        strncpy(peer.sir_name, "HRM", 25); // Usually <service>:IrDA:TinyTP    IrDA:TinyTP:LsapSel
        peer.sir_addr = daddr;
        
        if (connect(wfd, (struct sockaddr*) &peer, sizeof(struct sockaddr_irda))) {
                perror("connect");
                exit (-1);
        }	

}




// Send raw bytes to the device. Return the number of bytes written 
// comes form rs 400 tools
int Irda_driver::sendbytes(unsigned char query[], int size) {

    int sent = write(wfd, query, size);
    return sent;
}




// Receive raw bytes from the device. Returns the number of bytes read
// Note that it's the caller's duty to clear the receive buffer
// comes form rs 400 tools
int Irda_driver::recvbytes(unsigned char buf[]) {
	
	return read(wfd, buf, BUF_SIZE);
}	



// close the connection
void Irda_driver::close(void) {

	// it is getting strange - my programming -
	// every mistake I make did allready appear
	//http://stackoverflow.com/questions/5582127/close-filedescriptor-using-qt-in-linux
	::close(wfd);

}



//  Function echo_discover_devices (fd)
// 
//     Try to discover some remote device(s) that we can connect to
// 
int Irda_driver::irscanf_discover_devices(void) {

	irda_device_info *info = new irda_device_info;
	irda_device_list *list = new irda_device_list;

	unsigned char buf[MY_BUF_SIZE];

	//unsigned char *buf;
	socklen_t len;

/*
	typedef struct IRDA_DEVICE_INFO {
		u_char irdaDeviceID[4];
		char irdaDeviceName[22];
		u_char Reserved[2];
	} _IRDA_DEVICE_INFO;
*/



	/* calculate the memory size for buffer */
	//len = (socklen_t*) ((sizeof(struct irda_device_list) + sizeof(struct irda_device_info)) * MAX_DEVICES);

	//len = (socklen_t*) sizeof(irda_device_info);

	/* allocates meomory and returns a pointer */
	//buf = (unsigned char*) malloc(len);

	/* typecast */
	list = (struct irda_device_list*) buf;
	len = MY_BUF_SIZE;
	
	/* getsockopt, setsockopt - get and set options on sockets 
	SOL_IRLMP set of options
	http://msdn.microsoft.com/en-us/library/aa921099.aspx

	IRLMP_ENUMDEVICES     Returns a list of IrDA device IDs for IR capable devices within range.   
	http://msdn.microsoft.com/en-us/library/ms691760(v=vs.85).aspx   */
	if (getsockopt(wfd, SOL_IRLMP, IRLMP_ENUMDEVICES, buf, &len)) {
		perror("getsockopt");
		exit(-1);
	}

	if (len > 0) {

		#if defined(DEBUGPRINT)
			printf("Discovered: (list len=%d)\n", list->len);
		#endif

		for (int i=0;i<list->len;i++) {

			#if defined(DEBUGPRINT)
				printf("name:  %s\n", list->dev[i].info);
				printf("daddr: %08x\n", list->dev[i].daddr);
				printf("saddr: %08x\n", list->dev[i].saddr);
				printf("hint0: %08x\n",list->dev[i].hints[0]);
				printf("hint1: %08x\n",list->dev[i].hints[1]);
				printf("\n");
			#endif

			/* do we have a supported watch ?
			here we can do a switch case to automaticly choose some function 
			but for now we return the adress */
			if (list->dev[i].hints[0] & HINT_POLAR) {
				#if defined(DEBUGPRINT)
					printf("YESS POLAR Watch\n");
				#endif
				return list->dev[i].daddr;
			}

		}
	}

	#if defined(DEBUGPRINT)
		printf("Didn't find any devices!\n");
	#endif
return -1;
}