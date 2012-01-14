#ifndef _HRMFILE_H
#define _HRMFILE_H


#include <string>

#include "Session.h"
using namespace std;

class HrmFile {

  private:

	unsigned char monitor;
	unsigned char version;
	string filename;


  public:

	HrmFile(unsigned char monitor_, unsigned char version_);
	void save(Session *session);
	void setPath(string path);

	void setMonitor(unsigned char);
	unsigned char getMonitor(void);

	void setVersion(unsigned char);
	unsigned char getVersion(void);

};
#endif
