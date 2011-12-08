/***************************************************************************
 *            s625X_parse_session.c
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

#include "../../cli/polarhrm_config.h"
#include "../util_functions.h"

#include "S625X_comm.h"
#include "S625X_parse.h"

#include "../Watch/Session.h"
#include "../Watch/Sample.h"
#include "../Watch/RawSessionFile.h"
#include "../Watch/wTime.h"
#include "../Watch/wDate.h"




bool bike1;
bool bike2;
bool runspeed;
bool altitude;
bool cadence;
bool power;

int lap_byte_size;
int sample_size;



void S625Xparse::parse_samples(Session *w_session, RawSession *raw_sess){

	unsigned char *buf;
	buf = raw_sess->getRawBuffer();
	
	float speed_val;
	int alt, i;
	int j=0; // count printed samples  
	
	// set up a memory for storing the samples
	w_session->samples =(Sample**) new Sample[w_session->getNumberOfSamples()];

	// goto all samples [HRData] at hrm file and store them in the structure
	for (i = raw_sess->getRawBufferlen() - sample_size; i >= 130 + (w_session->getNumberOfLaps() * lap_byte_size); i = i - sample_size){
	
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

		printf("i%d\t%d",i+3,w_session->samples[j]->getHR());
	
		if (runspeed){

			// byte c [Upper 3 bits]: Speed (MSB)
			// byte c+1: Speed (LSB); speed (km/h) = Speed / 16.0
		
			speed_val = 0;
			
			if (altitude) speed_val = (buf[i+2]<<3)+buf[i+3];

			else speed_val = (buf[i+1]<<3)+buf[i+2];
			
			speed_val = (speed_val/16)*10;
			speed_val = round(speed_val);
			w_session->samples[j]->setSpeed((unsigned int) speed_val);

			printf("\t%d",w_session->samples[j]->getSpeed());
		}


		if (altitude) {

			alt=0;

			if (runspeed) alt = buf[i+1] + ((buf[i+2] & 0x1F)<<8)-512;
	
			w_session->samples[j]->setAlt(alt);   
			printf("\t%d",w_session->samples[j]->getAlt());
		}

		printf("\r\n");
		j++;
	} 
	printf("printed samples %d\n",j-1);

}




void S625Xparse::parse_laps(Session *w_session, RawSession *raw_sess ){

	unsigned char *buf;
	buf = raw_sess->getRawBuffer();

	int i;
	
	int hr_index;
	double prev_lap_endtime=0;
	unsigned int prev_index;


	printf("lap byte size, %d\n\n",lap_byte_size);
	w_session->laps = (Lap**) new Lap[w_session->getNumberOfLaps()-1];

	prev_index = raw_sess->getRawBufferlen();

	for (int j=0;j <= (w_session->getNumberOfLaps()-1); j++){

		w_session->laps[j]= new Lap;

		w_session->laps[j]->lap_no=j+1;	
		printf("\n ===== LAP %d ===== \n",j+1 );

		i=130+(lap_byte_size*j);

		w_session->laps[j]->lap_end_time = new wTime((buf[i+2]), (buf[i+1]&0x3F), (buf[i]&0x3F) , (unib((buf[i+1]&0xC0)|((buf[i]&0xC0)>>2))));
		printf("%d byte hours %s\n",i, w_session->laps[j]->lap_end_time->toString().c_str() );

		printf("w_session->lap[j]->lap_end_time=%f\n",w_session->laps[j]->lap_end_time->toDouble());


		hr_index = round ((w_session->laps[j]->lap_end_time->toDouble() - prev_lap_endtime) / w_session->getRecordingInterval());
		prev_lap_endtime = w_session->laps[j]->lap_end_time->toDouble();

		printf("search for minimum from %d to %d len %d\n" , prev_index-hr_index, prev_index, hr_index);

		/* need to search in samples for samllest value*/
		w_session->laps[j]->hr_min = S625Xparse::find_minimum (&buf[prev_index-hr_index-sample_size],hr_index, sample_size);
	
		/* shift to next */
		prev_index = prev_index-hr_index; 

		//Lap byte 3 is the lap HR
		//Lap byte 4 is the avg HR for the lap
		//Lap byte 5 is the max HR for the lap
	
		printf("start lap HR %d\n",buf[i+3]);
		
		w_session->laps[j]->hr_end = (unsigned int) buf[i+3];
		w_session->laps[j]->hr_avg = (unsigned int) buf[i+4];
		w_session->laps[j]->hr_max = (unsigned int) buf[i+5];

		printf("min HR = %d\n",w_session->laps[j]->hr_min);
		printf("avg HR = %d\n",w_session->laps[j]->hr_avg);
		printf("max HR = %d\n",w_session->laps[j]->hr_max);

		/*
		if (altitude){

			buf[i+6];
			buf[i+7];
			buf[i+8];
			buf[i+9];
			buf[i+10];

		}
		*/
			// runspeed || bike1 || bike2
		if (w_session->getHasSpeedData()) {
			// byte 6
			printf("distance %d\n",buf[i+6]);
			// byte 7
			printf("byte 7 %d\n",buf[i+7]);
			// byte 8
			printf("start HR %d\n",buf[i+8]);
			// byte 9
			printf("byte 9 %d\n",buf[i+9]);
	
			//high_nibble(c+3) * 4 + high_nibble(c+2) determine the integer portion
			//low_nibble(c+2)/16 is the fractional portion. 

			//speed
			i=136+(lap_byte_size*j);
			printf("speed %d\n",(unib(buf[i+3])*4+unib(buf[i+2]))*10+(lnib(buf[i+2])/16));

			w_session->laps[j]->speed_end = (unsigned int)(unib(buf[i+3])*4+unib(buf[i+2])*10+(lnib(buf[i+2])/16));
			printf("w_session->lap[j]->speed_end=%i\n",w_session->laps[j]->speed_end);
		
		}
	}

}


#if DO_NOT_COMPILE

int S625Xparse::parse_sportzones(SportZone **w_lzone, unsigned char sportzone_array[], int count){

	int i=0;
	int low_range;
	int high_range;
	
//	watch_limiterzone **w_lzone=NULL;


	
	while(sportzone_array[i]>0 && sportzone_array[i+1]>0 && i <= count){

		// create or extend memory to store 
		w_lzone = (SportZone**) realloc(w_lzone, sizeof(SportZone*)*(i+1));
		
		if (0==i) {

			low_range = sportzone_array[i+1];
			high_range = sportzone_array[i];

		}

		else {

			low_range = sportzone_array[i+1];
			high_range = sportzone_array[i]-1;

		}

		printf("sportzone from %d to %d\n",low_range,high_range);

		w_lzone[i] = (SportZone*) malloc(sizeof(SportZone));

		w_lzone[i]->limit_low=low_range;
		w_lzone[i]->limit_high=high_range;


		i++;

	}

	printf("number_of_zones %d\n",i);

	return i;

}

#endif


/*

Subtype 0x0b: "Get files" response: Length = variable:
-----------------------------------------------------------------------------
Payload byte	Meaning
=============================================================================
0		bit 7: "first packet" = 1
		bits 6-0: packets remaining after this one
1       File bytes (MSB)
2		File bytes (LSB)
3		? 0x22 (magic?)
4		? 0x02 (magic?)
5..n-1	File data (see README.file_format)


 

send command B 
length 116 bytes
B 80 12 8 2B 13 7 8F 0 0 0 0 0 0 0 0 0 1 40 15 22 11 5 32 0 0 0 0 1 1 0 1 0 0 10 0 50 A0 50 A0 50 A0 FF FF FB 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 32 0 0 0 0 0 0 0 0 2 0 17 0 0 0 0 0 0 0 0 0 0 0 0 0 0 80 0 80 0 80 0 0 0 0 0 0 0 0 0 0 64 0 


*/



Session* S625Xparse::parseSession(RawSession *raw_sess){

	unsigned char *buf;
	buf = raw_sess->getRawBuffer();
	int sesslen;
	sesslen = raw_sess->getRawBufferlen();
 
	
	lap_byte_size=6;
	sample_size=3;

	int i,chk_sesslen;

	Session *w_session = new Session();

	//0 	Bytes in file (LSB)
	//1 	Bytes in file (MSB)
	chk_sesslen=buf[0]+(buf[1]<<8);
	if (chk_sesslen != sesslen) {

		printf("session length missmatch\n");
		exit(1);
	
	}

 
	printf("sesslen %i\n",sesslen);


	/*	
	unsigned char rawdata[7];
	unsigned char encdata[7];
	// copy bytes to rawbuffer and encode 
	memcpy(rawdata, &buf[3], 7);
	watch_decode_string(encdata, rawdata);
	printf("%s\n",encdata);
	*/

	//	w_session->id[31];
    
	//	strcpy(w_session->name,"FILENAME");
	
	w_session->limit_type;
	
	//21	Laps in file (including autolaps in interval training) (MT)
	w_session->setNumberOfLaps(unbcd(buf[21]));
	printf("OK w_session->number_of_laps=%d\n",w_session->getNumberOfLaps());
	printf("OK number_automated_laps=%d\n",unbcd(buf[22])); //what is the difference?

	// not handeling AM/PM for now
	
	//13	bit 7 - AM/PM mode (yes if set, no if unset)
	//      bits 6-0: File date (day of month)

	wDate* sessStartDate = new wDate();
	sessStartDate->setYear((int)BASE_YEAR+unbcd(buf[14]));
	sessStartDate->setMonth((unsigned char)lnib(buf[15]));
	sessStartDate->setDay((unsigned char)(unbcd(buf[13])&0x3F));


	// not handeling AM/PM for now
	
	//12	bit 7: 1 => PM, 0 => AM
	//  	bits 6-0: File date (hours)

	wTime* sessStartTime = new wTime();
	sessStartTime->setHour(unbcd(buf[12])&0x3F);
	sessStartTime->setMinute(unbcd(buf[11]));
	sessStartTime->setSecond(unbcd(buf[10]));
	sessStartTime->setThenth((unsigned char) 0);

	sessStartDate->setTime(sessStartTime);

	w_session->setStartDate(sessStartDate);

	printf("OK Date %s\n",w_session->getStartDate()->toString().c_str());


	printf("OK Start Time %s\n",sessStartTime->toString().c_str());

	//  create a fileneame for later saving
	//  the extention is added on demand .hrm .dump or what ever 
	//  http://www.linuxquestions.org/questions/programming-9/convert-integer-to-string-atoi-32417/
	snprintf(w_session->id,31,"%d-%d-%d_%d_%d",w_session->getStartDate()->getYear(),
							w_session->getStartDate()->getMonth(),
							w_session->getStartDate()->getDay(),
							w_session->getStartDate()->getHour(),
							w_session->getStartDate()->getMinute());


//15	L: File date (month, jan = 1)
//	    U: Exercise duration (tenths of a second)
//16	Exercise duration (seconds) (BCD)
//17	Exercise duration (minutes) (BCD)
//18	Exercise Duration (hours) (BCD)
 
	w_session->setDuration(new wTime(unbcd(buf[18]),unbcd(buf[17]) ,unbcd(buf[16]),unib(buf[15])));
	printf("thenth %f\n",(double)buf[15]*0.1);
	printf("OK w_session->duration=%f\n",w_session->getDuration()->toDouble());
	printf("OK duration %s\n",w_session->getDuration()->toString().c_str());
	
//19	Avg HR
//20	Max HR
	w_session->hr_avg=buf[19];
	printf("OK w_session->hr_avg=%d\n",w_session->hr_avg);
	w_session->hr_max=buf[20];
	printf("OK w_session->hr_max=%d\n",w_session->hr_max);	
	w_session->hr_min;            //missing? where to get this


//  -> user related data
    w_session->user_hr_max=USER_HR_MAX;
    w_session->user_hr_rest=USER_HR_REST;
    w_session->user_vo2_max=USER_V02_MAX;


// is this calculated somehow?
   w_session->calories;

	
//70	Energy * 10 (BCD) (lower 2 digits)
//71	Energy * 10 (BCD) (next higher 2 digits)
//72	Energy * 10 (BCD) (highest 2 digits)
//
//        Energy = ((BCD(b70) + BCD(b71)*100 + BCD(b72)*10000)/10.0
//
//73	Total energy (BCD) (lower 2 digits)
//74	Total energy (BCD) (next higher 2 digits)
//75	Total energy (BCD) (highest 2 digits)
//
//        Total energy = (BCD(b73) + BCD(b74)*100 + BCD(b75)*10000

	printf("OK energy [kcal] = %f\n",((unbcd(buf[70]) + unbcd(buf[71])*100 + unbcd(buf[72])*10000)/10.0));

	printf("total energy ? = %d\n",unbcd(buf[73]) + unbcd(buf[74])*100 + unbcd(buf[75])*10000);

//2	Exercise number (0 to 5) [MM]
	printf("OK Exercise number (0 to 5) = %d\n",buf[2]);

//23	Exercise mode (0 = basic, 1 = interval) (MT)
	printf("OK Exercise mode 0 basic, 1 interval = %d\n",buf[23]);

//25	bit 1: Units (0 = metric, 1 = english)
	w_session->setSiUnit(buf[25]); // true = kg/cm/km 
	printf("OK w_session->si_units=%d (1= metric)\n",w_session->getSiUnit());


//85	Exercise distance (LSB)
//86	Exercise distance (MSB); exe dist = ((LSB + (MSB<<8)) / 10.0) km
	w_session->distance= ((buf[85]+ (buf[86]<<8)) );///10.0
	printf("w_session->distance = %d\n",w_session->distance);

//26	Mode [HR is always recorded]
//		bit 7: 0
//		bit 6: 0
//		bit 5: Bike 2 (speed)
//		bit 4: Bike 1 (speed)
//		bit 3: Power
//		bit 2: Cadence
//		bit 1: Altitude
//		bit 0: UNKNOWN - may have to do with metric/English units
	
	printf("26 bit 7 %d\n",(buf[26]&0x80));
	printf("26 bit 6 %d\n",(buf[26]&0x40));
	printf("26 bit 5 speed2 %d\n",(buf[26]&0x20));
	printf("26 bit 4 speed1 %d\n",(buf[26]&0x10));
	printf("26 bit 3 Power %d\n",(buf[26]&0x08));
	printf("26 bit 2 Cadence %d\n",(buf[26]&0x04));
	printf("26 bit 1 Altitude %d\n",(buf[26]&0x02));
	printf("26 bit 0 UNKNOWN %d\n",buf[26]&0x01);

	
	bike2 = ((buf[26]&0x20) && (buf[26]&0x10)) ? true:false;    //bit 5
	bike1 =  (buf[26]&0x20) ? true:false; 
	runspeed = (buf[26]&0x10) ? true:false;     //bit 4
	power = (buf[26]&0x08) ? true:false;      //bit 3
	cadence = (buf[26]&0x04) ? true:false;    //bit 2
	altitude = (buf[26]&0x02) ? true:false;   //bit 1

	if (runspeed){
	 
		sample_size=3;
		lap_byte_size =lap_byte_size+4;

		// during testing I found that relation
		if (cadence && power && altitude) {
			cadence = false;
			power = false;
			altitude = false;
		}

		
		if(altitude){

			sample_size = sample_size +1;
			lap_byte_size =lap_byte_size+5;
		}
	}

//bikes
	else if (bike1 || bike2){

		lap_byte_size =lap_byte_size + 4;

		//bike 1 ?
		if(bike1)	printf("bike 1\n");

		//bike 2 ?
		if (bike2) printf("bike 2\n");
	  
		if(altitude){

			lap_byte_size =lap_byte_size + 5;

		}

		if(cadence){

			lap_byte_size =lap_byte_size + 1;  
		}
	 
		if(power){
		
			lap_byte_size =lap_byte_size + 4;
		}
	}

	else if (altitude){

		printf("HR data + altitude \n");
		lap_byte_size=11;
		sample_size=3;

	}

	else {

		printf("just HR data\n");
		lap_byte_size=6;
		sample_size=1;
		}


	w_session->setHasSpeedData((runspeed || (bike1 || bike2)) ? true:false );
	w_session->setHasHRandCCData ((runspeed || (bike1 || bike2)) ? true:false);

	w_session->setHasAltitudeData(altitude);
	w_session->setHasCadenceData (cadence);
	w_session->setHasPowerData (power);

	printf("w_session->has_speed_data %d\n",w_session->getHasSpeedData() );
	printf("w_session->has_cadence_data %d\n",w_session->getHasCadenceData());
	printf("w_session->has_altitude_data %d\n",w_session->getHasAltitudeData());



	w_session->pace_max;
	w_session->pace_avg;

//66	[U]: Best lap tenths of a second (BCD)
//	L - UNKNOWN
//67	Best lap seconds (BCD)
//68	Best lap minutes (BCD)
//69	Best lap hours (BCD)
	w_session->best_lap;
	printf("w_session->best_lap=\n");


	wTime *best_laptime = new wTime();
	best_laptime->setHour (unbcd(buf[69]));
	best_laptime->setMinute (unbcd(buf[68]));
	best_laptime->setSecond (unbcd(buf[67]));
	best_laptime->setThenth (unib(buf[66]));

	w_session->setBestLapTime(best_laptime);

	printf("w_session->best_laptime=%f\n",w_session->getBestLapTime()->toDouble());
	printf("best_laptime %s\n",w_session->getBestLapTime()->toString().c_str());


	printf("Byte 27 U: UNKNOWN %X\n",unib(buf[27]));
//27	L: Recording interval (0 = 5s, 1 = 15s, 2 = 60s)
		switch (lnib(buf[27])) {
		case 0:
			w_session->setRecordingInterval(5);
			break;
		case 1:
			w_session->setRecordingInterval(15);
			break;
		case 2:
			w_session->setRecordingInterval(60);
			break;
		}
	printf("OK w_session->recording_interval=%d\n",w_session->getRecordingInterval());


	//calculating hr sample data length
	w_session->setNumberOfSamples((sesslen-(130+w_session->getNumberOfLaps()*lap_byte_size)-sample_size)/sample_size);

	printf("w_session->number_of_samples %d\n",w_session->getNumberOfSamples());


	printf("35 byte UNKNOWN = %d\n",buf[35]);
	printf("36 byte UNKNOWN - 0 or 1 = %d\n",buf[36]);
	printf("37 byte UNKNOWN = %d\n",buf[37]);


	printf("65 UNKNOWN number of laps once again?= %d\n",buf[65]);

	
//28	UNKNOWN - Sometimes 0, sometimes 16, 1 in all 625X files (MT)
	printf("28 UNKNOWN = %d\n",buf[28]);


	//109	Limit summary low
	printf("OK 109 byte Limit summary low %d\n",buf[109]);

	//110	Limit summary high
	printf("OK 110 byte Limit summary high %d\n",buf[110]);

	printf("111 byte UNKNOWN ? %d\n",buf[111]);
	printf("112 byte UNKNOWN ? %d\n",buf[112]);
	
	printf("113 byte UNKNOWN ? %d\n",buf[113]);
	printf("114 byte UNKNOWN ? %d\n",buf[114]);
	
	printf("115 byte UNKNOWN ? %d\n",buf[115]);
	printf("116 byte UNKNOWN ? %d\n",buf[116]);
	
	printf("117 byte UNKNOWN ? %d\n",buf[117]);
	printf("118 byte UNKNOWN ? %d\n",buf[118]);
	
	printf("119 byte UNKNOWN ? %d\n",buf[119]);
	printf("120 byte UNKNOWN ? %d\n",buf[120]);
	
	printf("121 byte UNKNOWN ? %d\n",buf[121]);
	printf("122 byte UNKNOWN ? %d\n",buf[122]);
	
	printf("123 byte UNKNOWN ? %d\n",buf[123]);
	printf("124 byte UNKNOWN ? %d\n",buf[124]);
	
	printf("125 byte UNKNOWN ? %d\n",buf[125]);
	printf("126 byte UNKNOWN ? %d\n",buf[126]);

	printf("127 byte UNKNOWN ? %d\n",buf[127]);
	printf("128 byte UNKNOWN ? %d\n",buf[128]);

	// 129  HR MAX
	printf("OK 129 byte USER HR MAX %d\n",buf[129]);

	w_session->hr_max = (unsigned int) buf[129];
	w_session->user_hr_max = (unsigned int) buf[129];
	//29	Limits 1 (Low)
	//30	Limits 1 (High)
	//31	Limits 2 (Low)
	//32	Limits 2 (High)
	//33	Limits 3 (Low)
	//34	Limits 3 (High)

	//38	Below Zone 1, sec (BCD)
	//39	Below Zone 1, min (BCD)
	//40	Below Zone 1, hour (BCD)
	//41	Within Zone 1, sec (BCD)
	//42	Within Zone 1, min (BCD)
	//43	Within Zone 1, hour (BCD)
	//44	Above Zone 1, sec (BCD)
	//45	Above Zone 1, min (BCD)
	//46	Above Zone 1, hour (BCD)

	w_session->limiterzone[0].limit_low=buf[29];
	w_session->limiterzone[0].limit_high=buf[30];
	w_session->limiterzone[0].sport_zone;
	w_session->limiterzone[0].below_zone = wTime(unbcd(buf[40]), unbcd(buf[39]), unbcd(buf[38]),0);
	w_session->limiterzone[0].on_zone = wTime(unbcd(buf[43]),unbcd(buf[42]),unbcd(buf[41]),0);
	w_session->limiterzone[0].above_zone = wTime(unbcd(buf[46]),unbcd(buf[45]),unbcd(buf[44]),0);

	printf("OK w_session->limiterzone[0].limit_low=%d\n",w_session->limiterzone[0].limit_low);
	printf("OK w_session->limiterzone[0].limit_high=%d\n",w_session->limiterzone[0].limit_high);
	printf("OK w_session->limiterzone[0].sport_zone WHAT IS THIS?\n");
	printf("OK w_session->limiterzone[0].below_zone_seconds=%f\n",w_session->limiterzone[0].below_zone.toDouble());
	printf("OK w_session->limiterzone[0].on_zone_seconds=%f\n",w_session->limiterzone[0].on_zone.toDouble());
	printf("OK w_session->limiterzone[0].above_zone_seconds=%f\n",w_session->limiterzone[0].above_zone.toDouble());

	w_session->limiterzone[1].limit_low=buf[31];
	w_session->limiterzone[1].limit_high=buf[32];
	w_session->limiterzone[1].sport_zone;
	w_session->limiterzone[1].below_zone = wTime(unbcd(buf[49]),unbcd(buf[48]),unbcd(buf[47]),0);
	w_session->limiterzone[1].on_zone = wTime(unbcd(buf[52]),unbcd(buf[51]),unbcd(buf[50]),0);
	w_session->limiterzone[1].above_zone = wTime(unbcd(buf[55]),unbcd(buf[54]),unbcd(buf[53]),0);

	printf("OK w_session->limiterzone[1].limit_low=%d\n",w_session->limiterzone[1].limit_low);
	printf("OK w_session->limiterzone[1].limit_high=%d\n",w_session->limiterzone[1].limit_high);
	printf("OK w_session->limiterzone[1].sport_zone WHAT IS THIS?\n");
	printf("OK w_session->limiterzone[1].below_zone_seconds=%f\n",w_session->limiterzone[1].below_zone.toDouble());
	printf("OK w_session->limiterzone[1].on_zone_seconds=%f\n",w_session->limiterzone[1].on_zone.toDouble());
	printf("OK w_session->limiterzone[1].above_zone_seconds=%f\n",w_session->limiterzone[1].above_zone.toDouble());

	w_session->limiterzone[2].limit_low=buf[33];
	w_session->limiterzone[2].limit_high=buf[34];
	w_session->limiterzone[2].sport_zone;
	w_session->limiterzone[2].below_zone= wTime(unbcd(buf[58]),unbcd(buf[57]),unbcd(buf[56]),0);
	w_session->limiterzone[2].on_zone = wTime(unbcd(buf[61]),unbcd(buf[60]),unbcd(buf[59]),0);
	w_session->limiterzone[2].above_zone = wTime(unbcd(buf[64]),unbcd(buf[63]),unbcd(buf[62]),0);

	printf("OK w_session->limiterzone[2].limit_low=%d\n",w_session->limiterzone[2].limit_low);
	printf("OK w_session->limiterzone[2].limit_high=%d\n",w_session->limiterzone[2].limit_high);
	printf("OK w_session->limiterzone[2].sport_zone WHAT IS THIS?\n");
	printf("OK w_session->limiterzone[2].below_zone_seconds=%f\n",w_session->limiterzone[2].below_zone.toDouble());
	printf("OK w_session->limiterzone[2].on_zone_seconds=%f\n",w_session->limiterzone[2].on_zone.toDouble());
	printf("OK w_session->limiterzone[2].above_zone_seconds=%f\n",w_session->limiterzone[2].above_zone.toDouble());


	parse_samples(w_session, raw_sess);

	parse_laps(w_session, raw_sess);

	return w_session;
}







 RawSessions* S625Xparse::parseRawSessions(Overview* w_overview,std::list<Datanode>* nodelist){

	RawSessions *raw_sess = new RawSessions;

	// buffer for all sessions without any protocol header
	unsigned char *tempbuf;
	tempbuf = new unsigned char[w_overview->getUsedBytes()-1]; // starting at index 0 correcting this by -1

	int tbuflen=0, buf_filled=0;
	int nodecount=0;

	unsigned char* nodebuf;


	//clear buffer - was importend to check that everything is copied correct ;-)
	for (int j=0; j<=w_overview->getUsedBytes();j++)
		tempbuf[j]=0xFF;


	list<Datanode>::iterator current;

		//cut off protocol head and tailer and fill the raw buffer
		for ( current=nodelist->begin() ; current != nodelist->end(); current++ ) {

			nodebuf = current->getBuffer();

			if (0 == nodecount) {
				//do this only at the first node because this are data
				//responsed by initial query!!!
				//cut off 6 bytes at the beginning, 1 at the end
				tbuflen=current->getBufLen()-7;
				memcpy(&tempbuf[0], &nodebuf[7], tbuflen);
			}

			else {
				//cut off 3 bytes  2 at the beginning, 1 at the end
				tbuflen=current->getBufLen()-3;
				memcpy(&tempbuf[buf_filled], &nodebuf[3], tbuflen); 
			}

			printf("node %d\n",nodecount);
			printf("nodelen orignal / cut %d / %d\n",current->getBufLen(), tbuflen);
			printf("buff filled %d\n\n",buf_filled);

			// whats in the node buffer
			for (int j=0; j<=current->getBufLen(); j++)
				printf("%X ", nodebuf[j]);
			printf("\n\n");

			// whats copied
			for (int j=buf_filled; j<=buf_filled+tbuflen; j++)
				printf("%X ",tempbuf[j]);
			printf("\n\n");

			buf_filled=buf_filled+tbuflen;
			nodecount++;
		}

			// result
			printf("\nfull buffer \n");
			for (int j=0; j<=buf_filled; j++)
				printf("%X ",tempbuf[j]);
			printf("\n\n");


	if(buf_filled != w_overview->getUsedBytes()) {

		printf("missing BYTES ..\n");
		printf("buf_filled %d\n",buf_filled);
		printf("w_overview.used_bytes %d\n",w_overview->getUsedBytes());
		exit (-1);
	}

	raw_sess->setRawBuffer(tempbuf, buf_filled);
	raw_sess->setSessionNumber(w_overview->getSessionNumber()); 
	raw_sess->setUsedBytes( w_overview->getUsedBytes());
	
	return raw_sess;
}



// 
//  find the minimum in a not sorted array 
//  this is proberbly a ppp software function
//
//  http://www.computing.net/answers/programming/max-amp-min-values-in-arrays-c/8620.html
//
unsigned char S625Xparse::find_minimum(unsigned char buf[], int len, int stepsize) {

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



