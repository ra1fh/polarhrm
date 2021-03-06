// RCX5_comm.cpp
//
// Copyright (C) 2012 - Thomas Foyth
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
#include <iostream>
#include <list>
#include "../Datanode.h"


#include "../Driver/datalnk_driver.h"
#include "../Watch/Overview.h"

#include "RCX5_comm.h"
//#include "RCX5_commands.h"


#include "../util_functions.h"


RCX5comm::RCX5comm(Driver * d){
	this->driver = d;
}



void RCX5comm::setDriver(Driver * d){
	this->driver = d;
}


void RCX5comm::initDriver(void){
	this->driver->init();
}


void RCX5comm::closeDriver(void){
	this->driver->close();
}



void RCX5comm::getUser(unsigned char & raw_user_data, int & len) {

}





// get the watch overview with some basic data
void RCX5comm::getOverview(unsigned char *raw_buffer, int &len) {

	int session_size=0;
	int counter=0;
	int command_counter=0;
	unsigned char sendquery[DATALNK_SEND_BUFFER_SIZE];

	// first query at an open snyc connection
	// Get overview -> response holds the number of sessions
	//
	unsigned char query[] = {0x01, 0x40, 0x02, 0x00, 0x54, 
							 hardwareID[0], 
							 hardwareID[1], 
							 hardwareID[2], 
							 0x00};


	unsigned char response[] = {0x04, 0x42, 0x3c};

	// response syncronisation faild ?
	// 05 07 00 00 03 80 13 70 04 00 00 e9 d1 02 9e

	write_buffer (sendquery,sizeof(sendquery),0);
	memmove(sendquery, query, sizeof(query));

	do {
		printf("get overview %d\n",command_counter);

		print_bytes ((char*)sendquery,sizeof(sendquery));
		printf("gonig down \n\n");

		//usleep(100*1000);
		this->driver->sendbytes(sendquery, sizeof(sendquery));

		counter=0;
		do {
			// FIXME: changed from 1*1000 -> 100*1000 to avoid getOverview
			// error in case of delayed press on 'yes' to confirm id
			// even pressing 'yes' to fast causes a problem
			// XXX 15.06.2013 enhancing function by outlining the sleep!!
			//usleep(1*1000);

			// return the length
			len = this->driver->recvbytes(raw_buffer);

			print_bytes((char*)raw_buffer,len);

			if( DATALNK_RECV_BUFFER_SIZE == len 
			&& raw_buffer[0] == response[0]
			&& raw_buffer[1] == response[1]
			&& raw_buffer[2] == response[2]){
				// XXX don t send commands for now
				// send success command
				//this->success();
				//this->idle2();
			
				return;
			}

			counter++;
		}while(counter < 20); //XXX 20 times might be reduced

		command_counter++;
	}while(command_counter < 1); //XXX looks that there is no loop required

	printf("did not get overview\n");
	this->disconnect();
	exit(EXIT_FAILURE);
}

// all commands with the same structure
// purpose is not knowen for all commands
// I guess these set of commands is used to sync the watchsettings
// with the syncapp
// 01 40 02 00 54 4d 34 1e 00 get overview
// 01 40 02 00 54 4d 34 1e 10 QueryA
// 01 40 02 00 54 4d 34 1e 1c QueryB
// 01 40 02 00 54 4d 34 1e 95
// 01 40 02 00 54 4d 34 1e b0
// 01 40 02 00 54 4d 34 1e 42
// 01 40 02 00 54 4d 34 1e 93
// 01 40 02 00 54 4d 34 1e 2f
// 01 40 02 00 54 4d 34 1e 33
// 01 40 02 00 54 4d 34 1e 31
// 01 40 02 00 54 4d 34 1e 38
// 01 40 02 00 54 4d 34 1e 3a
// 01 40 02 00 54 4d 34 1e 35
// 01 40 02 00 54 4d 34 1e 36
// 01 40 02 00 54 4d 34 1e 40
// 01 40 02 00 54 4d 34 1e e2
// 01 40 02 00 54 4d 34 1e f7
// 01 40 02 00 54 4d 34 1e 2f (5x serval times)
// 01 40 02 00 54 4d 34 1e 01
// 01 40 02 00 54 4d 34 1e c8
// 01 40 02 00 54 4d 34 1e e7
// 01 40 02 00 54 4d 34 1e ae
// 01 40 02 00 54 4d 34 1e 1c

// 01 40 02 00 54 4d 34 1e c9
// 01 40 02 00 54 4d 34 1e 44

// 04 40 f4 = logo command containing standard logo data
// Contains bits 8 & 9 = 40 1e = 64 x 30 which is logo size
// First two lines of logo are stored at the end of the line
// 00 01 00 88 06 00 00 00
// 80 00 00 00 88 00 80 00

// get session information
//
void RCX5comm::getSessionOverview(unsigned char *raw_buffer, int &len, int sess_no) {

	int session_size=0;
	int counter=0;
	unsigned char sendquery[DATALNK_SEND_BUFFER_SIZE];

	// this is not a general getOverview requerst , this is a get session size
	// request to get the session size of a spezific session
	// with this information the getsession function is calling for the 
	// itemized packets
	unsigned char query[] = {0x01, 0x40, 0x03, 0x00, 0x54,
							 hardwareID[0],
							 hardwareID[1],
							 hardwareID[2],
							 0xb2, 0x00, 
							 sess_no };

	RCX5comm::write_buffer(sendquery, DATALNK_SEND_BUFFER_SIZE, 0);

	//response data: 04 42 06 00 40 b2 00 5d 28 
	unsigned char response[] = {0x04, 0x42, 0x06};

	memmove(sendquery, query, sizeof(query));

	printf("get session information\n");
	this->driver->sendbytes(sendquery, sizeof(sendquery));

	do {
		usleep(1000);
		
		len = this->driver->recvbytes(raw_buffer);

		if( DATALNK_RECV_BUFFER_SIZE == len 
		&& raw_buffer[0] == response[0]
		&& raw_buffer[1] == response[1]
		&& raw_buffer[2] == response[2]){

			// moved to parsing
			//print_bytes ((char*)raw_buffer, len);

			return;
		}

		counter++;
	}while(counter < 10);

	printf("did not get session information\n");
	exit(EXIT_FAILURE);
}




/*
 * get the sessiondata out of the watch and store them in a node list. 
 * the idea is to have a clear separation and any datamaipulation 
 * data are manipulated at the with parse functions 
 *
 * 
 */
std::list<Datanode> RCX5comm::getSession(int sess_no, int sess_len) {

//get session
//little endian coding
//
//0xBBAA (accumulated  packets size) 
//0xYYXX (packet size)
//eg.
//0x01be
//0x037c
//NN session number?
//                              NN AA BB       XX YY
//01 40 09 00 54 4d 34 1e b3 00 01 00 00 00 00 be 01
//01 40 09 00 54 4d 34 1e b3 00 01 be 01 00 00 be 01
//01 40 09 00 54 4d 34 1e b3 00 01 7c 03 00 00 be 01

	std::list<Datanode> l;

	int checksum = 0;
	int sesschecksum;

	int len,i;
	int nodecount = 0; // counts up
	int nodes_left; // counts down - start value set at first packet

	unsigned char buf[RCX5_TRANSFER_BUFFER_SIZE];
	unsigned char sendquery[DATALNK_SEND_BUFFER_SIZE];

	const int packet_size = RCX5_PACKETS_NO_HEADER;

	union Usize{
		int16_t intvalue;
		unsigned char uchar[2];
	};

	Usize accumulate, packet;


	packet.intvalue = packet_size;
	accumulate.intvalue = 0; // start value

	sesschecksum = sess_len;

	if((sess_len % packet_size) != 0) {
		// dont forget the modulo as additonal packet
		nodes_left=(sess_len/packet_size)+1;

		printf("%d packets each %d bytes (load) \n",nodes_left-1, packet_size );
		printf("last packet size %d bytes\n",sess_len%packet_size);

	}
	else {
		nodes_left=(sess_len/packet_size);
		printf("%d packets each %d bytes (load) \n",nodes_left, packet_size );
	}


	
	do {
		// emty buffer
		RCX5comm::write_buffer(sendquery, DATALNK_SEND_BUFFER_SIZE, 0);

		unsigned char query[] = {0x01, 0x40, 0x09, 0x00, 0x54, 
								 hardwareID[0],
								 hardwareID[1], 
								 hardwareID[2], 
								 0xb3, 0x00, 
								 sess_no,
								 accumulate.uchar[0], //LSB least significant byte
								 accumulate.uchar[1], //MSB most significant byte 
								 0x00, 0x00,
								 packet.uchar[0],
								 packet.uchar[1]};

		memmove(sendquery, query, sizeof(query));

		printf("command:");
		for (int z=0; z < sizeof(query); z++)
			printf("%x ",query[z]);
		printf("\n");

		// send command 
		this->driver->sendbytes(sendquery, sizeof(sendquery));
		do {
			usleep(1000);
			len = this->driver->recvbytes(buf);
		}while (len != RCX5_TRANSFER_BUFFER_SIZE);

		print_bytes((char*)buf, len);
		printf("There are %i packets left to transfer\n",nodes_left-1);


		// this is a bit nasty but makes life much more easy at raw session 
		// parsing with the existing function
		// we just cut off the transferbuffer witch is 512 to the packet 
		// size of 446
		// and the last packet (nodes left 1) gets cut to its calculated size
		if (nodes_left == 1) {
			checksum += sess_len % packet_size;
			//overwrite len with packet size
			len = sess_len % packet_size;
		}
		else {
			checksum += packet_size;
			//overwrite len with packet size
			len = packet_size;
		}
		//add header
		len += RCX5_ALL_OTHER_PACKETS_CORRECTION_HEAD;

		//put the received data into a node list
		Datanode node(buf, len);
		l.push_back(node);
		nodecount++;
		nodes_left--;


		printf("get sessiondata bytes: %d / %d\n\n\n",checksum, sesschecksum);




		//finally calculate values for the next query
		accumulate.intvalue += packet_size;
		if(1 == nodes_left ) {
			packet.intvalue = sess_len % packet_size;
		}
		else {
			packet.intvalue = packet_size;
		}

	} while (nodes_left > 0);

return l;
}







/*
 *  success command??
 *  
 * 
 * 
 * 
 * 
 */
void RCX5comm::success(void) {
	
	unsigned char sendquery[DATALNK_SEND_BUFFER_SIZE];
	
	unsigned char query[] = {0x01,0x40,0x04,0x00,0x54,
							 hardwareID[0],
							 hardwareID[1],
							 hardwareID[2],
							 0xb7,0x00,0xff};
	memmove(sendquery, query, sizeof(query));
	this->driver->sendbytes(sendquery, sizeof(sendquery));
}







/*
 *  disconnect the watch
 * 
 * 
 * 01 40 04 00 54 4d 34 1e b7 00 ff  work in progress command (appears quite often)
 * 01 40 04 00 54 4d 34 1e b7 00 00 01 finish - escape sequence? 
 * 
 * 
 */
void RCX5comm::disconnect(void) {

	unsigned char sendquery[DATALNK_SEND_BUFFER_SIZE];
	//XXX lets try this escape sequence
	unsigned char query[] = {0x01,0x40,0x04,0x00,0x54,
							 hardwareID[0],
							 hardwareID[1],
							 hardwareID[2],
							 0xb7,0x00,0x00,0x01};
	memmove(sendquery, query, sizeof(query));
	this->driver->sendbytes(sendquery, sizeof(sendquery));
}



/*
 *
 * try to establish a connection between host and watch
 *
 */
int RCX5comm::pairing(void){

	unsigned char sendquery[DATALNK_SEND_BUFFER_SIZE];
	unsigned char rbuf[DATALNK_RECV_BUFFER_SIZE];
	int rlen=0;


	bool pairingSuccess; 
	int ret;

	const int pairingID = 9047; // four digits 
	char pairingIDarry[5];
	sprintf(pairingIDarry, "%d", pairingID);

	//fix ASCII to int
	pairingIDarry[0] -= 0x30;
	pairingIDarry[1] -= 0x30;
	pairingIDarry[2] -= 0x30;
	pairingIDarry[3] -= 0x30;

	// pairing call 
	// asking the watch to accept connections with a given ID
	// query[10-13] bcd pairing ID 0x09, 0x0, 0x04, 0x07  -> ID 9047
	// if pairing id does exists
	// 
	// the following read
	// response is 1 byte long with 0x00 
	//                      01  40   06   00   54   1a   4e   35   b6     00 00 04 08 03
	// the read request is sent up to 4 times
//	unsigned char q[] = {0x01,0x40,0x06,0x00,0x54, 0x4d,0x34,0x1e, 0xb6,0x00,0x09,0x00,0x04,0x07};
	unsigned char q[] = {0x01,0x40,0x06,0x00,0x54, 
							hardwareID[0], //hardwareID
							hardwareID[1],
							hardwareID[2], 
							0xb6,0x00,
							pairingIDarry[0],
							pairingIDarry[1],
							pairingIDarry[2],
							pairingIDarry[3]};

	// the following read commands 
	// 00
	// 04 40 03 00 40 b6 00 02 
	// 04 42 03 00 40 b6 00 02
	// 04 42 03 00 40 b6 00 02 

	// FIXME looks like the response when the user clicks
	//       no when he gets asked for pairing
	//       dont handle this for now 15.02.2013
	// 05 07 b6 22 03 80 13 70 04 00 00 e9 d1 02 3a
	// 05 07 b6 02 03 00 13 70 04 00 00 e9 d1 02 6a

	memmove(sendquery, q, sizeof(q));

	int counter  = 1;

	printf("send pairing command ...\nlook at your watch to accept the request %d\n",pairingID);
	do{
		printf("debug: send pairing command %d\n",counter);
		this->driver->sendbytes(sendquery, sizeof(sendquery));

		int rcounter = 1;
		do {
			usleep(10*1000);
			printf("debug: read %d\n",rcounter);
			rlen = this->driver->recvbytes(rbuf);
			print_bytes ((char*)rbuf,rlen);
			rcounter++;
		}while (rcounter <= 5 && rlen != DATALNK_RECV_BUFFER_SIZE); // try it X times

		//response
		//04 42 03 00 40 b6 00 01 00 00 00 00 00 00 00 00
		if(rbuf[7] == 1) {
			pairingSuccess = true;
			printf("sync connection has opend! \n");
			break;
		}

		// waiting for accepting pairing
		//response
		//04 42 03 00 40 b6 00 02 00 00 00 00 00 00 00 00
		else if(rbuf[7] == 2) {
			usleep(1000);
			pairingSuccess=false;
		}
		
		sleep(5); // XXX: think about the timing 
		counter++;
	}while(counter <= 10 && !pairingSuccess);

	this->idle();
	this->idle2();

	if(pairingSuccess){
		return 1;
	}
	else {
		return -1;
	}
}



//
// once the datalnk device is setup, it is looking for HRM 
// next to it. the function is periodic reading form the usb 
// waiting to receive data with a protocol format that includes 
// information with the avaible watch.
//
int RCX5comm::findWatch(int retry){

	int counter=0;
	int IDNumber=0;
	unsigned char rbuf[DATALNK_RECV_BUFFER_SIZE];
	int rlen=0;
	unsigned char report[] = {0x04, 0x42, 0x20 };

	printf("start searching for a watch ...");

	do {
		usleep(1000);
		rlen = this->driver->recvbytes(rbuf);

		printf("...");

	// (0 == memcmp(rbuf,report, sizeof(report)*3 ))
		if( DATALNK_RECV_BUFFER_SIZE == rlen 
		&& rbuf[0] == report[0]
		&& rbuf[1] == report[1]
		&& rbuf[2] == report[2]){

			//FIXME this is a dirty hack to save the 
			// hardwareID for further communication
			// there are much more nicer ways of implementation
			// but lets see if it works out!
			if (rbuf[4]==0x10 && rbuf[8]==0x10) {
				//sort the hardwareID for better usage
				hardwareID[0]=rbuf[7];
				hardwareID[1]=rbuf[6];
				hardwareID[2]=rbuf[5];
			}
				
			IDNumber  = unbcd(rbuf[21])*100000;
			IDNumber += unbcd(rbuf[22])*10000;
			IDNumber += unbcd(rbuf[23])*1000;
			IDNumber += unbcd(rbuf[24])*100;
			IDNumber += unbcd(rbuf[25])*10;
			IDNumber += unbcd(rbuf[26])*1; 

			return IDNumber;
		}
		counter++;
	} while(counter < retry );
	return -1;
}




// FIXME
// this fuction is a periodic report that appears quite often in the logs
// it looks like a some kind of report to stay connected
//
void RCX5comm::idle(void){

	unsigned char sendquery[DATALNK_SEND_BUFFER_SIZE];

	unsigned char y[] = {0x01,0x40,0x01,0x00,0x51}; //send the command down
	memmove(sendquery, y, sizeof(y));
	this->driver->sendbytes(sendquery, sizeof(sendquery));
}

// data coming back                          XX
// 05 07 00 00 03 81 13 70 04 00 00 e9 d1 02 32





//
// this fuction is a periodic report that appears quite often in the logs
// it looks like a some kind of report to stay connected
//
void RCX5comm::idle2(void){

	unsigned char sendquery[DATALNK_SEND_BUFFER_SIZE];

	unsigned char y[] = {0x01,0x40,0x01,0x00,0x55};
	memmove(sendquery, y, sizeof(y));
	this->driver->sendbytes(sendquery, sizeof(sendquery));
}

// data coming back
// 05 07 00 00 03 80 13 70 04 00 00 e9 d1 02 XX
// XX data changes






//
// emty a given buffer with a specific char
//
const void RCX5comm::write_buffer(unsigned char* buf, int len, unsigned char filler){

	for (int i=0; i<= len; i++) {
		buf[i]= filler;
	}
}
