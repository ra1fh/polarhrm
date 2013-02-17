// RCX5_parse_overview.cpp
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

#include <stdio.h>

#include "RCX5_parse.h"



#include "../util_functions.h"
#include "../Watch/Overview.h"



Overview* RCX5parse::parseOverview(unsigned char buf[], int len) {

	int number_of_sessions=buf[13]; // assume this!  
	int used_bytes=0; // FIXME there is a seperate query for each session

	//used_bytes  = buf[8] << 8;
	//used_bytes += buf[7];

	Overview *w_overview = new Overview(number_of_sessions, used_bytes);

	print_bytes((char*)buf,len);


	printf("w_overview->getSessionNumber() %d\n",w_overview->getSessionNumber());
	printf("? w_overview->getUsedBytes() %X (int %d) not confirmed\n",w_overview->getUsedBytes(),w_overview->getUsedBytes());


return w_overview;
}



// get the used bytes
int RCX5parse::parseSessionOverview(unsigned char buf[], int len){

	int used_bytes;

	used_bytes  = buf[8] << 8;
	used_bytes += buf[7];


	return used_bytes;
}

