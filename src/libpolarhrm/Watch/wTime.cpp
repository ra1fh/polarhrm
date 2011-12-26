

#include "wTime.h"
#include <sstream>
#include <iomanip>

using namespace std;



string wTime::toString() {

	// formated printing
	// http://www.arachnoid.com/cpptutor/student3.html

	ostringstream outStream;
	outStream << setfill('0');
	outStream << setw(2) << (int)this->hour << ":" 
			  << setw(2) << (int)this->minute << ":" 
			  << setw(2) << (int)this->second << "."
			  << setw(1) << (int)this->tenth;

	return outStream.str();
	
}


double wTime::toDouble() {

	return (double)hour* 3600 + (double)minute * 60 + (double)second + (double)tenth * 0.1;
}




wTime::wTime(unsigned char hour, unsigned char minute, unsigned char sec, unsigned char tenth){

	this->hour = hour;
	this->minute = minute;
	this->second = sec;
	this->tenth = tenth;
}



wTime::wTime(void){

	this->hour = 0;
	this->minute = 0;
	this->second = 0;
	this->tenth = 0;
}


wTime wTime::operator+(wTime &rhs){

	int h, m, s, t;

	h = rhs.getHour() ;
	m = rhs.getMinute();
	s = rhs.getSecond();
	t = rhs.getTenth();

	h += this->hour;
	m += this->minute;
	s += this->second;
	t += this->tenth;

	if(t / 10 > 0) {
		s = s + (t / 10);
		t = t % 10;
	}

	if(s / 60 > 0) {
		m = m + (s / 60 );
		s = s % 60;
	}

	if(m / 60 > 0 ) {
		h = h + (m / 60 );
		m = m % 60;
	}

	return wTime(h,m,s,t);
}




void wTime::setSecond(int s){

	if(s / 60 > 0) {
		this->second = s % 60;
		this->minute = (s / 60 );
	}

	else {
		this->second = s;
	}
}