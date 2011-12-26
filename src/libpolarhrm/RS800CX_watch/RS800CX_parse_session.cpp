/***************************************************************************
 *            RS800CX_parse_session.c
 *
 *  Sun May 22 17:54:03 2011
 *  Copyright  2011  Thomas Foyth
 *  <thomas.foyth@gmail.com>
 ****************************************************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */



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

#include "RS800CX_comm.h"
#include "RS800CX_parse.h"

#include "../Watch/Session.h"
#include "../Watch/Sample.h"
#include "../Watch/RawSessionFile.h"
#include "../Watch/wTime.h"
#include "../Watch/wDate.h"
#include "../Watch/sportZone.h"
#include "../Watch/Gps.h"


#define SHOW(x) show(x, &buf[x]);



#if DO_COMPILE

bool bike1;
bool bike2;
bool runspeed;
bool altitude;
bool cadence;
bool power;

int lap_byte_size;
int sample_size;

#endif // DO NOT COMPILE


void RS800CXparse::parse_samples(Session *w_session, RawSession *raw_sess){

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






void RS800CXparse::parse_laps(Session *w_session, RawSession *raw_sess){

	unsigned char *buf;
	buf = raw_sess->getRawBuffer();
	int sesslen;
	sesslen = raw_sess->getRawBufferlen();


	int i;

	int hr_index;
	double prev_lap_endtime=0;
	int prev_index;

	int lap_byte_size = w_session->lap_byte_size;



	printf("lap byte size, %d\n\n",lap_byte_size);
	w_session->laps = (Lap**) new Lap[w_session->getNumberOfLaps()-1];

	prev_index = raw_sess->getRawBufferlen();

	for (int j=0;j<=(w_session->getNumberOfLaps()-1);j++){

		w_session->laps[j]= new Lap;

		w_session->laps[j]->lap_no=j+1;
		printf("\n===== LAP %d ===== \n",j+1 );

		i=sesslen-lap_byte_size-(lap_byte_size*j);


		wTime *laptime = new wTime();
		laptime->setHour(buf[i+2] & 0x7F);
		laptime->setMinute(buf[i+1] & 0x3F);
		laptime->setSecond(buf[i] & 0x3F);
		laptime->setTenth(((buf[i+1] & 0xc0) >> 4) | ((buf[i+0] & 0xc0) >> 6)) ;

		w_session->laps[j]->lap_end_time = laptime;
		printf("%d time %s\n",i, w_session->laps[j]->lap_end_time->toString().c_str() );

		printf("w_session->lap[j]->lap_end_time=%f\n",w_session->laps[j]->lap_end_time->toDouble());


		//hr_index = round ((w_session->laps[j]->lap_end_time->toDouble() - prev_lap_endtime) / w_session->getRecordingInterval());
		//prev_lap_endtime = w_session->laps[j]->lap_end_time->toDouble();
	
	
		//printf("search for minimum from %d to %d len %d\n" , prev_index-hr_index, prev_index, hr_index);

		/* need to search in samples for samllest value*/
		//w_session->laps[j]->hr_min = find_minimum (&buf[prev_index-hr_index-sample_size],hr_index, sample_size);
	
		/* shift to next */
		//prev_index = prev_index-hr_index; 


		printf("start lap HR %d\n",buf[i+3]);
		
		w_session->laps[j]->hr_end = (unsigned int) buf[i+3];
		w_session->laps[j]->hr_avg = (unsigned int) buf[i+4];
		w_session->laps[j]->hr_max = (unsigned int) buf[i+5];

		printf("min HR = %d\n",w_session->laps[j]->hr_min);
		printf("avg HR = %d\n",w_session->laps[j]->hr_avg);
		printf("max HR = %d\n",w_session->laps[j]->hr_max);


		if (w_session->getHasAltitudeData()){
			printf("ALT = %d\n",buf[i+18]);
			w_session->laps[j]->alt = buf[i+20];
			printf("Avg ALT of Lap = %d\n",w_session->laps[j]->alt);

			printf("?? temperature = %d\n",(lnib(buf[i+30])<<4) + unib(buf[i+31]) );
		}
		
			// just gps for now
		if (w_session->getHasSpeedData()) {

			double longitude; 
			double latitude;

			latitude  = toGpsDec(&buf[i+22]);
			longitude = toGpsDec(&buf[i+18]);
			printf("lat %.9f lon %.9f\n",latitude, longitude);

		}
	}

}



void RS800CXparse::parse_sportzones(Session *w_session, RawSession *raw_sess){

	unsigned char *buf;
	buf = raw_sess->getRawBuffer();
	int sesslen;
	sesslen = raw_sess->getRawBufferlen();

	/* Sport zone limits */
	SportZone::number_of_sportzones=buf[123];
	printf("?? Number of SportZones=%d\n", SportZone::number_of_sportzones);
	w_session->sportzones = (SportZone**)new SportZone[SportZone::number_of_sportzones-1];
	for (int i = 0; i < SportZone::number_of_sportzones; i++) {
		w_session->sportzones[i] = new SportZone();
		w_session->sportzones[i]->setZoneNumber(i+1);
		w_session->sportzones[i]->setLowPercent(buf[124 + i]);
		w_session->sportzones[i]->setHighPercent(buf[124 + 1 + i]);

		wTime onzone;
		int base_index = 135;
		onzone.setHour((unsigned char)buf[base_index+2 +i*3] & 0x7f);
		onzone.setMinute((unsigned char) buf[base_index+1 +i*3] & 0x3f);
		onzone.setSecond((unsigned char) buf[base_index +i*3] & 0x3f);
		onzone.setTenth((unsigned char) ((buf[base_index+1] & 0xc0) >> 4) | ((buf[base_index] & 0xc0) >> 6));

		w_session->sportzones[i]->setOnZoneSeconds(onzone.toDouble());
		if (i < SportZone::number_of_sportzones-1) {
			printf("?? Zone Z%d form %d to %d%% %gsec\n", 
			       w_session->sportzones[i]->getZoneNumber(),
			       w_session->sportzones[i]->getLowPercent(),
			       w_session->sportzones[i]->getHighPercent()-1,
			       w_session->sportzones[i]->getOnZoneSeconds());
		}
		else {
			printf("?? Zone Z%d form %d to %d%% %gsec\n", 
			       w_session->sportzones[i]->getZoneNumber(),
			       w_session->sportzones[i]->getLowPercent(),
			       w_session->sportzones[i]->getHighPercent(),
			       w_session->sportzones[i]->getOnZoneSeconds());
		}
	}
}


void RS800CXparse::parse_gpsdata(Session *w_session, RawSession *raw_sess){



}





// main session parsing function
Session* RS800CXparse::parseSession(RawSession *raw_sess){

	unsigned char *buf;
	buf = raw_sess->getRawBuffer();
	int sesslen;
	sesslen = raw_sess->getRawBufferlen();

	int chk_sesslen;

	Session *w_session = new Session();

	//0 	Bytes in file (MSB)
	//1 	Bytes in file (LSB)
	chk_sesslen=buf[1]+(buf[0]<<8);

	if (chk_sesslen != sesslen) {
		printf("session length missmatch \n"
		       "Length should be %i but buffer size is %i\n", chk_sesslen, sesslen);
		printf("WE HAVE A PROBLEM !!!\n\n"
		       "It looks like there is 4 byte discrepance\n"
		       "I guess the first 4 bytes count as part of\n"
		       " the protocol head but I am not sure\n\n");
	}

	printf("lines printed with OK seem to be correct parsed\n"
			"lines that have one or more questionmarks at the beginning \n"
			"need to be checked - and confirmed\n");


	printf("sesslen %i\n",sesslen);
	
	/*	
	unsigned char rawdata[7];
	unsigned char encdata[7];
	// copy bytes to rawbuffer and encode 
	memcpy(rawdata, &buf[3], 7);
	watch_decode_string(encdata, rawdata);
	printf("%s\n",encdata);
	

	//	w_session->id[31];
    
	//	strcpy(w_session->name,"FILENAME");
	
	w_session->limit_type;
*/

	// FIXME dont know where this is stored
	w_session->setSiUnit(true);

	unsigned short timestamp = ((buf[22] << 8) | buf[21]);

	wDate* sessStartDate = new wDate();
	sessStartDate->setYear((int)BASE_YEAR + (timestamp >> 9) );
	sessStartDate->setMonth((timestamp & 0x1e0) >> 5);
	sessStartDate->setDay(timestamp & 0x1f);

	wTime* sessStartTime = new wTime();
	sessStartTime->setHour(unbcd(buf[25])&0x3F); 
	sessStartTime->setMinute(unbcd(buf[24])); 
	sessStartTime->setSecond(unbcd(buf[23]));
	sessStartTime->setTenth((unsigned char) 0);

	sessStartDate->setTime(sessStartTime);
	w_session->setStartDate(sessStartDate);

	printf("OK Date %s\n",w_session->getStartDate()->toString().c_str());
	printf("OK Start Time %s\n",sessStartTime->toString().c_str());

	//  create a fileneame for later saving
	//  the extention is added on demand .hrm .dump or what ever 
	snprintf(w_session->id,31,"%d-%d-%d_%d_%d",
							w_session->getStartDate()->getYear(),
							w_session->getStartDate()->getMonth(),
							w_session->getStartDate()->getDay(),
							w_session->getStartDate()->getHour(),
							w_session->getStartDate()->getMinute());

	wTime *duration =new wTime();
	duration->setHour(buf[28] & 0x7f);
	duration->setMinute (buf[27] & 0x3f);
	duration->setSecond (buf[26] & 0x3f);
	duration->setTenth(((buf[27] & 0xc0) >> 4) | ((buf[26] & 0xc0) >> 6)); //tenth
	w_session->setDuration(duration);

	printf("?? thenth %f\n", (double)(((buf[27] & 0xc0) >> 4) | ( (buf[26] & 0xc0) >> 6))    );
	printf("OK w_session->duration=%f\n",w_session->getDuration()->toDouble());
	printf("OK w_session->duration=%s\n",w_session->getDuration()->toString().c_str());

	printf("?? duration at byte 99 ");
	test(&buf[99]);

	w_session->hr_avg=buf[29];
	printf("OK w_session->hr_avg=%d\n",w_session->hr_avg);
	w_session->hr_max=buf[30];
	printf("OK w_session->hr_max=%d\n",w_session->hr_max);
	w_session->hr_min=buf[31];
	printf("OK w_session->hr_min=%d\n",w_session->hr_min);

	//  user related data
	w_session->user_hr_max = buf[35];
	printf("OK w_session->user_hr_max=%d\n",w_session->user_hr_max);
	w_session->user_hr_rest = buf[36];
	printf("OK w_session->user_hr_rest=%d\n",w_session->user_hr_rest);
	w_session->user_vo2_max = buf[37];
	printf("OK w_session->user_vo2_max=%d\n",w_session->user_vo2_max);
	w_session->calories = toshort(&buf[32]);
	printf("OK w_session->calories=%d\n",w_session->calories);

	SHOW(34);
	SHOW(38);
	SHOW(39);
	SHOW(40);


	w_session->setNumberOfLaps(buf[46]);
	printf("OK w_session->number_of_laps=%d\n",w_session->getNumberOfLaps());
	printf("?? number_automated_laps=%d\n",buf[47]); //what is the difference?

	w_session->best_lap = buf[51];
	printf("?? w_session->best_lap=%d\n",w_session->best_lap);
	w_session->pace_max = toshort(&buf[52]);
	printf("?? w_session->pace_max=%d\n",w_session->pace_max);
	w_session->pace_avg = toshort(&buf[54]);;
	printf("?? w_session->pace_avg=%d\n",w_session->pace_avg);



/*
(buf[28] & 0x7f, //hour
buf[27] & 0x3f, // minutes
buf[26] & 0x3f, // seconds
((buf[27] & 0xc0) >> 4) | ((buf[26] & 0xc0) >> 6)
*/
	wTime *best_laptime = new wTime();

	//if (w_session->getNumberOfLaps() != 1) {
		best_laptime->setHour   (buf[49] & 0x7F);
		best_laptime->setMinute (buf[48] & 0x3F);
		best_laptime->setSecond (buf[47] & 0x3F);
		best_laptime->setTenth (((buf[48] & 0xc0) >> 4) | ((buf[47] & 0xc0) >> 6));
	//}

	//else {
		//set end time
		//best_laptime = duration;

	//}
	
	w_session->setBestLapTime(best_laptime);

	printf("?? w_session->best_laptime=%f\n",w_session->getBestLapTime()->toDouble());
	printf("?? w_session->best_laptime=%s\n",w_session->getBestLapTime()->toString().c_str());


	printf("?? 56 byte %X\t%d allways FF??\n",buf[56], buf[56]);


	SHOW(18);

	// figure out what data is recorded
	printf("?? 18 bit 7 UNKNOWN %d\n",(buf[18]&0x80) >> 7);
	printf("?? 18 bit 6 UNKNOWN %d\n",(buf[18]&0x40) >> 6);
	printf("?? 18 bit 5 UNKNOWN %d\n",(buf[18]&0x20) >> 5);
	printf("?? 18 bit 4 GPS on %d\n",(buf[18]&0x10) >> 4);
	printf("?? 18 bit 3 has Speed/pace data %d\n",(buf[18]&0x08) >> 3);
	printf("?? 18 bit 2 Cadence %d\n",(buf[18]&0x04) >> 2);
	printf("?? 18 bit 1 UNKNOWN %d\n",(buf[18]&0x02) >> 1);
	printf("?? 18 bit 0 Altitude %d\n",buf[18]&0x01);


	bool bike2 = ((buf[18]&0x20) && (buf[18]&0x10)) ? true:false;    //bit 5
	bool bike1 =  (buf[18]&0x20) ? true:false; 
	bool runspeed = (buf[18]&0x10) ? true:false;     //bit 4
	bool power = (buf[18]&0x08) ? true:false;      //bit 3
	bool cadence = (buf[18]&0x04) ? true:false;    //bit 2
	bool altitude = (buf[18]&0x01) ? true:false;   //bit 1

	// lets try this
	bool gps = ((buf[18]&0x10) && (buf[18]&0x08)) ? true:false;


	SHOW(20);

	SHOW(41);
	SHOW(42);
	SHOW(43);
	SHOW(44);
	SHOW(45);

	SHOW(47);
	SHOW(48);
	SHOW(49);
	SHOW(50);

	SHOW(56);
	SHOW(57);
	SHOW(58);
	SHOW(59);
	SHOW(60);
	printf("\n\n");


/* need to find this values
	 * bike 1- 3
	 * speed sensor
	 * cadence
	 * shoe 1- 3
	 * gps G3 on - off byte 18 -> 0x18
	 * altitude on - off
*/

	/* the rs800cx manual tells the impact of switching on or of various settings

	 *	RR-Data (messures heartbeat variations)
		 don t handle this setting 

	 *	Speed
	 *	Cadence
	 *	S3
	 *	GPS
 		if gps on lap_byte_size = 25

	 *	Altitude
		if ALT on byte 18 bit 0 is true
		if ALT lap_byte_size = 14


	 */


	// lap_byte_size
	// standard size is 7 (HR Only) but sometimes 6

	// speed could be 
	// runspeed foodpod S3
	// runspeed GPS
	// cs bike 

	int lap_byte_size = 7; // base length
	int sample_size = 1;   //HR only sounds correct ;-)

	if (altitude) {
		sample_size = 2; //FIXME
		lap_byte_size = 7; //need to be checked
	}
	if (gps){
		sample_size=12; 
		// HR = 1 byte
		// special purpose 3 bytes, satellite connections, pace
		// longitude = 4 byte
		// latitude = 4 byte
		lap_byte_size = 26; 
	}
	if (altitude && gps) {
		sample_size=14; // FIXME higher value!
		lap_byte_size =  34;
	}

	w_session->lap_byte_size = lap_byte_size;
	w_session->sample_size = sample_size;

	printf("?? w_session->sample_size = %d\n",w_session->sample_size);


	w_session->setHasAltitudeData(altitude);
	printf("?? w_session->has_altitude_data %d\n",w_session->getHasAltitudeData());

	w_session->setHasSpeedData((runspeed || gps) ? true:false );
	printf("?? w_session->has_speed_data %d\n",w_session->getHasSpeedData() );



	// FIXME get this working therefore cycling data is needed
	/*
	w_session->setHasHRandCCData ((runspeed || (bike1 || bike2)) ? true:false);

	w_session->setHasCadenceData (cadence);
	w_session->setHasPowerData (power);

	printf("?? w_session->has_cadence_data %d\n",w_session->getHasCadenceData());
	*/

	// looks like some similar value
	test(&buf[38]);
	printf("?? left time to save sessions? ");
	test(&buf[80]);
	// left time to save sessions?
	//SHOW(80);
	//SHOW(81);
	//SHOW(82);


	switch (buf[19]) {
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

	printf("OK w_session->recording_interval=%d\n",w_session->getRecordingInterval());


	// RS800CX protocoll size is at least 170 
	//calculating hr sample data length
	// e.g.:
	// 2046 samples
	// 175 first hr value
	// 2220 hr
	// 2227 end length there is only one lap stored


	//w_session->setNumberOfSamples((sesslen-(174+w_session->getNumberOfLaps()*lap_byte_size)-sample_size)/sample_size);
	w_session->setNumberOfSamples((int)floor(w_session->getDuration()->toDouble() / w_session->getRecordingInterval())-1);

	printf("?? w_session->number_of_samples looks correct %d\n",w_session->getNumberOfSamples());
	printf("?? proof of calculation: duration / record interval = %d\n", (int)floor(w_session->getDuration()->toDouble() / w_session->getRecordingInterval()));
	printf ("?? byte 97 number_of_samples seems to be something different %d\n",toshort(&buf[97]));


	SHOW(97);
	SHOW(98);


	SHOW(102);
	SHOW(103);
	SHOW(104);
	SHOW(105);
	SHOW(106);
	SHOW(107);
	SHOW(108);
	SHOW(109);
	SHOW(110);

	SHOW(111);
	SHOW(112);
	SHOW(113);
	SHOW(114);
	SHOW(115);

	SHOW(116);
	SHOW(117);
	SHOW(118);
	SHOW(119);
	SHOW(120);

	SHOW(121);
	SHOW(122);


	SHOW(165);
	//SHOW(170);
	SHOW(171);
	SHOW(172);
	SHOW(173);
	SHOW(174);
	SHOW(175);

	//XXX disabled for shorter output
	parse_samples(w_session, raw_sess);
	parse_sportzones (w_session, raw_sess);

	parse_laps(w_session, raw_sess);

/*
	int base= 422;
	test(&buf[base + 0]);
	test(&buf[base + 1]);
	test(&buf[base + 2]);
	test(&buf[base + 3]);
	test(&buf[base + 4]);
	test(&buf[base + 5]);
	test(&buf[base + 6]);
	test(&buf[base + 7]);
	test(&buf[base + 8]);
	test(&buf[base + 9]);
	test(&buf[base + 10]);
	printf("\n");
	//632 ALT @620
	// -34
	test(&buf[599]);
	// -34
	test(&buf[565]);
	// -34
	test(&buf[531]);
	// -33
	test(&buf[498]);
	// -34
	test(&buf[464]);
	// -34
	test(&buf[430]);
//514

test(&buf[47]);

174

430

*/

	return w_session;
}





RawSession* RS800CXparse::parseRawSession(std::list<Datanode>* nodelist){

	RawSession *raw_session = new RawSession;

	unsigned char *nodebuf;
	int buflen;
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
			//buflen -= RS800CX_FIRST_PACKET_CORRECTION_HEAD;
			//buflen -= RS800CX_FIRST_PACKET_CORRECTION_TAIL;
		}
		else {
			//buflen -= RS800CX_ALL_OTHER_PACKETS_CORRECTION_HEAD;
			//buflen -= RS800CX_ALL_OTHER_PACKETS_CORRECTION_TAIL;
		}
	}


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
			// cut off RS800CX_FIRST_PACKET_CORRECTION_HEAD bytes at the beginning,
			// RS800CX_FIRST_PACKET_CORRECTION_TAIL at the end
			nodebuflen -= RS800CX_FIRST_PACKET_CORRECTION_HEAD;
			nodebuflen -= RS800CX_FIRST_PACKET_CORRECTION_TAIL;
			memcpy(&rawbuf[0], &nodebuf[RS800CX_FIRST_PACKET_CORRECTION_HEAD], nodebuflen);
		}

		else {
			// cut off RS800CX_ALL_OTHER_PACKETS_CORRECTION_HEAD bytes at the beginning,
			// RS800CX_ALL_OTHER_PACKETS_CORRECTION_TAIL at the end
			nodebuflen -= RS800CX_ALL_OTHER_PACKETS_CORRECTION_HEAD;
			nodebuflen -= RS800CX_ALL_OTHER_PACKETS_CORRECTION_TAIL;
			memcpy(&rawbuf[rawbuf_filled], &nodebuf[RS800CX_ALL_OTHER_PACKETS_CORRECTION_HEAD], nodebuflen); 
		}

		printf("node %d\n",nodecount);
		printf("nodelen orignal / cut %d / %d\n",current->getBufLen(), nodebuflen);
		printf("rawbuffer filled %d\n\n",rawbuf_filled);

		// whats in the node buffer
		for (int j=0; j<=current->getBufLen(); j++)
			printf("%X ", nodebuf[j]);
		printf("\n\n");

		// whats copied
		for (int j=rawbuf_filled; j<=rawbuf_filled+nodebuflen; j++)
			printf("%X ",rawbuf[j]);
		printf("\n\n");

		rawbuf_filled += nodebuflen;
		nodecount++;
	}

	// result
	printf("\nfinal rawbuffer (len %i)\n", rawbuf_filled);
	
	for (int j=0; j<=rawbuf_filled; j++)
		printf("%X ",rawbuf[j]);
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



void RS800CXparse::test(unsigned char pos[]) {
    unsigned char hours = pos[2] & 0x7f;
    unsigned char minutes = pos[1] & 0x3f;
    unsigned char seconds = pos[0] & 0x3f;
    /* We can just fit the tents in the extra bits */
    unsigned char tenths = ((pos[1] & 0xc0) >> 4) | ((pos[0] & 0xc0) >> 6);
	printf("time %d:%d:%d.%d\n", hours, minutes, seconds, tenths);
}


// tempurary function for convinient displaying whats in the buffer
void RS800CXparse::show(int i, unsigned char *pos){
	std::cout << "?? " << std::dec << i << " byte\t" << std::hex << (int)pos[0] << "\t" << std::dec << (int)pos[0] << "\t";
	std::cout << std::bitset<8>(pos[0]) << std::endl;
}
