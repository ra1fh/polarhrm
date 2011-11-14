
#include "sportZone.h"


int SportZone::number_of_sportzones = 0;


void SportZone::setZoneNumber(unsigned int zone_number){
	this->zone_number = zone_number;
}




unsigned int SportZone::getZoneNumber(void){
return this->zone_number;
}



void SportZone::setLowPercent(unsigned int low_percent){
	this->low_percent = low_percent;
}


unsigned int SportZone::getLowPercent(void){
	return this->low_percent;
}





void SportZone::setHighPercent(unsigned int high_percent){
	this->high_percent = high_percent;
}




unsigned int SportZone::getHighPercent(void){
	return this->high_percent;
}



void SportZone::setOnZoneSeconds(double on_zone_seconds){
	this->on_zone_seconds =  on_zone_seconds;
}


double SportZone::getOnZoneSeconds(void){
	return this->on_zone_seconds;
}