#ifndef _SAMPLE_H
#define _SAMPLE_H

#include "Gps.h"

class Sample {
	
	private:
		unsigned int hr;
		unsigned int speed;
		int alt;
		Gps *gps;

	public:
		
		unsigned int getHR();
		void setHR(int hr);

		unsigned int getSpeed();
		void setSpeed(int speed);

		void setAlt(int alt);
		int getAlt();

		void setGps(Gps *);
		Gps* getGps(void);

};
#endif
