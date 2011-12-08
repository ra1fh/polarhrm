#ifndef _RAWSESSIONFILE_H
#define _RAWSESSIONFILE_H


#include <string>
using namespace std;

class RawSession {
  private:
	unsigned char * rawbuffer;
	int length;

  public:


	RawSession& operator=(const RawSession &);

	void setRawBuffer(unsigned char *buffer, int len);

	unsigned char* getRawBuffer(void);
	int getRawBufferlen(void);

	void saveRaw(string path);
	void readRaw(string path);

	void print(void);
};
#endif
