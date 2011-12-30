#ifndef _WDATE_H
#define _WDATE_H


#include <string>
#include "wTime.h"
#include "../libpolarhrm.h"

using namespace std;

class wTime;

class wDate {
  private:

	wTime *time;

	int year;
	unsigned char month;
	unsigned char day;

  public:

	wDate(void){};
	wDate(int year, unsigned char month, unsigned char day);
	wDate(int year, int month, int day);
	string toString(void);
	string toTimestamp(void);
	string getTimeToString(void);

	void setDate(int year, unsigned char month, unsigned char day);
	void setTime(unsigned char hour, unsigned char minute, unsigned char sec, unsigned char thenth);
	void setTime(wTime*);
	wTime* getTime(void){return this->time;};

	void setYear(int y){this->year = y;}
	int getshortYear(void){return (this->year-BASE_YEAR);};
	int getYear(void){return this->year;};

	void setMonth(unsigned char m){this->month = m;}
	unsigned char getMonth(void){return this->month;};

	void setDay(unsigned char d){this->day = d;}
	unsigned char getDay(void){return this->day;};

	unsigned char getHour(void){return this->time->getHour();}
	unsigned char getMinute(void){return this->time->getMinute();}
	unsigned char getSecond(void){return this->time->getSecond();}
};

#endif
