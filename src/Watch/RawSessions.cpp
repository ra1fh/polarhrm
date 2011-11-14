

#include <cstring>
#include <cstdio>

#include "RawSessionFile.h"
#include "RawSessions.h"



int RawSessions::used_bytes=0;
int RawSessions::number_of_sessions = 0;
int RawSessions::rawbufferlen = 0;
unsigned char* RawSessions::rawbuffer = NULL;
RawSession** RawSessions::rsessions = NULL;




int RawSessions::getSessionNumber(void){
	RawSessions::number_of_sessions;
}



void RawSessions::setSessionNumber(int number_of_sessions){
	RawSessions::number_of_sessions = number_of_sessions;

	RawSessions::rsessions = (RawSession**) new RawSession[number_of_sessions-1];

	for (int i=0; i<= number_of_sessions; i++) {
			// create a new raw session!
		RawSessions::rsessions[i] = new RawSession();
	}

}


int RawSessions::getUsedBytes(void){
	RawSessions::used_bytes;
}



void RawSessions::setUsedBytes(int ubytes){
	RawSessions::used_bytes = ubytes;
}



void RawSessions::setRawBuffer(unsigned char *buffer, int len){
	RawSessions::rawbuffer = new unsigned char[len];
	
	memcpy(RawSessions::rawbuffer , buffer, len);
	RawSessions::rawbufferlen = len;
}




void RawSessions::split(void) {

	int sesslen=0;

	RawSessions::rsessions = (RawSession**) new RawSession[number_of_sessions-1];

	for (int i=0; i<=(number_of_sessions-1); i++) {

		//calculating session length
		sesslen= rawbuffer[0]+(rawbuffer[1]<<8);

		// create a new raw session!
		RawSessions::rsessions[i] = new RawSession();

		// pass the single section to a raw session
		RawSessions::rsessions[i]->setRawBuffer(rawbuffer,sesslen);

		//finally shift buffer to next session 
		rawbuffer = &rawbuffer[sesslen];
	}
}




RawSession *RawSessions::getRawSession(int num){

	return rsessions[num-1];

}

void RawSessions::setRawSession(RawSession* raw_sess, int sess_no){

	RawSessions::rsessions[sess_no-1] = raw_sess;

	// create a new raw session!
//	RawSessions::rsessions[sess_no-1] = new RawSession();


	// pass the single section to a raw session
//	RawSessions::rsessions[sess_no-1]->setRawBuffer(rawbuffer,sesslen);


}