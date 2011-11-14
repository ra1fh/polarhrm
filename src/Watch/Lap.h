#ifndef _LAP_H
#define _LAP_H


#include "wTime.h"

class wTime;

class Lap {

  public:

	unsigned int lap_no;

	wTime *lap_end_time;
	wTime *laptime;

	bool autolap;
	int hr_end;
	int hr_avg;
	int hr_max;
	int hr_min;
	int speed_end;
	int speed_avg;
	int distance;

};
#endif
