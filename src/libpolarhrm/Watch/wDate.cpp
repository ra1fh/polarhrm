
#include "wDate.h"
#include "wTime.h"

#include <sstream>
#include <iomanip>

using namespace std;


wDate::wDate(int year, unsigned char month, unsigned char day) {
	this->year = year;
	this->month = month;
	this->day = day;
}



wDate::wDate(int year, int month, int day) {
	this->year = year;
	this->month = (unsigned char)month;
	this->day = (unsigned char)day;
}




void wDate::setDate(int year, unsigned char month, unsigned char day) {
	this->year = year;
	this->month = month;
	this->day = day;
}


void wDate::setTime(unsigned char hour, unsigned char minute, unsigned char sec, unsigned char thenth){
	time = new wTime(hour, minute, sec, thenth);
}





string wDate::toString() {

	// formated printing
	// http://www.arachnoid.com/cpptutor/student3.html

	ostringstream outStream;
	
	outStream << setfill('0');
	outStream << setw(4) << (int)year 
			  << setw(2) << (int)month
			  << setw(2) << (int)day;

	return outStream.str();
}


string wDate::getTimeToString(void){
	return this->time->toString();
}


void wDate::setTime(wTime* t){
	this->time = t;
}




string wDate::toTimestamp(void){

	// timestamp
	// YYYY-MM-DDThh:mm:ssTZD 
	// http://www.w3.org/TR/NOTE-datetime

	ostringstream outStream;
	
	outStream << setfill('0');
	outStream << setw(4) << (int)year << "-"
			  << setw(2) << (int)month << "-"
			  << setw(2) << (int)day << "T"
			  << setw(2) << (int) time->getHour() << ":"
			  << setw(2) << (int) time->getMinute() << ":"
			  << setw(2) << (int) time->getSecond () << "Z"; //dont know how to handle this

	return outStream.str();
}