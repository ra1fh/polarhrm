
#include "Sample.h"

unsigned int Sample::getHR() {
	return hr;
}

void Sample::setHR(int hr) {
	this->hr = hr;
}

unsigned int Sample::getSpeed() {
	return speed;
}

void Sample::setSpeed(int speed) {
	this->speed = speed;
}

void Sample::setAlt(int alt) {
	this->alt = alt;
}

int Sample::getAlt() {
	return alt;
}

void Sample::setGps(Gps *gps){
	this->gps = gps;
}

Gps* Sample::getGps(void){
	return this->gps;
}