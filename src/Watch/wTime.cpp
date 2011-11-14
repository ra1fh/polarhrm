

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
			  << setw(1) << (int)this->thenth;

	return outStream.str();
	
}


double wTime::toDouble() {

	return (double)hour* 3600 + (double)minute * 60 + (double)second + (double)thenth * 0.1;
}




wTime::wTime(unsigned char hour, unsigned char minute, unsigned char sec, unsigned char thenth){

	this->hour = hour;
	this->minute = minute;
	this->second = sec;
	this->thenth = thenth;
}



wTime::wTime(void){

}
