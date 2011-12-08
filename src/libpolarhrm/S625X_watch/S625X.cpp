#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>


#include "S625X.h"

#include "../Driver/driver.h"
#include "../Driver/irda_driver.h"

#include "S625X_comm.h"
#include "S625X_parse.h"


#include "../Watch/Overview.h"
#include "../Watch/RawSessions.h"
#include "../Watch/RawSessionFile.h"
#include "../Watch/HrmFile.h"
#include "../file_operations.h"
#include "../../cli/polarhrm_config.h"

#include <list>
#include "../Datanode.h"

S625X::S625X(void){

	driver = new Irda_driver;
	parser = new S625Xparse;
	watchcomm = new S625Xcomm(driver);

}


void S625X::saveHRM(void){

		unsigned char buf[BUF_SIZE];
		int i,len;


		this->watchcomm->setDriver(driver);
		this->watchcomm->initDriver();

		Overview *w_overview = new Overview;
		this->watchcomm->getOverview(buf, len);
		w_overview = this->parser->parseOverview(buf, len);

		std::list<Datanode> nodelist;

		if (0 < w_overview->getSessionNumber()) {
			// get session raw data including protocoll data and store them in a node list

			nodelist = this->watchcomm->getFiles(w_overview);
		}

		else {
			std::cout<< "nothing to do! no sessions on the device " << endl;
		}

			// once all raw data is transfered just close the connection to the watch 
			this->watchcomm->disconnect();
			this->watchcomm->closeDriver();

		if (0 < w_overview->getSessionNumber()) {

			RawSessions *allraw_sessions = new RawSessions();
			allraw_sessions = S625Xparse::parseRawSessions(w_overview, &nodelist);
			//std::cout<< "RawSessions buf len"<< raw_sess->rawlen << std::endl;

			//emty the nodelist
			nodelist.clear();

			// split raw session to seperate buffers
			allraw_sessions->split();
			std::cout<< "Sessions "<< allraw_sessions->getSessionNumber() << " Bytes " << allraw_sessions->getUsedBytes() << std::endl;
		
			// from this point the nodes can be deleted. all data is stroed in buffer
			// implementation does not work yet
			//delete_all_nodes(&head);


			// now we go allong all sessions and print nice HRM files 
			// in future we can snap in for printing XML or what ever
			// note! we start counting at 1
			// it is importend to know that shifting the index from watch orignal to 
			// array-index 0 for storing data does not get messed up!
			// getRawSession() gets i-1
			for (int i=1; i<=w_overview->getSessionNumber(); i++ ) {

				RawSession *raw_session;
				raw_session = allraw_sessions->getRawSession(i);

				//raw_session->print();

				#ifdef DUMP_RAW
				std::string raw_path;
				raw_path = create_filepath(MYPATH,TEMP_FILENAME,DUMP_EXTENTION);
				raw_session->saveRaw(raw_path);
				#endif

				Session *session;
				session = this->parser->parseSession(raw_session);

				//create the filename string for the session
				session->setFileExtention(HRM_EXTENTION);
				/*TODO set a parameter to overwirte an existing file */
				session->createFilename(MYPATH);


				#ifdef DUMP_RAW
				std::string dump_path;
				dump_path = create_filepath(MYPATH,session->getFilename().c_str(),DUMP_EXTENTION);
				rename(raw_path.c_str(), dump_path.c_str());
				#endif


				std::string hrmpath;
				hrmpath = create_filepath(MYPATH,session->getFilename().c_str(),HRM_EXTENTION);

				HrmFile *hrmfile = new HrmFile(this->monitor,this->version);
				hrmfile->setPath(hrmpath);
				hrmfile->save(session);

				#ifdef DUMP_RAW
				std::cout<< "saved dump session number " << i << " @ " << dump_path << std::endl;
				#endif
				std::cout << "saved hrm session number " << i << " @ " << hrmpath << std::endl;

				// emty memory
				delete raw_session;
				delete session;
			} // end for
		} //end if
}


void S625X::eraseSessions(void) {

	unsigned char buf[BUF_SIZE];
	int len;

	this->watchcomm->setDriver(driver);
	this->watchcomm->initDriver();

	Overview *w_overview = new Overview;
	this->watchcomm->getOverview(buf, len);
	w_overview = this->parser->parseOverview(buf, len);
	//std::cout<< "Sessions "<< w_overview->getSessionNumber() << " Bytes " << w_overview->getUsedBytes() << std::endl;

	//this is working
	this->watchcomm->deleteAllFiles();

	// once all data is transfered just close the connection to the watch 
	this->watchcomm->disconnect();
	this->watchcomm->closeDriver();
}






	void S625X::openRaw(std::string rawfilepath){

		RawSession *rawsession = new RawSession();

		rawsession->readRaw(rawfilepath);
		std::cout<< "read rawsession file " << rawsession->getRawBufferlen()<< std::endl;

		Session *session = new Session();
		session = parser->parseSession(rawsession);

		HrmFile *hrmfile = new HrmFile(monitor,version);

		std::string savepath;
		savepath.assign(MYPATH);
		savepath.append(session->id);
		savepath.append(HRM_EXTENTION);

		hrmfile->setPath(savepath);
		hrmfile->save(session);

	}
