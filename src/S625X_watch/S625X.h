#ifndef _S625X_H
#define _S625X_H


#define S625X_MAX_PACKET_SIZE 130

#include "Watch/Watch.h"
//#include "WatchData.h"
#include "S625X_parse.h"
#include "S625X_comm.h"

#include "Driver/driver.h"
#include "polarhrm_config.h"

//class Watch;

class S625Xcomm;
class S625Xparse;

//class Driver;


class S625X:public Watch {

  private:
	static const int monitor=DEVICE_S625X;
	static const int version=106;

	//static const int packet_size = S625X_MAX_PACKET_SIZE;
	//int memory_size;

/* TODO */
//WatchData *watchdata;

	S625Xcomm *watchcomm;
	S625Xparse *parser;
	Driver *driver;

  public:
	S625X(void);
	void saveHRM(void);
	void eraseSessions(void);
	void openRaw(std::string);
};
#endif
