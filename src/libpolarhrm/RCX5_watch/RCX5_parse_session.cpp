// RCX5_parse_session.cpp
//
// Copyright (C) 2012 - Thomas Foyth
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.


#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

#include <bitset>
#include <iostream>




#include "../util_functions.h"

#include "RCX5_comm.h"
#include "RCX5_parse.h"

#include "../Watch/Session.h"
#include "../Watch/Sample.h"
#include "../Watch/RawSessionFile.h"
#include "../Watch/wTime.h"
#include "../Watch/wDate.h"
#include "../Watch/sportZone.h"
#include "../Watch/Gps.h"


#define SHOW(x) show(x, &buf[x]);





void RCX5parse::parse_samples(Session *w_session, RawSession *raw_sess){

	unsigned char *buf;
	buf = raw_sess->getRawBuffer();
	buf = &buf[175];

	float speed_val;
	int alt;
	int j=0; // count printed samples

	// FIXME get values form parse calling parse function
	int lap_byte_size = w_session->lap_byte_size;
	int sample_size = w_session->sample_size;
	
	// set up a memory for storing the samples
	w_session->samples =(Sample**) new Sample[w_session->getNumberOfSamples()];



	for (int i = 0; i<=(w_session->getNumberOfSamples()*sample_size); i += sample_size ) {
	
		w_session->samples[j]= new Sample;

		w_session->samples[j]->setHR(0);
		w_session->samples[j]->setSpeed(0);
		w_session->samples[j]->setAlt(0);


		// HR data is allways set
		w_session->samples[j]->setHR(buf[i]);

		#ifdef ENABLE_HR_FIXING
		// it seems that not all data are printed to hrm file. 
		// when data are out of range they are droped but what is the range?
		// here are my assumptions
		 
		//
		// when HR is ZERO write the last knowen value
		//
		if (w_session->samples[j]->getHR() == 0 && j > 0) {

			w_session->samples[j]->setHR (w_session->samples[j-1]->getHR());
			buf[i] = w_session->samples[j-1]->getHR();
		}
		#endif

		printf("i%d\t%d\t",i,w_session->samples[j]->getHR());

		// FIXME when we have gps data
		// for footpot data might look differnent 
		// however for now, getHasSpeedData is equal to GPS data
		if (w_session->getHasSpeedData()){

			int alt_offset = 0;
			if(w_session->getHasAltitudeData ()) {
				alt_offset = 2;
			}

			Gps *gpsWpt = new Gps();

			gpsWpt->setLatitude(toGpsDec(&buf[i+8+alt_offset]));
			gpsWpt->setLongitude (toGpsDec(&buf[i+4+alt_offset]));

			printf("lat %.9f lon %.9f",gpsWpt->getLatitude(), gpsWpt->getLongitude());
			w_session->samples[j]->setGps(gpsWpt);
		}


/*
		if (w_session->getHasAltitudeData()) {

			alt=0;

			if (w_session->getHasSpeedData()) 
				alt = buf[i+1] + ((buf[i+2] & 0x1F)<<8)-512;
	
			w_session->samples[j]->setAlt(alt);
			printf("\t%d",w_session->samples[j]->getAlt());
		}
*/
		printf("\r\n");
		j++;
	}
	printf("sample_size %i\n",sample_size);
	printf("printed samples %d\n",j);

}






void RCX5parse::parse_laps(Session *w_session, RawSession *raw_sess){

	unsigned char *buf;
	int sesslen;
	double prev_lap_endtime=0;

	buf = raw_sess->getRawBuffer();
	sesslen = raw_sess->getRawBufferlen();

	// base lap offset 
	const int lap_offset = 52;

	w_session->laps = (Lap**) new Lap[w_session->getNumberOfLaps()-1];

	int i = raw_sess->getRawBufferlen();
	i=i-13;

	int first_lap_adress;
	int last_lap_adress;

	first_lap_adress = (buf[324]<<8)+buf[323];
	last_lap_adress  = (buf[327]<<8)+buf[326];

	printf("first lap adress %d +39 %d\n",first_lap_adress,first_lap_adress+39);
	printf("last lap adress  %d +39 %d\n",last_lap_adress,last_lap_adress+39);

	for (int lap_no=w_session->getNumberOfLaps(); lap_no>0; lap_no--){

		w_session->laps[lap_no]= new Lap;

		w_session->laps[lap_no]->lap_no=lap_no;
		printf("\n===== LAP %d ===== %d index %d\n",lap_no, buf[i],i );

		// split time 
		wTime *laptime_sum = new wTime();
		laptime_sum->setHour  (unbcd(buf[i+4]));
		laptime_sum->setMinute(unbcd(buf[i+3]));
		laptime_sum->setSecond(unbcd(buf[i+2]));
		laptime_sum->setTenth (unbcd(buf[i+1])*0.1);

		w_session->laps[lap_no]->lap_end_time = laptime_sum;
		printf("%d split time %s\n",i, w_session->laps[lap_no]->lap_end_time->toString().c_str() );
		printf("w_session->lap[%d]->lap_end_time=%f\n",lap_no,w_session->laps[lap_no]->lap_end_time->toDouble());

		//lap time 
		wTime *laptime = new wTime();
		laptime->setHour  (unbcd(buf[i+8]));
		laptime->setMinute(unbcd(buf[i+7]));
		laptime->setSecond(unbcd(buf[i+6]));
		laptime->setTenth (unbcd(buf[i+5])*0.1);

		w_session->laps[lap_no]->laptime = laptime;
		printf("%d lap time %s\n",i, w_session->laps[lap_no]->laptime->toString().c_str() );
		printf("w_session->lap[%d]->laptime=%f\n",lap_no, w_session->laps[lap_no]->laptime->toDouble());
		printf("%f\n",w_session->laps[lap_no]->laptime->toDouble()-(w_session->laps[lap_no]->laptime->getTenth()*0.1));

//		printf("start lap HR %d\n",buf[i+3]);

		if (w_session->getHasHRData()){
			w_session->laps[lap_no]->hr_end = (unsigned int) buf[i-17];
			w_session->laps[lap_no]->hr_avg = (unsigned int) buf[i-15];
			w_session->laps[lap_no]->hr_max = (unsigned int) buf[i-13];

			printf("end HR = %d 0x%.2X\n",w_session->laps[lap_no]->hr_end,w_session->laps[lap_no]->hr_end);
			printf("avg HR = %d 0x%.2X\n",w_session->laps[lap_no]->hr_avg,w_session->laps[lap_no]->hr_avg);
			printf("max HR = %d 0x%.2X\n",w_session->laps[lap_no]->hr_max,w_session->laps[lap_no]->hr_max);
		}
		if (w_session->getHasGPSData()) {
			double longitude; 
			double latitude;
			latitude  = toGpsDec(&buf[i-5]);
			longitude = toGpsDec(&buf[i-9]);
			printf("lat %.9f lon %.9f\n",latitude, longitude);
		}
		// XXX this is a dirty workaround to parse the laps
		// the issue is that I cannot calculate correct aumount of data 
		// that is stored within a lap! :-(

		int actualIndex;
		actualIndex = i;
		
/*		if(lap_no > 0) {
			i = i-lap_offset;
			i = findNextLapOffset (buf,i,lap_no-1);
		}
*/
		int prev_adress;
		int next_adress;

		next_adress =(buf[i-34])<<8;
		next_adress += buf[i-35];
		prev_adress =(buf[i-38])<<8;
		prev_adress += buf[i-39]; 

		printf("next adress %.2X %.2X\nshifted %d x+39 %dx\n\n ",buf[i-34], buf[i-35],next_adress,next_adress+39 );
		printf("prev adress %.2X %.2X\nshifted %d x+39 %dx\n ",buf[i-38], buf[i-39],prev_adress,prev_adress+39 );

		if(lap_no == 1) {
			//prev_adress = first_lap_adress-52;
			prev_adress = 328-52+16;
		}		
		
		printf("How is HR data stored?(end / avg / max HR is well knowen): %d - %d\n",prev_adress+52, actualIndex);
		for (int k =prev_adress+52; k<actualIndex; k++) {
			printf("%.2X ",buf[k]);
		}
		//FIXME Helps to deploy HR encoding. 
		// looks like Delta + length encoding
		// thanks to Manuel Ruß for figuring it out :-))
		printf("\nbinary\n");
		for (int k =prev_adress+52; k<actualIndex; k++) {
			//printf("%d ",buf[k]);
			 cout<<std::bitset<8>(buf[k])<<" ";
		}
		printf("\n");


		i=prev_adress+39;


	}
}





void RCX5parse::parse_sportzones(Session *w_session, RawSession *raw_sess){

	unsigned char *buf;
	buf = raw_sess->getRawBuffer();
	int sesslen;
	sesslen = raw_sess->getRawBufferlen();

	/* Sport zone limits */
	SportZone::number_of_sportzones=5;
	printf("?? Number of SportZones=%d\n", SportZone::number_of_sportzones);
	w_session->sportzones = (SportZone**)new SportZone[SportZone::number_of_sportzones];
	for (int i = 0; i < SportZone::number_of_sportzones; i++) {

		w_session->sportzones[i] = new SportZone();
		w_session->sportzones[i]->setZoneNumber(i+1);
		w_session->sportzones[i]->setLowPercent (buf[202 + (2 * i)]); //FIXME here we have absolute HR values!
		w_session->sportzones[i]->setHighPercent(buf[204 + (2 * i)]); // add logic to sportzone class to handle percent and absolute values! 

		wTime onzone;
		int base_index = 264;
		onzone.setHour  ((unsigned char) unbcd(buf[base_index +2 +i*3]));
		onzone.setMinute((unsigned char) unbcd(buf[base_index +1 +i*3]));
		onzone.setSecond((unsigned char) unbcd(buf[base_index +   i*3]));
		onzone.setTenth ((unsigned char) 0);

		w_session->sportzones[i]->setOnZoneSeconds(onzone.toDouble());
//		printf("Zone %d %s\n",i+1,(char *)onzone.toString().c_str() );

		if (i < SportZone::number_of_sportzones-1) {
			printf("?? Zone Z%d form\t%d to %d\t%gsec\n", 
			       w_session->sportzones[i]->getZoneNumber(),
			       w_session->sportzones[i]->getLowPercent(),
			       w_session->sportzones[i]->getHighPercent()-1,
			       w_session->sportzones[i]->getOnZoneSeconds());
		}
		else {
			printf("?? Zone Z%d form\t%d to %d\t%gsec\n", 
			       w_session->sportzones[i]->getZoneNumber(),
			       w_session->sportzones[i]->getLowPercent(),
			       w_session->sportzones[i]->getHighPercent(),
			       w_session->sportzones[i]->getOnZoneSeconds());
		}
	}
}






void RCX5parse::parse_gpsdata(Session *w_session, RawSession *raw_sess){

}





// main session parsing function
Session* RCX5parse::parseSession(RawSession *raw_sess){

	unsigned char *buf;
	buf = raw_sess->getRawBuffer();
	int sesslen = raw_sess->getRawBufferlen();

	Session *w_session = new Session();



	printf("lines printed with OK seem to be correct parsed\n"
			"lines that have one or more questionmarks at the beginning \n"
			"need to be checked - and confirmed\n");

	printf("sesslen %i\n",sesslen);
	

	//w_session->limit_type;


	// FIXME dont know where this is stored
	w_session->setSiUnit(true);

	// Start date of the session
	wDate* sessStartDate = new wDate();
	sessStartDate->setYear((int)BASE_YEAR + lnib(buf[37]) );
	sessStartDate->setMonth(buf[36]);
	sessStartDate->setDay(buf[35]);
	w_session->setStartDate(sessStartDate);
	printf("OK Date %s\n",w_session->getStartDate()->toString().c_str());


	// Start time of the session
	wTime* sessStartTime = new wTime();
	sessStartTime->setHour(unbcd(buf[34])); 
	sessStartTime->setMinute(unbcd(buf[33])); 
	sessStartTime->setSecond(unbcd(buf[32]));
	sessStartTime->setTenth((unsigned char) 0);
	sessStartDate->setTime(sessStartTime);
	printf("OK Start Time %s\n",sessStartTime->toString().c_str());


	// create a fileneame for later saving
	// the extention is added on demand .hrm .dump or what ever 
	snprintf(w_session->id,31,"%d-%d-%d_%d_%d",
							w_session->getStartDate()->getYear(),
							w_session->getStartDate()->getMonth(),
							w_session->getStartDate()->getDay(),
							w_session->getStartDate()->getHour(),
							w_session->getStartDate()->getMinute());


	// Session Duration
	wTime *duration =new wTime();
	duration->setHour( unbcd (buf[31]) );
	duration->setMinute (unbcd (buf[30]));
	duration->setSecond (unbcd (buf[29]));
	duration->setTenth(unbcd(buf[28])*0.1); //tenth
	w_session->setDuration(duration);
	printf("OK w_session->duration=%f\n",w_session->getDuration()->toDouble());
	printf("OK w_session->duration=%s\n",w_session->getDuration()->toString().c_str());


	// Session HR values 
	w_session->hr_avg=buf[194];
	w_session->hr_max=buf[198];
	w_session->hr_min=buf[196];
	printf("OK w_session->hr_avg=%d\n",w_session->hr_avg);
	printf("OK w_session->hr_max=%d\n",w_session->hr_max);
	printf("OK w_session->hr_min=%d\n",w_session->hr_min);


	// user related Data
	w_session->user_hr_max = buf[212];
	w_session->user_hr_rest = buf[47];
	w_session->user_vo2_max = buf[43];
	printf("?? w_session->user_hr_max=%d\n",w_session->user_hr_max);
	printf("OK w_session->user_hr_rest=%d\n",w_session->user_hr_rest);
	printf("OK w_session->user_vo2_max=%d\n",w_session->user_vo2_max);

	w_session->calories = toshort(&buf[192]);
	printf("OK w_session->calories=%d\n",w_session->calories);

	int lbs = buf[40];
	int kg = lbs*0.453592 + 0.5;
	printf("OK user_weight=%d need to handle it\n",kg);


	//FIXME figure out the number of stored laps
	// this is a first approach.
	if(buf[292] > 99) {
		w_session->setNumberOfLaps(1);
	}
	else {
		w_session->setNumberOfLaps(buf[292]);
	}
	printf("?? w_session->number_of_laps=%d\n",w_session->getNumberOfLaps());
/*
//	printf("?? number_automated_laps=%d\n",buf[47]); //what is the difference?

	w_session->best_lap = buf[51];
	printf("?? w_session->best_lap=%d\n",w_session->best_lap);
	w_session->pace_max = toshort(&buf[52]);
	printf("?? w_session->pace_max=%d\n",w_session->pace_max);
	w_session->pace_avg = toshort(&buf[54]);;
	printf("?? w_session->pace_avg=%d\n",w_session->pace_avg);

*/

	// laptime and avg laptime
	wTime *best_laptime = new wTime();
	wTime *avg_laptime  = new wTime();

	if (w_session->getNumberOfLaps() != 1) {
		best_laptime->setHour   (unbcd(buf[305]));
		best_laptime->setMinute (unbcd(buf[304]));
		best_laptime->setSecond (unbcd(buf[303]));
		best_laptime->setTenth  (unbcd(buf[302])*0.1);

		avg_laptime->setHour   (unbcd(buf[297]));
		avg_laptime->setMinute (unbcd(buf[296]));
		avg_laptime->setSecond (unbcd(buf[295]));
		avg_laptime->setTenth  (unbcd(buf[294])*0.1);
	}
	else {
		//set end time
		best_laptime = duration;
		avg_laptime = duration;
	}
	w_session->setBestLapTime(best_laptime);
	printf("OK w_session->best_laptime=%f\n",w_session->getBestLapTime()->toDouble());
	printf("OK w_session->best_laptime=%s\n",w_session->getBestLapTime()->toString().c_str());
	//XXX not saving this for now
	printf("OK            avg_laptime=%f\n",avg_laptime->toDouble());
	printf("OK            avg_laptime=%s\n",avg_laptime->toString().c_str());



	// figure out what data is recorded in the session

/*
	printf("?? 18 bit 7 UNKNOWN %d\n",(buf[18]&0x80) >> 7);
	printf("?? 18 bit 6 UNKNOWN %d\n",(buf[18]&0x40) >> 6);
	printf("?? 18 bit 5 UNKNOWN %d\n",(buf[18]&0x20) >> 5);
	printf("?? 18 bit 4 GPS on %d\n",(buf[18]&0x10) >> 4);
	printf("?? 18 bit 3 has Speed/pace data %d\n",(buf[18]&0x08) >> 3);
	printf("?? 18 bit 2 Cadence %d\n",(buf[18]&0x04) >> 2);
	printf("?? 18 bit 1 UNKNOWN %d\n",(buf[18]&0x02) >> 1);
	printf("?? 18 bit 0 Altitude %d\n",buf[18]&0x01);
*/

/* need to find this values
	 * bike 1- 2
	 * speed sensor
	 * cadence
	 * gps G3 on - off
	 * altitude on - off

	 the RCX5 manual tells the impact of switching on or of various settings
	 *	Speed
	 *	Cadence
	 *	S3
	 *	GPS
*/


	// has HR data?
	if (1 == buf[158]) {
		w_session->setHasHRData(true);
	}
	else {
		w_session->setHasHRData(false);
	}

	// has GPS data?
	if (1 == buf[159]) {
		w_session->setHasGPSData(true);
	}
	else {
		w_session->setHasGPSData(false);
	}

	//FIXME has cadence data?
	if(0) {
		w_session->setHasCadenceData (true);
	}
	else {
		w_session->setHasCadenceData (false);
	}

	//FIXME has power data?
	if(0) {
		w_session->setHasPowerData (true);
	}
	else {
		w_session->setHasPowerData (false);
	}

	printf("?? record HR data %d\n",w_session->getHasHRData());
	printf("?? record GPS data %d\n",w_session->getHasGPSData());


	//FIXME session record interval - find where the data is stored?
	//buf[50], buf[114]
	switch (buf[160]) {
		case 0:
			w_session->setRecordingInterval(1);
			break;
		case 1:
			w_session->setRecordingInterval(2);
			break;
		case 2:
			w_session->setRecordingInterval(5);
			break;
		case 3:
			w_session->setRecordingInterval(15);
			break;
		case 4:
			w_session->setRecordingInterval(60);
			break;
	}
	printf("?? w_session->recording_interval=%d\n",w_session->getRecordingInterval());
	printf("?? recording_interval raw buf %d\n",buf[160]);



	/*
	// FIXME i think it goes differnet than rs800cx
	// RCX5 protocoll size is at least ?? 
	// calculating hr sample data length

	w_session->lap_byte_size = lap_byte_size;
	w_session->sample_size = sample_size;

	printf("?? w_session->sample_size = %d\n",w_session->sample_size);

	//w_session->setNumberOfSamples((sesslen-(174+w_session->getNumberOfLaps()*lap_byte_size)-sample_size)/sample_size);
	w_session->setNumberOfSamples((int)floor(w_session->getDuration()->toDouble() / w_session->getRecordingInterval())-1);

	printf("?? w_session->number_of_samples looks correct %d\n",w_session->getNumberOfSamples());
	printf("?? proof of calculation: duration / record interval = %d\n", (int)floor(w_session->getDuration()->toDouble() / w_session->getRecordingInterval()));
	printf ("?? byte 97 number_of_samples seems to be something different %d\n",toshort(&buf[97]));

	*/

/*
	// interesting data
	SHOW(45);
	SHOW(46);
	SHOW(66);
	SHOW(67);
	SHOW(70);
	SHOW(85);
	SHOW(113);
	SHOW(114);
	SHOW(115);
	SHOW(116);
	SHOW(117);

	SHOW(120);
	SHOW(121);
	//SHOW(122);
	SHOW(123);
	SHOW(124);
	SHOW(125);
	SHOW(126);
	SHOW(127);

	SHOW(160);
	SHOW(161);

	SHOW(202);
	SHOW(204);
	SHOW(206);
	SHOW(208);
	SHOW(210);
	SHOW(212);
*/

//FIXME not implemented 
//	parse_samples(w_session, raw_sess);

/*
	//XXX disabled for shorter output
	parse_sportzones (w_session, raw_sess);

*/
	if (w_session->getNumberOfLaps ()>1) {
		parse_laps(w_session, raw_sess);
	}

	return w_session;
}





RawSession* RCX5parse::parseRawSession(std::list<Datanode>* nodelist){

	RawSession *raw_session = new RawSession;

	unsigned char *nodebuf;
	int buflen=0;
	int rawbuf_filled=0;
	int nodecount=0;
	int nodebuflen=0; 

	list<Datanode>::iterator current;

	//calculate the buffer size for the session raw buffer
	// FIXME include calculation of protocol content to give the correct buffer size
	for ( current=nodelist->begin(); current != nodelist->end(); current++ ) {
		buflen += current->getBufLen();
		if (current == nodelist->begin()){
		// handle first node different
			//buflen -= RCX5_FIRST_PACKET_CORRECTION_HEAD;
			//buflen -= RCX5_FIRST_PACKET_CORRECTION_TAIL;
		}
		else {
			//buflen -= RCX5_ALL_OTHER_PACKETS_CORRECTION_HEAD;
			//buflen -= RCX5_ALL_OTHER_PACKETS_CORRECTION_TAIL;
		}
	}

	printf("allocate %d memory\n",buflen);
	unsigned char *rawbuf; // buffer the session without any protocol header
	rawbuf = new unsigned char[buflen]; //FIXME buflen is too large because we
										// didn t subtracted the bytes used by
										// the header ( we will cut off next)


	//cut off protocol head and tailer and fill the raw buffer
	for ( current=nodelist->begin() ; current != nodelist->end(); current++ ) {

		nodebuf = current->getBuffer();
		nodebuflen = current->getBufLen();
		if (0 == nodecount) {
			// do this only at the first node because this are data
			// responsed by initial query!!!
			// cut off RCX5_FIRST_PACKET_CORRECTION_HEAD bytes at the beginning,
			// RCX5_FIRST_PACKET_CORRECTION_TAIL at the end
			nodebuflen -= RCX5_FIRST_PACKET_CORRECTION_HEAD;
			nodebuflen -= RCX5_FIRST_PACKET_CORRECTION_TAIL;
			memcpy(&rawbuf[0], &nodebuf[RCX5_FIRST_PACKET_CORRECTION_HEAD], nodebuflen);
		}

		else {
			// cut off RCX5_ALL_OTHER_PACKETS_CORRECTION_HEAD bytes at the beginning,
			// RCX5_ALL_OTHER_PACKETS_CORRECTION_TAIL at the end
			nodebuflen -= RCX5_ALL_OTHER_PACKETS_CORRECTION_HEAD;
			nodebuflen -= RCX5_ALL_OTHER_PACKETS_CORRECTION_TAIL;
			memcpy(&rawbuf[rawbuf_filled], &nodebuf[RCX5_ALL_OTHER_PACKETS_CORRECTION_HEAD], nodebuflen); 
		}

		printf("node %d\n",nodecount);
		printf("nodelen orignal / cut %d / %d\n",current->getBufLen(), nodebuflen);
		printf("rawbuffer filled %d\n\n",rawbuf_filled);

		// whats in the node buffer
		for (int j=0; j<current->getBufLen(); j++)
			printf("%X ", nodebuf[j]);
		printf("\n\n");

		// whats copied
		for (int j=rawbuf_filled; j<rawbuf_filled+nodebuflen; j++)
			printf("%X ",rawbuf[j]);
		printf("\n\n");

		rawbuf_filled += nodebuflen;
		nodecount++;
	}

	// result
	printf("\nfinal rawbuffer (len %i)\n", rawbuf_filled);
	
	print_bytes ((char*)rawbuf, rawbuf_filled);
	printf("\n\n");


/*
	if(buf_filled != w_overview->getUsedBytes()) {

		printf("missing BYTES ..\n");
		printf("buf_filled %d\n",buf_filled);
		printf("w_overview.used_bytes %d\n",w_overview->getUsedBytes());
		exit (-1);
	}
*/

	raw_session->setRawBuffer(rawbuf, rawbuf_filled);

	return raw_session;
}



/*

// 
//  find the minimum in a not sorted array 
//  this is proberbly a ppp software function
//
//  http://www.computing.net/answers/programming/max-amp-min-values-in-arrays-c/8620.html
//
static unsigned char find_minimum(unsigned char buf[], int len, int stepsize) {

	int i=0;
	unsigned char min;


	do {

		min = buf[i];
		i++;

	} while (min == 0 || i<=len);
	

	printf("minimum start value %d\n",min);

	i=0;
	
	for (i = 0; i < len; i=i+stepsize) { 

		if (buf[i] != 0 && buf[i] < min){
			min = buf[i];
		}
	}

	return min;
}


*/



void RCX5parse::test(unsigned char pos[]) {
    unsigned char hours = pos[2] & 0x7f;
    unsigned char minutes = pos[1] & 0x3f;
    unsigned char seconds = pos[0] & 0x3f;
    /* We can just fit the tents in the extra bits */
    unsigned char tenths = ((pos[1] & 0xc0) >> 4) | ((pos[0] & 0xc0) >> 6);
	printf("time %d:%d:%d.%d\n", hours, minutes, seconds, tenths);
}


// tempurary function for convinient displaying whats in the buffer
void RCX5parse::show(int i, unsigned char *pos){
	std::cout << "?? " << std::dec << i << " byte\t" << std::hex << (int)pos[0] << "\t" << std::dec << (int)pos[0] << "\t";
	std::cout << std::bitset<8>(pos[0]) << std::endl;
}




// the function trys to find the next stating point for parsing lap data
// I dont know why there is a discrepance with the lap offsets
//
//
int RCX5parse::findNextLapOffset(unsigned char *buf, int i, int lap_no){


	printf("search for lap %d @ index %d\n",lap_no,i );



	while (buf[i] != lap_no 	// find lap number
	       ||  buf[i+8] != 0x00
	       ||  buf[i+9] != 0x00
	       || buf[i+10] != 0x00
	       || buf[i+11] != 0x00
	       || buf[i+12] != 0x00){ 

//		printf("%X ",buf[i]);
		i--;
	}

	printf("next index %d\n",i);
	return i;
}
/*
		for (int k =i-39; k<actualIndex; k++) {

			if (lap_no == 1) { //first lap
	
			if(buf[k+0] == 0x00 && //prev
 			   buf[k+1] == 0x00 && //prev
			   buf[k+2] == 0x00 &&
			   buf[k+3] == 0x00 &&
			   buf[k+4] >= 0x00 && //next
			   buf[k+5] >= 0x00) { //next

				  next_adress =(buf[k+5])<<8;
				  next_adress += buf[k+4]; 	  
				  prev_adress =(buf[k+1])<<8;
				  prev_adress += buf[k+0]; 

				  printf("next adress %.2X %.2X\nshifted %d x+39 %dx\n ",buf[k+5], buf[k+4],next_adress,next_adress+39 );
				  printf("prev adress %.2X %.2X\nshifted %d x+39 %dx\n ",buf[k+1], buf[k+0],prev_adress,prev_adress+39 );
				  break;      
			   }
			
			}
			else if(lap_no == w_session->getNumberOfLaps()) {//last lap
			if(buf[k+0] >= 0x00 && //prev
 			   buf[k+1] >= 0x00 && //prev
			   buf[k+2] == 0x00 &&
			   buf[k+3] == 0x00 &&
			   buf[k+4] == 0x00 && //next
			   buf[k+5] == 0x00) { //next

				  next_adress =(buf[k+5])<<8;
				  next_adress += buf[k+4]; 	  
				  prev_adress =(buf[k+1])<<8;
				  prev_adress += buf[k+0]; 

				  printf("next adress %.2X %.2X\nshifted %d x+39 %dx\n ",buf[k+5], buf[k+4],next_adress,next_adress+39 );
				  printf("prev adress %.2X %.2X\nshifted %d x+39 %dx\n ",buf[k+1], buf[k+0],prev_adress,prev_adress+39 );
				  break;      
			   }	
			}

			else {// all other
			
			if(buf[k+0] >= 0x00 && //prev
 			   buf[k+1] >= 0x00 && //prev
			   buf[k+2] == 0x00 &&
			   buf[k+3] == 0x00 &&
			   buf[k+4] >= 0x00 && //next
			   buf[k+5] >= 0x00) { //next

				  next_adress =(buf[k+5])<<8;
				  next_adress += buf[k+4]; 	  
				  prev_adress =(buf[k+1])<<8;
				  prev_adress += buf[k+0]; 
				   
				  printf("next adress %.2X %.2X\nshifted %d x+39 %dx\n ",buf[k+5], buf[k+4],next_adress,next_adress+39 );
				  printf("prev adress %.2X %.2X\nshifted %d x+39 %dx\n ",buf[k+1], buf[k+0],prev_adress,prev_adress+39 );
			      break;
			   }
			}
		}
*/		