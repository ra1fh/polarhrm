#include <cmath>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

#include "../../cli/polarhrm_config.h"
#include "Session.h"


#include "HrmFile.h"


/* Save the data stored in the session structure into a HRM file */
void HrmFile::save(Session *session) {

    FILE *fd; 


	fd = fopen(filename.c_str(), "w");
	printf("path %s\n\n",filename.c_str());
	if (0 == fd) {
		printf("error opening file for write\n");
	}

    /* [Params] 
	this is allmost quite static, but the idea is to wirte this data dynanmic therfore i set up a sturct
	holding that data
	however this has no priority for now */
	fprintf(fd,
        "[Params]\r\n"
        "Version=%d\r\n"
        "Monitor=%d\r\n",
         this->getVersion(), //e.g. 106
         this->getMonitor()  //e.g. 22
    );


    fprintf(fd, "SMode=%d%d%d%d%d%d%d%d%d\r\n",
        session->getHasSpeedData(), /* speed */
        session->getHasCadenceData(), /* cadence */
        session->getHasAltitudeData(), /* altitude */
        session->getHasPowerData(), /* power */
        session->getHasPowerLRBData() ? true : false, /* power left right balance */
        session->getHasPowerData() ? true : false, /* power pedalling index */
        session->getHasHRandCCData()  ? true : false, /* HR/CC data */
        session->getSiUnit() ? false : true, /* SI / US units 0 = Euro (km, km/h, m, °C) 1 = US (miles, mph, ft, °F) */
        session->getHasAirPressureData() ? true : false  /* air pressure */
    );
    fprintf(fd, "Date=%4d%02d%02d\r\n",
        session->getStartDate()->getYear(),
        session->getStartDate()->getMonth(),
        session->getStartDate()->getDay()
    );
    fprintf(fd, "StartTime=%2d:%02d:%02d.0\r\n",
        session->getStartDate()->getHour(),
        session->getStartDate()->getMinute(),
        session->getStartDate()->getSecond()
    );
    
    fprintf(fd, "Length=%s\r\n", session->getDuration()->toString().c_str());
    fprintf(fd, "Interval=%d\r\n",
        session->getRecordingInterval()
    );
    fprintf(fd, "Upper1=%d\r\n",
        session->limiterzone[0].limit_high
    );
    fprintf(fd, "Lower1=%d\r\n",
        session->limiterzone[0].limit_low
    );
    fprintf(fd, "Upper2=%d\r\n",
        session->limiterzone[1].limit_high
    );
    fprintf(fd, "Lower2=%d\r\n",
        session->limiterzone[1].limit_low
    );
    fprintf(fd, "Upper3=%d\r\n",  // Upper threshold / Upper limit 3 (bpm) this is what the doc says 
        session->limiterzone[2].limit_high  // but is there a switch or how is this handled ??
    );
    fprintf(fd, "Lower3=%d\r\n",
        session->limiterzone[2].limit_low
    );
//    timetostr(time_str, 0); /* TODO */
    fprintf(fd, "Timer1=%s\r\n", "");
    /* ...but spec says these are just mm:ss */
//    timetostr(time_str, 0); 
    fprintf(fd, "Timer2=%s\r\n", "");
//    timetostr(time_str, 0); /* TODO */
    fprintf(fd, "Timer3=%s\r\n", "");

    fprintf(fd, "ActiveLimit=%d\r\n", /* TODO */
        session->activelimit ? true : false /* Limits in use in “File Summary”: 0 = Limits 1 and 2, 1 = Treshold limits */
    );
    fprintf(fd, "MaxHR=%d\r\n",
        /* Polar gets these from somewhere else? */
        session->user_hr_max
    );
    fprintf(fd, "RestHR=%d\r\n",
        session->user_hr_rest
    );
    fprintf(fd, "StartDelay=0\r\n");
    fprintf(fd, "VO2max=%d\r\n",
        session->user_vo2_max
    );
    fprintf(fd, "Weight=%d\r\n",
        USER_WEIGHT 
    );

    /* [Note] */
    fprintf(fd, "\r\n[Note]\r\n");
    /* Max 250 characters. Just leave this empty for now. */
    
    /* [IntTimes] */
    fprintf(fd, "\r\n[IntTimes]\r\n");
    for(int i = 0; i < session->getNumberOfLaps(); i++) {
        Lap *lap = session->laps[i];
        fprintf(fd, "%s\t%d\t%d\t%d\t%d\r\n", /* Row 1 */
            lap->lap_end_time->toString().c_str(), /* lap time */
            lap->hr_end,
            lap->hr_min,
            lap->hr_avg,
            lap->hr_max
        );
        fprintf(fd, "%d\t%d\t%d\t%d\t%d\t%d\r\n", /* Row 2 */
            0, /* flags */
            0, /* rec. time */
            0, /* rec. hr */
           // (int)round(lap->pace_end / 25.6), /* momentary speed */
             lap->speed_end,    
            0, /* momentary cadence */
            0 /* momentary altitude */
        );
        fprintf(fd, "%d\t%d\t%d\t%d\t%d\r\n", /* Row 3 */
            0, 0, 0, 0, 0 /* I think these will be empty */
        );
        fprintf(fd, "%d\t%d\t%d\t%d\t%d\t%d\r\n", /* Row 4 */
            0, /* lap type = normal lap. */
            (int) round(lap->distance / 25.6), /* lap distance */
            0, /* power */
            0, /* temperature */
            0, /* phaselap */
            0
        );
        fprintf(fd, "%d\t%d\t%d\t%d\t%d\t%d\r\n", /* Row 5 - undocumented in spec? */
            0, 0, 0, 0, 0, 0
        );
    }

    /* [IntNotes] */
    fprintf(fd, "\r\n[IntNotes]\r\n");
    /* Leave empty. */
    
    /* [ExtraData] */
    fprintf(fd, "\r\n[ExtraData]\r\n");
    /* Leave empty. */
    
    /* [Summary-123] */
    fprintf(fd, "\r\n[Summary-123]\r\n");
    for(int i = 0; i < 3; i++) {
        /* This should use the limiter zones but for now let's just use the totals. */
        Limit *zone = &session->limiterzone[i];
        fprintf(fd, "%d\t%d\t%d\t%d\t%d\t%d\r\n",
            (int)floor(session->getDuration()->toDouble()), /* total time for selection */
            0, /* time with HR above max */
            0, /* time with HR between UL and max */
            (int) floor(session->getDuration()->toDouble()), /* time with HR between limits */
            0, /* time with HR between LL and rest */
            0  /* time with HR below rest */
        );
        fprintf(fd, "%d\t%d\t%d\t%d\r\n",
            session->user_hr_max, /* max HR */
            zone->limit_high, /* UL */
            zone->limit_low, /* LL */
            session->user_hr_rest  /* rest HR */
        );
    }
    fprintf(fd, "%d\t%d\r\n",
        0,
        (int) session->getNumberOfSamples()
    );
    /* [Summary-TH] */
    /* For now only one summary is supported */
    fprintf(fd, "\r\n[Summary-TH]\r\n");
    fprintf(fd, "%d\t%d\t%d\t%d\t%d\t%d\r\n",
        (int) floor(session->getDuration()->toDouble()),
        0,
        0,
        (int) floor(session->getDuration()->toDouble()),
        0,
        0
    );
    fprintf(fd, "%d\t%d\t%d\t%d\r\n",
        session->user_hr_max,
        0,
        0,
        session->user_hr_rest
    );
    fprintf(fd, "%d\t%d\t\r\n",
        0,
        session->getNumberOfSamples()
    );

    /* [HRZones] */
    fprintf(fd, "\r\n[HRZones]\r\n");

	if (this->getMonitor() == DEVICE_S625X) { 
		for(int i = 0; i <= USER_NUMBER_OF_SPORTZONES; i++) {
			fprintf(fd, "%d\r\n",
			user_sportzones[i]);
		}
	}
	else if(this->getMonitor() == DEVICE_RS800CX){
		for(int i = 0; i < SportZone::number_of_sportzones; i++) {
			fprintf(fd, "%d\r\n",
			(int) round(session->sportzones[i]->getHighPercent() * session->user_hr_max/ 100.0));
		}
		//fprintf(fd, "%d\r\n",
		//(int) round(session->sportzone[4].low_percent * session->user_hr_max / 100.0));
		for(int i = 0; i < SportZone::number_of_sportzones; i++) {
			fprintf(fd, "0\r\n");
		}
	}


	
    /* [SwapTimes] */
    fprintf(fd, "\r\n[SwapTimes]\r\n");
    /* Leave empty.  */
    
    /* [Trip] */
    fprintf(fd, "\r\n[Trip]\r\n");
    fprintf(fd, "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n",
        session->distance,
        0, /* ascent */
        (int) floor(session->getDuration()->toDouble()),
        0, /* avg altitude */
        0, /* max altitude */
        session->pace_avg / 2,
        session->pace_max / 2,
        0 /* end odometer value */
    ); 
    /* [HRData] */
    fprintf(fd, "\r\n[HRData]\r\n");

	// print the first sample twice
	if (this->getMonitor () == DEVICE_RS800CX) {
		fprintf(fd, "%d", session->samples[0]->getHR());

		if (session->getHasSpeedData()) {
			/* Need to output in multiples of 1/10 km/h */
			fprintf(fd, "\t%d", (int) session->samples[0]->getSpeed()); 
		}
		fprintf(fd, "\r\n");
	}

	for(int i = 0; i <= session->getNumberOfSamples(); i++) {
		
		fprintf(fd, "%d", session->samples[i]->getHR());
		
 		if (session->getHasSpeedData()) {
			// session->sample[i]->speed / 25.6;
			fprintf(fd, "\t%d",(int) session->samples[i]->getSpeed() );
		}

		if (session->getHasAltitudeData()){

			fprintf(fd, "\t%d",(int) session->samples[i]->getAlt() );

		}

		fprintf(fd, "\r\n");
    }
}

void HrmFile::setPath(string path) {
	filename = path;
}



HrmFile::HrmFile(unsigned char monitor, unsigned char version){
	this->monitor = monitor;
	this->version = version;

}



void HrmFile::setMonitor(unsigned char monitor){
	this->monitor = monitor;

}


unsigned char HrmFile::getMonitor(void){
	return monitor;

}



void HrmFile::setVersion(unsigned char version){
	this->version = version;

}


unsigned char HrmFile::getVersion(void){
	return version;

}