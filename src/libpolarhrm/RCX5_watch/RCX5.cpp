// RCX5.cpp
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


#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>


#include "RCX5.h"

#include "../Driver/driver.h"
#include "../Driver/datalnk_driver.h"

#include "RCX5_comm.h"
#include "RCX5_parse.h"

#include "../Watch/Overview.h"
#include "../Watch/RawSessions.h"
#include "../Watch/RawSessionFile.h"
#include "../Watch/HrmFile.h"
#include "../file_operations.h"
#include "../libpolarhrm.h"
#include "../Watch/GpxFile.h"
#include <list>
#include "../Datanode.h"

RCX5::RCX5(void){

	driver = new DataLnk_driver();
	parser = new RCX5parse;
	watchcomm = new RCX5comm(driver);

}


void RCX5::saveHRM(void){

		unsigned char buf[BUF_SIZE];
		int i,len;
		int ret, watchID;

		this->watchcomm->setDriver(driver);
		this->watchcomm->initDriver();

		watchID=this->watchcomm->findWatch(20); //try 20 times

		if (watchID > 0) {
			printf("\nfound watch with ID number (serial number?) %d\n",watchID);
			printf("in the received buffer must be the product name string\n");
		}
		else {
			printf("\ndidn' t find any watch!\n");
			printf("please try agian\n");
			exit(EXIT_FAILURE);
		}

		ret = this->watchcomm->pairing();
		if (-1 == ret ) {
			exit(EXIT_FAILURE);
		}

		Overview *w_overview = new Overview;
		this->watchcomm->getOverview(buf, len);

		w_overview = this->parser->parseOverview(buf, len);



		 std::list<Datanode> nodelist;

		RawSessions *allraw_sessions = new RawSessions();
		allraw_sessions->setSessionNumber(w_overview->getSessionNumber());

		RawSession *raw_session;

//XXX just for testing to get only one specific session :-)
// some additonal source code adapitons required
//	int sess_no = 7;
		if (0 < w_overview->getSessionNumber()) {

			// get session raw data including protocoll data and store them in a node list
			for (int sess_no=0; sess_no < w_overview->getSessionNumber(); sess_no++) {

				this->watchcomm->getSessionOverview(buf, len, sess_no);
				int used_bytes = this->parser->parseSessionOverview(buf,len);

				printf ("sess %d bytes %d \n", sess_no, used_bytes);
				nodelist = this->watchcomm->getSession(sess_no, used_bytes);



				raw_session = new RawSession();
				raw_session = RCX5parse::parseRawSession(&nodelist);

				allraw_sessions->setRawSession(raw_session, sess_no);
			}
		}

		else {
			std::cout<< "nothing to do! no sessions on the device " << endl;
		}

		// once all raw data is transfered just close the connection to the watch 
		this->watchcomm->disconnect();
		//this->watchcomm->closeDriver();

		if (0 < w_overview->getSessionNumber()) {

			// now we go allong all sessions and print nice HRM files 
			// in future we can snap in for printing XML or what ever
			// note! we start counting at 1
			// it is importend to know that shifting the index from watch orignal to 
			// array-index 0 for storing data does not get messed up!
			for (int sess_no=0; sess_no<w_overview->getSessionNumber(); sess_no++ ) {

				RawSession *raw_session;
				raw_session = allraw_sessions->getRawSession(sess_no);

				//raw_session->print();


				//#ifdef DUMP_RAW
				std::string raw_path;
				char filename[40];
				sprintf(filename, "rcx5_session%02d", sess_no);
				raw_path = create_filepath(workingDir,filename,dumpExtention);

				raw_session->saveRaw(raw_path);
				//#endif

/*
				Session *session;
				session = this->parser->parseSession(raw_session);

				//create the filename string for the session
				session->setFileExtention(hrmExtention);
				//FIXME set a parameter to overwirte an existing file 
				session->createFilename(workingDir);


				//#ifdef DUMP_RAW
				std::string dump_path;
				dump_path = create_filepath(workingDir,session->getFilename().c_str(),dumpExtention);
				rename(raw_path.c_str(), dump_path.c_str());
				//#endif


				std::string hrmpath;
				hrmpath = create_filepath(workingDir,session->getFilename().c_str(),hrmExtention);

				HrmFile *hrmfile = new HrmFile(this->monitor,this->version);
				hrmfile->setPath(hrmpath);
				hrmfile->save(session);

				//#ifdef DUMP_RAW
				std::cout<< "saved dump session number " << sess_no << " @ " << dump_path << std::endl;
				//#endif
//				std::cout << "saved hrm session number " << sess_no << " @ " << hrmpath << std::endl;

				// emty memory
				delete raw_session;
				delete session; */
			} // end for 
		} //end if 
		
}


void RCX5::eraseSessions(void) {

	unsigned char buf[BUF_SIZE];
	int len;

	printf("not implemented!\n");
/*
	this->watchcomm->setDriver(driver);
	this->watchcomm->initDriver();

	Overview *w_overview = new Overview;
	this->watchcomm->getOverview(buf, len);
	w_overview = this->parser->parseOverview(buf, len);
	//std::cout<< "Sessions "<< w_overview->getSessionNumber() << " Bytes " << w_overview->getUsedBytes() << std::endl;

	//this is working
	//this->watchcomm->deleteAllFiles();

	//this->watchcomm->testsomething();

	// once all data is transfered just close the connection to the watch 
	this->watchcomm->disconnect();
	this->watchcomm->closeDriver();

*/
}


void RCX5::openRaw(std::string rawfilepath){

		RawSession *rawsession = new RawSession();

		rawsession->readRaw(rawfilepath);
		std::cout<< "read rawsession file " << rawsession->getRawBufferlen()<< std::endl;

		Session *session = new Session();
		session = parser->parseSession(rawsession);
/*
		HrmFile *hrmfile = new HrmFile(monitor,version);

		//overwrite existing file
		std::string path;
		path.assign(workingDir);path.append(session->id);path.append(hrmExtention);

		hrmfile->setPath(path);
		hrmfile->save(session);


		//FIXME only write gpx files when gps data are stored in session
		GpxFile *gpxfile = new GpxFile();
		std::string gpxpath;

		gpxpath.assign(workingDir);gpxpath.append(session->id);gpxpath.append(gpxExtention);
		gpxfile->setPath(gpxpath);
		gpxfile->save(session);
*/
}
