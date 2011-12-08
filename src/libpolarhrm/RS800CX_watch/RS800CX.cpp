// RS800CX.cpp
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


#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>


#include "RS800CX.h"

#include "../Driver/driver.h"
#include "../Driver/irda_driver.h"

#include "RS800CX_comm.h"
#include "RS800CX_parse.h"

#include "../Watch/Overview.h"
#include "../Watch/RawSessions.h"
#include "../Watch/RawSessionFile.h"
#include "../Watch/HrmFile.h"
#include "../file_operations.h"
#include "../../cli/polarhrm_config.h"

#include <list>
#include "../Datanode.h"

RS800CX::RS800CX(void){

	driver = new Irda_driver;
	parser = new RS800CXparse;
	watchcomm = new RS800CXcomm(driver);

}


void RS800CX::saveHRM(void){

		unsigned char buf[BUF_SIZE];
		int i,len;


		this->watchcomm->setDriver(driver);
		this->watchcomm->initDriver();

		Overview *w_overview = new Overview;
		this->watchcomm->getOverview(buf, len);

		cout << "w_overview len " << len << endl;
		// show up what's in the overview buffer
		for (i=0; i<= len; i++) {
			cout << hex << (int)buf[i] << " ";
		}
		cout << endl;


		w_overview = this->parser->parseOverview(buf, len);

		 std::list<Datanode> nodelist;

		RawSessions *allraw_sessions = new RawSessions();
		allraw_sessions->setSessionNumber(w_overview->getSessionNumber());

		RawSession *raw_session;

		if (0 < w_overview->getSessionNumber()) {

			// get session raw data including protocoll data and store them in a node list
			for (int sess_no=1; sess_no <= w_overview->getSessionNumber(); sess_no++) {

				nodelist = this->watchcomm->getSession(sess_no);

				raw_session = new RawSession();
				raw_session = RS800CXparse::parseRawSession(&nodelist);

				allraw_sessions->setRawSession(raw_session, sess_no);
			}
		}

		else {
			std::cout<< "nothing to do! no sessions on the device " << endl;
		}

		// once all raw data is transfered just close the connection to the watch 
		this->watchcomm->disconnect();
		this->watchcomm->closeDriver();

		if (0 < w_overview->getSessionNumber()) {

			// now we go allong all sessions and print nice HRM files 
			// in future we can snap in for printing XML or what ever
			// note! we start counting at 1
			// it is importend to know that shifting the index from watch orignal to 
			// array-index 0 for storing data does not get messed up!
			for (int i=1; i<=w_overview->getSessionNumber(); i++ ) {

				RawSession *raw_session;
				raw_session = allraw_sessions->getRawSession(i);

				//raw_session->print();


				//#ifdef DUMP_RAW
				std::string raw_path;
				char filename[40];
				sprintf(filename, "rs800_session%02d", i);
				raw_path = create_filepath(MYPATH,filename,DUMP_EXTENTION);

				raw_session->saveRaw(raw_path);
				//#endif

/* FIXME leave this commented till parsing shows more data.

				Session *session;
//				session = this->parser->parseSession(raw_session);

				//create the filename string for the session
				session->setFileExtention(HRM_EXTENTION);
				//FIXME set a parameter to overwirte an existing file 
				session->createFilename(MYPATH);


				//#ifdef DUMP_RAW
				std::string dump_path;
				dump_path = create_filepath(MYPATH,session->getFilename().c_str(),DUMP_EXTENTION);
				rename(raw_path.c_str(), dump_path.c_str());
				//#endif


				std::string hrmpath;
				hrmpath = create_filepath(MYPATH,session->getFilename().c_str(),HRM_EXTENTION);

				HrmFile *hrmfile = new HrmFile(this->monitor,this->version);
				hrmfile->setPath(hrmpath);
				hrmfile->save(session);

				//#ifdef DUMP_RAW
				std::cout<< "saved dump session number " << i << " @ " << dump_path << std::endl;
				//#endif
				std::cout << "saved hrm session number " << i << " @ " << hrmpath << std::endl;

				// emty memory
				delete raw_session;
				delete session; */
			} // end for 
		} //end if 

}


void RS800CX::eraseSessions(void) {

	unsigned char buf[BUF_SIZE];
	int len;

	printf("not implemented!\n");

	this->watchcomm->setDriver(driver);
	this->watchcomm->initDriver();

	Overview *w_overview = new Overview;
	this->watchcomm->getOverview(buf, len);
	w_overview = this->parser->parseOverview(buf, len);
	//std::cout<< "Sessions "<< w_overview->getSessionNumber() << " Bytes " << w_overview->getUsedBytes() << std::endl;

	//this is working
	//this->watchcomm->deleteAllFiles();

	this->watchcomm->testsomething();

	// once all data is transfered just close the connection to the watch 
	this->watchcomm->disconnect();
	this->watchcomm->closeDriver();


}


void RS800CX::openRaw(std::string rawfilepath){

		RawSession *rawsession = new RawSession();

		rawsession->readRaw(rawfilepath);
		std::cout<< "read rawsession file " << rawsession->getRawBufferlen()<< std::endl;

		Session *session = new Session();
		session = parser->parseSession(rawsession);

		HrmFile *hrmfile = new HrmFile(monitor,version);

		//overwrite existing file
		std::string path;
		path.assign(MYPATH);path.append(session->id);path.append(HRM_EXTENTION);

		hrmfile->setPath(path);
		hrmfile->save(session);

}
