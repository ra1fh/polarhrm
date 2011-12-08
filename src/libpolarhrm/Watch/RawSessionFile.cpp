
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "RawSessionFile.h"

#include "../file_operations.h"



RawSession &RawSession::operator=(const RawSession &rhs){ // rhs: right hand side

	if (this == &rhs) // protect against invalid self-assignment
		return *this; // See "4:"

	// 1: deallocate old memory
	delete[] rawbuffer;

	// 2: allocate new memory and copy the elements
	rawbuffer = new unsigned char[rhs.length];
	//std::copy(rhs.array, rhs.array + rhs.count, array);

	// 3: assign the new memory to the object
	length = 99;

	// 4: Return with *this in order to allow a=b=5 like semantics.
	return *this;
}





void RawSession::setRawBuffer(unsigned char *buffer, int len){
	this->rawbuffer = new unsigned char[len];
	memcpy(this->rawbuffer ,buffer, len);
	this->length = len;
}



unsigned char* RawSession::getRawBuffer(void){
	return this->rawbuffer;
}




int RawSession::getRawBufferlen(void){
	return this->length;
}



// just for debugging reasons
void RawSession::print(void){
	printf ("\nprint RawSessionFile print()\n");
	for (int n=0; n<this->length; n++ ){
		printf("%X",this->rawbuffer[n]);
	}
}



void RawSession::saveRaw(string filename) {
	ofstream outfile (filename.c_str(),ofstream::binary);
	// write to outfile
	outfile.write ((char*)rawbuffer,length);
	outfile.close();
}




void RawSession::readRaw(string filename) {

	int retval;

	retval= does_file_exist ((char*)filename.c_str());

	if (true == retval) {

		FILE *fp;
		int ret;

		length=fsize((char*)filename.c_str());

		rawbuffer = new unsigned char[length] ;
		
		fp=fopen((char*)filename.c_str(),"r");

		ret = fread(rawbuffer, sizeof(unsigned char), length, fp);

		if (ret != length) {
			printf("error reading file %s",filename.c_str());
			exit (-1);
		}

		fclose(fp);
	}
}