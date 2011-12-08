#ifndef _WTIME_H
#define _WTIME_H


#include <string>
using namespace std;

/*
class WatchTime {
};
*/
class wTime {
	
  private:
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    unsigned char thenth;


  public:

	wTime(void);
	wTime(unsigned char hour, unsigned char minute, unsigned char sec, unsigned char thenth);

	string toString();
	double toDouble();

	void setHour(unsigned char h){this->hour=h;}
	unsigned char getHour(void){return this->hour;}

	void setMinute(unsigned char m){this->minute=m;}
	unsigned char getMinute(void){return this->minute;}

	void setSecond(unsigned char s){this->second = s;}
	unsigned char getSecond(void){return this->second;}

	void setThenth(unsigned char t){this->thenth = t;}
	unsigned char getThenth(void){return this->thenth;}
	

};
#endif
