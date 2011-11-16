#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "Datanode.h"



Datanode::Datanode(unsigned char* rawbuf, int rlen){

	this->buf = new unsigned char[rlen];
	this->len = rlen;

	memcpy(this->buf, rawbuf, rlen );	//copy data
}




void Datanode::setBuffer(unsigned char* rawbuf, int rlen){

	this->buf = new unsigned char[rlen];
	this->len = rlen;

	memcpy(this->buf, rawbuf, rlen );	//copy data
}



void Datanode::setBuffer(unsigned char* buf){
	this->buf = buf;
}


unsigned char* Datanode::getBuffer(void){
	return this->buf;
}


void Datanode::setBufLen(int len){
	this->len = len;
}


int Datanode::getBufLen(void){
	return this->len;
}