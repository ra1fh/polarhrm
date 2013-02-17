
#include "Session.h"
#include "Lap.h"
//#include "WatchData.h"
#include "HrmFile.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#include "../file_operations.h"

using namespace std;


void Session::setFileExtention(string ext){

	this->sessExtention = ext;

}



string Session::getFileExtention(void){

	return this->sessExtention;

}



void Session::createFilename(string basepath){

	int year;
	int month;
	int day;
	int counter=1;

	string tmp;
	char * filename;

	year = this->start_date->getshortYear();
	month = this->start_date->getMonth ();
	day = this->start_date->getDay();

	do {

		ostringstream outStream;
		outStream << setfill('0');
		outStream << setw(2) << year
				  << setw(2) << month 
				  << setw(2) << day
				  << setw(2) << counter;


		tmp = outStream.str();

		filename = create_filepath(basepath.c_str(), tmp.c_str(), this->getFileExtention().c_str() );


		cout << "check if file " << filename << " exists? " << does_file_exist(filename) << "\n";


		counter++;

	}while (true == does_file_exist(filename) );

	this->sessFilename = tmp; //without extention 
	this->sessPath = basepath; 
}



string Session::getFilename(void){
	return this->sessFilename;
}


int Session::getRecordingInterval(void){
	return this->recording_interval;
}



void Session::setRecordingInterval(int interval){
	this->recording_interval = interval;
}


wTime* Session::getDuration(void){
	return this->duration;
}

void Session::setDuration(wTime* duration){
	this->duration = duration;
}


void Session::setStartDate(wDate* start_date){
	this->start_date = start_date;
}

wDate* Session::getStartDate(void){
	return this->start_date;
}


void Session::setBestLapTime(wTime* t){
	this->best_laptime = t;
}


wTime* Session::getBestLapTime(void){
	return this->best_laptime;
}


void Session::setHasSpeedData(bool has_speed){
	this->has_speed_data = has_speed;
}

bool Session::getHasSpeedData(void){
	return this->has_speed_data;
}


void Session::setHasCadenceData(bool has_cadence){
	this->has_cadence_data = has_cadence;
}


bool Session::getHasCadenceData(void){
	return this->has_cadence_data;
}


void Session::setHasAltitudeData(bool has_altitude){
	this->has_altitude_data = has_altitude;
}


bool Session::getHasAltitudeData(void){
	return this->has_altitude_data;
}


void Session::setHasPowerData(bool has_power){
	this->has_power_data = has_power;
}


bool Session::getHasPowerData(void){
	return this->has_power_data;
}




void Session::setNumberOfLaps(unsigned int number_of_laps){
	this->number_of_laps = number_of_laps;
}


unsigned int Session::getNumberOfLaps(void){
	return this->number_of_laps;
}



void Session::setNumberOfSamples(unsigned int number_of_samples){
	this->number_of_samples = number_of_samples;
}


unsigned int Session::getNumberOfSamples(void){
	return this->number_of_samples;
}


void Session::setSiUnit(bool si){
	this->si_units = si;
}


bool Session::getSiUnit(void){
	return this->si_units;
}


void Session::setHasPowerLRBData(bool PowerLRB){
	this->has_power_LRB_data = PowerLRB;
}


bool Session::getHasPowerLRBData(void){
	return this->has_power_LRB_data;
}


void Session::setHasPowerPedallingIndexData(bool PowerPedallingIndex){
	this->has_power_pedalling_index_data = PowerPedallingIndex;	/* Power Pedalling Index */
}


bool Session::setHasPowerPedallingIndexData(void){
	return this->has_power_pedalling_index_data;
}


void Session::setHasHRandCCData(bool HR_AND_CC){
	this->has_HR_AND_CC_data = HR_AND_CC;	/* HR/CC data 0 = HR data only, 1 = HR + cycling data */
}


bool Session::getHasHRandCCData(void){
	return this->has_HR_AND_CC_data; 
}


void Session::setHasAirPressureData(bool AirPressure){
	this->has_air_pressure_data = AirPressure;	/* Air pressure (0=off, 1=on) */
}


bool Session::getHasAirPressureData(void){
	return this->has_air_pressure_data;
}



void Session::setHasGPSData(bool GPSData){
	this->has_GPS_data = GPSData;
}



bool Session::getHasGPSData(void){
	return this->has_GPS_data;
}


void Session::setHasHRData(bool HRdata){
	this->has_HR_data = HRdata;
}


bool Session::getHasHRData(void){
	return this->has_HR_data;
}

