/*
 * RCX5_comm.h
 *
 * Copyright (C) 2012 - Thomas Foyth
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

#ifndef _RCX5COMM_H
#define _RCX5COMM_H

#include "RCX5.h"
//#include "RCX5_commands.h"

#include "../Watch/Overview.h"


#include <list>
#include "../Datanode.h"


// values for data transfer
#define RCX5_TRANSFER_BUFFER_SIZE 512

// fix the fact that the datalength starts at byte 1
// but saving the data in the array beginns at index 0
#define RCX5_ZERO_INDEX_FIX 1

#define RCX5_PACKETS_INCLUDING_HEADER 446
#define RCX5_FIRST_PACKET_CORRECTION_HEAD 7
#define RCX5_FIRST_PACKET_CORRECTION_TAIL 0 //looks like there is no tail to 
												// cut off!! 
												// it figured out during 
												// testing the lap byte offset

#define RCX5_ALL_OTHER_PACKETS_CORRECTION_HEAD 7
#define RCX5_ALL_OTHER_PACKETS_CORRECTION_TAIL 0



class Driver;

class RCX5comm {

	public:

	RCX5comm(Driver * d);

	void setDriver(Driver * d);
	void initDriver(void);
	void closeDriver(void);
		
	void getOverview(unsigned char *raw_buffer, int &len);

	std::list<Datanode> getSession(int, int);
	void deleteFile(unsigned char year,unsigned char month,unsigned char day,unsigned char hour,unsigned char minute,unsigned char second);
	void deleteAllFiles(void);

	void handshake(void);
	void getUser(unsigned char &raw_user_data, int &len);

	int getReminder(unsigned char data[], unsigned char rem_num);	
	int setReminder(unsigned char data[], unsigned char len);

	int findWatch(int retry);
	void getSessionOverview(unsigned char *raw_buffer, int &len, int sess_no);

	// send the disconnect command to the watch
	// explecid closing of the driver required 
	void disconnect(void);

  protected:


  private:
	Driver * driver;

	const void write_buffer(unsigned char* buf, int len, unsigned char filler);
		

	//dont let people call this
	// only the object knows when to call idle
	void idle(void);
	void idle2(void);
};
#endif
