#ifndef _RAWSESSIONS_H
#define _RAWSESSIONS_H


#include "RawSessionFile.h"


class RawSessions {

  private:

	static RawSession **rsessions;

	static unsigned char *rawbuffer;
	static int rawbufferlen;

	static int number_of_sessions;
	static int used_bytes;

  public:

	//RawSessions(int);

	void setRawBuffer(unsigned char *buffer, int len);
	RawSession* getRawSession(int num);
	void setRawSession(RawSession*, int);

	static void split(void);

	int getSessionNumber(void);
	void setSessionNumber(int);

	static int getUsedBytes(void);
	static void setUsedBytes(int);

};
#endif
