// RS800CX_comm.cpp
//
// Copyright (C) 2011 - Thomas Foyth
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <list>
#include "../Datanode.h"

#include "../Driver/driver.h"
#include "../Watch/Overview.h"

#include "RS800CX_comm.h"
#include "RS800CX_commands.h"

#include "../../cli/polarhrm_config.h"
#include "../util_functions.h"


RS800CXcomm::RS800CXcomm(Driver * d){
	this->driver = d;
}



void RS800CXcomm::setDriver(Driver * d){
	this->driver = d;
}


void RS800CXcomm::initDriver(void){
	this->driver->init();
}


void RS800CXcomm::closeDriver(void){
	this->driver->close();
}



void RS800CXcomm::getUser(unsigned char & raw_user_data, int & len) {

}




// get the watch overview with some basic data
void RS800CXcomm::getOverview(unsigned char *raw_buffer, int &len) {

	unsigned char query[] = { RS800CX_GET_OVERVIEW };
	this->driver->sendbytes(query, sizeof(query));

	// return the length
	len =  driver->recvbytes(raw_buffer);
}




/* get the sessiondata out of the watch and store them in a node list. 
 the idea is to have a clear separation and any datamaipulation 
 data are manipulated at the with parse functions */
std::list<Datanode> RS800CXcomm::getSession(int sess_no) {

	sess_no -= 1; //fix session number offset

	std::list<Datanode> l;

	int checksum = 0;
	int sesschecksum;

	int len,i;
	int nodecount = 0; // counts up
	int nodes_left; // counts down - start value set at first packet
	unsigned char buf[BUF_SIZE];


	//unsigned char query[]  = { 0x6a, sess_no, 0x00, 0x32}; //0x6a, 0x01, 0x00 start counting at zero 0x00 (first session)
	unsigned char iquery[] = { RS800CX_CMD_GETSESSION, sess_no, 0x00, 0x00};
	unsigned char cquery[] = { RS800CX_CMD_CONTINUE_TRANSMISSION, sess_no };


	// initial query 
	driver->sendbytes(iquery, sizeof(iquery));
	len=driver->recvbytes(buf);

	sesschecksum = (buf[7]<<8)+buf[8];

	printf("command:");
	for (int z=0; z < sizeof(iquery); z++)
		printf("%x ",iquery[z]);
	printf("\n %i node len:%i\n",nodecount, len);
	for (int k=0; k<=len; k++) {
		printf("%.2X ", buf[k]);
	}
	printf("\n");
	printf("There are %i packets left to transfer (not confirmed)\n",buf[4]);


	// need to subtract the protocol specific bytes
	// but ONLY for checksum.
	//will be managed in parse function
	checksum += len;
	checksum -= RS800CX_FIRST_PACKET_CORRECTION_HEAD;
	checksum -= RS800CX_FIRST_PACKET_CORRECTION_TAIL;

	//put the received data into the node list
	Datanode node(buf, len);
	l.push_back(node);
	nodes_left = buf[4];

	// after sending the init query go ahead a long as all bytes get received
	//while (checksum < w_o->used_bytes))
	while (nodes_left > 0) {

		// send continue command 
		driver->sendbytes(cquery, sizeof(cquery));
		len=driver->recvbytes(buf);

		printf("command:");
		for (int z=0; z < sizeof(cquery); z++)
			printf("%x ",cquery[z]);

		printf("\n%i node len:%i\n",nodecount+1, len);
		for (int k=0; k<=len; k++) {
			printf("%.2X ", buf[k]);
		}
		printf("\n");
		printf("There are %i packets left to transfer (not confirmed)\n",buf[4]);

			// need to subtract the protocol specific bytes
			// but ONLY for checksum.
			//will be managed in parse function
		checksum += len;
		checksum -= RS800CX_ALL_OTHER_PACKETS_CORRECTION_HEAD;
		checksum -= RS800CX_ALL_OTHER_PACKETS_CORRECTION_TAIL;

		//put the received data into a node list
		node = Datanode(buf, len);
		l.push_back(node);
		nodecount++;
		nodes_left = buf[4];


		#if defined(DEBUGPRINT)
			printf("get sessiondata bytes: %d / %d\n\n\n",checksum, sesschecksum);
		#endif
	} // end while

return l;
}



int RS800CXcomm::getReminder(unsigned char data[], unsigned char rem_num){
/*
	unsigned char query[1]; 

	query[0]=RS800CX_GET_RIMINDER;
	query[1]=rem_num;
	
	this->driver->sendbytes(query, sizeof(query));
	
return driver->recvbytes(data);
*/
}



int RS800CXcomm::setReminder(unsigned char data[], unsigned char len){

/* TODO implement later ...	
	unsigned char query[] = { RS800CX_SET_RIMINDER,data[],0x00 };
	sendbytes(query, sizeof(query));
*/	
return 0;
}

/*
   disconnect the watch
*/
void RS800CXcomm::disconnect(void) {
	unsigned char query[] = { RS800CX_CMD_WATCHMODE };
	this->driver->sendbytes(query, sizeof(query));
}


/*
   delete a session file from the watch
*/
void RS800CXcomm::deleteFile(unsigned char bcdyear,
                           unsigned char month,
                           unsigned char bcdday,
                           unsigned char bcdhour,
                           unsigned char bcdminute,
                           unsigned char bcdsecond){

/*
delete session 1 (File 20110913_20:56:38.0.dump)
0000   00 0c fe 38 56 20 13 11 09                       ...8V ...

delete session 2 (File 20110913_20:56:28.0.dump)
0000   00 0c fe 28 56 20 13 11 09                       ...(V ...

*/

	unsigned char query[7];
	//unsigned char confirm_query[] = { 0x00 };
	//unsigned char confirm_query[] = { 0x6 };

	unsigned char confirm_query[] = { 0x18, 0x00 };
	//unsigned char confirm_query[] = { 0x0, 0x18, 0x0 };

	query[0] = RS800CX_DELETE_SESSION;
	query[1] = 0xfe; // I triggert this value at ppp software for specific session deletion



/*FIXME don t know how the delete works in detail */
//	query[1] = 0xff; // delete all files (don t know the rest of the parameters has to be)

	query[2] = bcdsecond; // second
	query[3] = bcdminute; // minute
	query[4] = bcdhour; // hour
	query[5] = bcdday; // day
	query[6] = bcdyear; // year last 2 digits
	query[7] = month; // month

	#ifdef DEBUGPRINT
	std::cout<< "delete command: ";
	for (int i=0; i<=7; i++) {
		printf("%02x ", query[i]);
	} 
	std::cout<<"\n";
	#endif
	this->driver->sendbytes(confirm_query, sizeof(confirm_query));


	unsigned char buf[BUF_SIZE]; 
	int len;
	len = this->driver->recvbytes(buf);

		for (int i=0; i<=len; i++) {

			std::cout << hex <<(int) buf[i] << " ";
		}

	 this->driver->sendbytes(query, sizeof(query));

}


/*
 delete all sessionfiles from the watch
*/
void RS800CXcomm::deleteAllFiles(void){

	unsigned char query[7];

	query[0] = 0x0c;

	query[1] = 0xff; // delete all files
	query[2] = bcd(00); // second
	query[3] = bcd(00); // minute
	query[4] = bcd(00); // hour
	query[5] = bcd(02); // day
	query[6] = bcd(00); // year last 2 digits
	query[7] = bcd(00); // month

	#ifdef DEBUGPRINT
	std::cout<< "delete command: ";
	for (int i=0; i<=7; i++) {
		std::cout<< hex <<(int) query[i]<<" ";
	} 
	std::cout<<"\n";
	#endif

	this->driver->sendbytes(query, sizeof(query));
}



/*
 test some thing
 * 5A 0 5A 1 0 0 21 0 
 * 6A 0 0 32 

*/
void RS800CXcomm::testsomething(void){

	unsigned char *raw_buffer;
	int len;

	raw_buffer = new unsigned char[BUF_SIZE];

	//unsigned char query[] = { 0x5A, 0x00, 0x5A, 0x01, 0x0, 0x0, 0x21, 0x0 };
	
	//unsigned char query[] = { 0x5A, 0x00, 0x42, 0x04, 0x00, 0x00, 0x57, 0x0}; 
	//unsigned char query[] = {0x6A, 0x00, 0x00, 0x32 };
	unsigned char query[] = {0x6A, 0x0, 0x0, 0x0};

	this->driver->sendbytes(query, sizeof(query));

	// return the length
	len =  driver->recvbytes(raw_buffer);

	for (int i=0; i<len; i++){
		printf("0x%X, ",raw_buffer[i]);
	}
	printf("\n");
}