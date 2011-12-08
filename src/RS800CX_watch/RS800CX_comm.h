/*
 * RS800CX_comm.h
 *
 * Copyright (C) 2011 - Thomas Foyth
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

#ifndef _RS800CXCOMM_H
#define _RS800CXCOMM_H

#include "RS800CX.h"
#include "RS800CX_commands.h"

#include "Watch/Overview.h"
#include "polarhrm_config.h"

#include <list>
#include "Datanode.h"


// values for data transfer

// fix the fact that the datalength starts at byte 1
// but saving the data in the array beginns at index 0
#define RS800CX_ZERO_INDEX_FIX 1

#define RS800CX_FIRST_PACKET_CORRECTION_HEAD 7
#define RS800CX_FIRST_PACKET_CORRECTION_TAIL 0 //looks like there is no tail to 
												// cut off!! 
												// it figured out during 
												// testing the lap byte offset

#define RS800CX_ALL_OTHER_PACKETS_CORRECTION_HEAD 5
#define RS800CX_ALL_OTHER_PACKETS_CORRECTION_TAIL 0



class Driver;

class RS800CXcomm {

	public:

	RS800CXcomm(Driver * d);

	void setDriver(Driver * d);
	void initDriver(void);
	void closeDriver(void);
		
	void getOverview(unsigned char *raw_buffer, int &len);

	std::list<Datanode> getSession(int);
	void deleteFile(unsigned char year,unsigned char month,unsigned char day,unsigned char hour,unsigned char minute,unsigned char second);
	void deleteAllFiles(void);

	void testsomething(void);
	void getUser(unsigned char &raw_user_data, int &len);

	int getReminder(unsigned char data[], unsigned char rem_num);	
	int setReminder(unsigned char data[], unsigned char len);

	// send the disconnect command to the watch
	// explecid closing of the driver required 
	void disconnect(void);

  protected:


  private:
	Driver * driver;


};
#endif
