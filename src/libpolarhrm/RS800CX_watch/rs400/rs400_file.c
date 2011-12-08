/************************************************************************
This file is part of RS400 Tools

RS400 Tools is Copyright (C) 2007 Matti Juvonen <mpj@iki.fi>
Portions Copyright (C) 2006 Jani Hursti <jani.hursti@hut.fi>,
Portions Copyright (C) 2001-2003 Tom Oliveira e Silva

RS400 Tools is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

RS400 Tools is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#include "rs400_struct.h"
#include "rs400_util.h"

void RS400_dumpraw(char filename[], unsigned char data[], int size) {
    int dump = creat(filename, 0644);
    write(dump, data, size);
    close(dump);
}

/* Save the data stored in the session structure into a HRM file */
void RS400_save_hrm(char filename[], rs400_session *session) {
    int i;
    char time_str[12];
    
    FILE *fd = fopen(filename, "w");
    
    /* [Params] */
    fprintf(fd,
        "[Params]\r\n"
        "Version=107\r\n"
        "Monitor=36\r\n" /* RS400 */
    );
    fprintf(fd, "SMode=%d%d%d%d%d%d%d%d%d\r\n",
        session->has_pace_data ? 1 : 0, /* speed */
        0, /* cadence */
        0, /* altitude */
        0, /* power */
        0, /* power left right balance */
        0, /* power pedalling index */
        1, /* HR/CC data */
        session->si_units ? 0 : 1, /* SI / US units */
        0  /* air pressure */
    );
    fprintf(fd, "Date=%4d%02d%02d\r\n",
        session->year,
        session->month,
        session->day
    );
    fprintf(fd, "StartTime=%2d:%2d:%2d.0\r\n",
        session->start_hour,
        session->start_minute,
        session->start_second
    );
    timetostr(time_str, session->duration);
    fprintf(fd, "Length=%s\r\n", time_str);
    fprintf(fd, "Interval=%d\r\n",
        session->recording_interval
    );
    fprintf(fd, "Upper1=%d\r\n",
        0 /* TODO */
    );
    fprintf(fd, "Lower1=%d\r\n",
        0
    );
    fprintf(fd, "Upper2=%d\r\n",
        0
    );
    fprintf(fd, "Lower2=%d\r\n",
        0
    );
    fprintf(fd, "Upper3=%d\r\n",
        0
    );
    fprintf(fd, "Lower3=%d\r\n",
        0
    );
    timetostr(time_str, 0); /* TODO */
    fprintf(fd, "Timer1=%s\r\n", time_str);
    /* ...but spec says these are just mm:ss */
    timetostr(time_str, 0); 
    fprintf(fd, "Timer2=%s\r\n", time_str);
    timetostr(time_str, 0); /* TODO */
    fprintf(fd, "Timer3=%s\r\n", time_str);
    fprintf(fd, "ActiveLimit=%d\r\n",
        0 /* TODO */
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
        0 /* Polar software leaves this at 0 */
    );

    /* [Note] */
    fprintf(fd, "\r\n[Note]\r\n");
    /* Max 250 characters. Just leave this empty for now. */
    
    /* [IntTimes] */
    fprintf(fd, "\r\n[IntTimes]\r\n");
    for(i = 0; i < session->number_of_laps; i++) {
        rs400_lap *lap = session->lap[i];
        timetostr(time_str, lap->lap_end_time);
        fprintf(fd, "%s\t%d\t%d\t%d\t%d\r\n", /* Row 1 */
            time_str, /* lap time */
            lap->hr_end,
            lap->hr_min,
            lap->hr_avg,
            lap->hr_max
        );
        fprintf(fd, "%d\t%d\t%d\t%d\t%d\t%d\r\n", /* Row 2 */
            0, /* flags */
            0, /* rec. time */
            0, /* rec. hr */
            (int) round(lap->pace_end / 25.6), /* momentary speed */
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
    for(i = 0; i < 3; i++) {
        /* This should use the limiter zones but for now let's just use the totals. */
        rs400_limiterzone *zone = &session->limiterzone[i];
        fprintf(fd, "%d\t%d\t%d\t%d\t%d\t%d\r\n",
            (int) round(session->duration), /* total time for selection */
            0, /* time with HR above max */
            0, /* time with HR between UL and max */
            (int) round(session->duration), /* time with HR between limits */
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
        (int) session->number_of_samples
    );
    /* [Summary-TH] */
    /* For now only one summary is supported */
    fprintf(fd, "\r\n[Summary-TH]\r\n");
    fprintf(fd, "%d\t%d\t%d\t%d\t%d\t%d\r\n",
        (int) round(session->duration),
        0,
        0,
        (int) round(session->duration),
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
        session->number_of_samples
    );

    /* [HRZones] */
    fprintf(fd, "\r\n[HRZones]\r\n");
    for(i = 0; i < 5; i++) {
        fprintf(fd, "%d\r\n",
            (int) round(session->sportzone[i].high_percent * session->user_hr_max / 100.0));
    }
    fprintf(fd, "%d\r\n",
        (int) round(session->sportzone[4].low_percent * session->user_hr_max / 100.0));
    for(i = 0; i < 5; i++) {
        fprintf(fd, "0\r\n");
    }
    /* [SwapTimes] */
    fprintf(fd, "\r\n[SwapTimes]\r\n");
    /* Leave empty.  */
    
    /* [Trip] */
    fprintf(fd, "\r\n[Trip]\r\n");
    fprintf(fd, "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n",
        (int) round(session->distance / 100.0),
        0, /* ascent */
        (int) round(session->duration),
        0, /* avg altitude */
        0, /* max altitude */
        session->pace_avg / 2,
        session->pace_max / 2,
        0 /* end odometer value */
    );
    /* [HRData] */
    fprintf(fd, "\r\n[HRData]\r\n");
    /* For some reason the first sample is duplicated */    
    fprintf(fd, "%d", session->sample[0]->hr);
    if (session->has_pace_data) {
        /* Need to output in multiples of 1/10 km/h */
        fprintf(fd, "\t%d", (int) round(session->sample[0]->speed / 25.6)); 
    }
    fprintf(fd, "\r\n");
    for(int i = 0; i < session->number_of_samples; i++) {
        fprintf(fd, "%d", session->sample[i]->hr);
        if (session->has_pace_data) {
            fprintf(fd, "\t%d", (int) round(session->sample[i]->speed / 25.6));
        }
        fprintf(fd, "\r\n");
    }
}
