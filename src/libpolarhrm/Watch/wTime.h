#ifndef _WTIME_H
#define _WTIME_H


#include <string>
using namespace std;


class wTime {
	
  private:
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    unsigned char tenth;


  public:

	wTime(void);
	wTime(unsigned char hour, unsigned char minute, unsigned char sec, unsigned char tenth);
	wTime operator+(wTime &);
	
	string toString();
	double toDouble();

	void setHour(unsigned char h){this->hour=h;}
	unsigned char getHour(void){return this->hour;}

	void setMinute(unsigned char m){this->minute=m;}
	unsigned char getMinute(void){return this->minute;}

	void setSecond(int );
	void setSecond(unsigned char s){this->second = s;}
	unsigned char getSecond(void){return this->second;}

	void setTenth(unsigned char t){this->tenth = t;}
	unsigned char getTenth(void){return this->tenth;}
	

};
#endif
