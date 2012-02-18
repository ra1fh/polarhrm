#ifndef _SESSION_H
#define _SESSION_H


#include "sportZone.h"
#include "Limit.h"
#include "Sample.h"
#include "Lap.h"
#include "wTime.h"
#include "wDate.h"

class wTime;
class wDate;
class Lap;
class Limit;
class WatchData;
class HrmFile;

class Session {

	public:

	/* TODO: object orientation INFORMATION HIDING */

	char id[31];
	unsigned char name[9];
	unsigned char limit_type;

	Sample **samples;
	Lap **laps;

	unsigned int hr_avg;
	unsigned int hr_max;
	unsigned int hr_min;
	unsigned int user_hr_max;
	unsigned int user_hr_rest;
	unsigned int user_vo2_max;
	unsigned int calories;
	unsigned int distance;
	unsigned int pace_max;
	unsigned int pace_avg;
	unsigned int best_lap;

	int lap_byte_size;
	int sample_size;

	bool activelimit;
	Limit limiterzone[3];

	// there could be more than 5 so we allocate a array of sportzones
	SportZone **sportzones;

	/* this functions are final */

	void createFilename(string path);
	void setFileExtention(string ext);
	string getFileExtention(void);
	string getFilename(void);

	void setRecordingInterval(int);
	int getRecordingInterval(void);

	void setDuration(wTime*);
	wTime* getDuration(void);

	void setStartDate(wDate*);
	wDate* getStartDate(void);

	void setBestLapTime(wTime*);
	wTime* getBestLapTime(void);

	void setHasSpeedData(bool);
	bool getHasSpeedData(void);

	void setHasCadenceData(bool);
	bool getHasCadenceData(void);

	void setHasAltitudeData(bool);
	bool getHasAltitudeData(void);

	void setHasPowerData(bool);
	bool getHasPowerData(void);

	void setHasPowerLRBData(bool);
	bool getHasPowerLRBData(void);

	void setHasPowerPedallingIndexData(bool);
	bool setHasPowerPedallingIndexData(void);

	void setHasHRandCCData(bool);
	bool getHasHRandCCData(void);

	void setHasAirPressureData(bool);
	bool getHasAirPressureData(void);

	void setHasGPSData(bool);
	bool getHasGPSData(void);

	void setHasHRData(bool);
	bool getHasHRData(void);

	void setNumberOfLaps(unsigned int);
	unsigned int getNumberOfLaps(void);

	void setNumberOfSamples(unsigned int);
	unsigned int getNumberOfSamples(void);

	void setSiUnit(bool);
	bool getSiUnit(void);

	private:

	bool has_speed_data;
	bool has_cadence_data;
	bool has_altitude_data;
	bool has_power_data;

	bool has_power_LRB_data;				/* Power Left Right Balance */ 
	bool has_power_pedalling_index_data;	/* Power Pedalling Index */
	bool has_HR_AND_CC_data;				/* HR/CC data 0 = HR data only, 1 = HR + cycling data */
	bool has_air_pressure_data;				/* Air pressure (0=off, 1=on) */

	bool has_HR_data;
	bool has_GPS_data;

	bool si_units;							/* true = kg/cm/km */


	wDate *start_date;
	wTime *duration;
	int recording_interval;

	wTime *best_laptime;

	unsigned int number_of_laps;
	unsigned int number_of_samples;

	string sessPath;
	string sessFilename;
	string sessExtention;







 

//    WatchData * ;

//    HrmFile * ;

};
#endif
