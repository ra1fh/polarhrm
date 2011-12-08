
#include "Overview.h"

Overview::Overview(int number_of_sessions, int used_bytes) {

	this->number_of_sessions = number_of_sessions;
	this->used_bytes = used_bytes;
	
}


int Overview::getSessionNumber(void){

	return number_of_sessions;

}



void Overview::setSessionNumber(int number_of_sessions){

	this->number_of_sessions = number_of_sessions;

}


int Overview::getUsedBytes(void){

	return used_bytes;

}



void Overview::setUsedBytes(int used_bytes){

	this->used_bytes = used_bytes;

}