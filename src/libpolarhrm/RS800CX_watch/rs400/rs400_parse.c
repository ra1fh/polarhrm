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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "rs400_comm.h"
#include "rs400_util.h"
#include "rs400_struct.h"
#include "rs400_file.h"

void RS400_parse_overview(unsigned char data[], rs400_data * rs400) {
    rs400->number_of_sessions = data[4];
    printf("Found %d sessions\n", rs400->number_of_sessions);
}

/* Parses user settings (but not name) */
void RS400_parse_user(unsigned char data[], rs400_user * user) {
    user->weight = round(toshort(&data[6]) * 0.045359237); /* TODO: This is true if using SI units */
    user->height = data[8]; /* TODO: ditto */
    user->birth_day = data[9];
    user->birth_month = data[10];
    user->birth_year = 1900 + data[11];
    user->hr_max = data[12];
    user->vo2_max = data[13];
    user->hr_rest = data[14];
}

/* Find minimum HRs for all laps. */
void find_lap_hr_min(rs400_session * session) {
    int i = 0;
    for (int lap = 0; lap < session->number_of_laps; lap++) {
        session->lap[lap]->hr_min = 255; /* Set to maximum */
        for (; (i * session->recording_interval < session->lap[lap]->lap_end_time) && (i < session->number_of_samples); i++) {
            if (session->sample[i]->hr < session->lap[lap]->hr_min) {
                session->lap[lap]->hr_min = session->sample[i]->hr;
            }
        }
    }
}

/* Work out the total distance for each lap. */
void find_lap_distances(rs400_session * session) {
    int i = 0;
    for (int lap = 0; lap < session->number_of_laps; lap++) {
        int distance = 0;
        for (; (i * session->recording_interval < session->lap[lap]->lap_end_time) && (i < session->number_of_samples); i++) {
            distance += session->sample[i]->speed * session->recording_interval;
        }
        session->lap[lap]->distance = distance / 36.0;
    }
}

/* Work out the pace statistics. */
void find_lap_paces(rs400_session * session) {
    int i = 0;
    for (int lap = 0; lap < session->number_of_laps; lap++) {
        int pace_end = 0;
        int pace_sum = 0;
        int samples = 0;
        for (; (i * session->recording_interval < session->lap[lap]->lap_end_time) && (i < session->number_of_samples); i++) {
            pace_end = session->sample[i]->speed;
            pace_sum += session->sample[i]->speed;
            samples++;
        }
        session->lap[lap]->pace_end = pace_end;
        session->lap[lap]->pace_avg = (int) round((double) pace_sum / samples);
    }
}

void RS400_parse_session(unsigned char data[], rs400_session * session) {
    int i;
    /* Default to SI units for now. TODO. */
    session->si_units = 1;

    /* Exercise name at most 8 characters */
    rs400_decode_string(session->name, &data[8]);
    /* Exercise type */
    session->has_pace_data = (data[21] & 0x8) >> 3;     /* Shift may be unnecessary... */
    
    switch (data[22]) {
        case 0:
            session->recording_interval = 1;
            break;
        case 1:
            session->recording_interval = 5;
            break;
        case 2:
            session->recording_interval = 10;
            break;
        case 3:
            session->recording_interval = 60;
            break;
    }
    
    unsigned short timestamp = toshort(&data[24]);
    session->day = timestamp & 0x1f;
    session->month = (timestamp & 0x1e0) >> 5;
    session->year = 2000 + (timestamp >> 9);
    /* Time is encoded as BCD. Can't you guys decide? */
    session->start_second = bcd2i(data[26]);
    session->start_minute = bcd2i(data[27]);
    session->start_hour = bcd2i(data[28]);
    /* Create a unique session id from the name and timestamp */
    sprintf(session->id, "%d-%02d-%02d_%02d:%02d:%02d_%s",
            session->year,
            session->month,
            session->day,
            session->start_hour,
            session->start_minute,
            session->start_second,
            session->name
    );
    /* Session statistics */
    /* Session duration will be stored in seconds */
    session->duration = short2sec(&data[29]);

    /* Session heart rates */
    session->hr_avg = data[32];
    session->hr_max = data[33];
    session->hr_min = data[34];
    session->calories = toshort(&data[35]);

    /* User values */
    session->user_hr_max = data[38];
    session->user_hr_rest = data[39];
    session->user_vo2_max = data[40];
    
    /* Pace and lap summary */
    if (session->has_pace_data) {
        session->distance = toshort(&data[44]) * 10;
    }
    session->number_of_laps = data[49];
    session->best_laptime = bcd_hmsc2sec(&data[50]);
    session->best_lap = data[54];
    session->pace_max = toshort(&data[55]);
    session->pace_avg = toshort(&data[57]);
    
    /* Sport zone limits */
    for (i = 0; i < 5; i++) { /* I think there are ten */
        session->sportzone[4 - i].zone_number = 5 - i;
        session->sportzone[4 - i].low_percent = data[78 + i];
        session->sportzone[4 - i].high_percent = data[78 + i + 1];
        session->sportzone[4 - i].on_zone_seconds = bcd_hms2sec(&data[94 + i * 3]);
    }

    /* Find out how much storage is used for each lap entry */
    int sample_offset, lap_offset;
    if (session->has_pace_data) {
        sample_offset = 4;
        lap_offset = 13;
    } else {
        sample_offset = 1;
        lap_offset = 6;
    }

    session->number_of_samples = ceil(session->duration / session->recording_interval);
    /* printf(" (%d samples) ", session->number_of_samples); */
    
    /* Remove padding characters from the sample data. */
    unsigned char *srcptr = &data[128]; /* Start of sample data */
    unsigned char *dstptr = &data[128];
    for(int i = 0; i < session->number_of_samples * sample_offset; i++) {
        if ((i & 0xff) == 0x00) {
            /* Character to be skipped should be a zero */
            if (*srcptr != 0) {
                fprintf(stderr, "Padding character at 0x%x should be 0x00, found 0x%x\n", (int)srcptr, *srcptr);
            }
            srcptr++;
            continue;
        }
        *dstptr++ = *srcptr++;
    }
    /* Skip 0xff's at the end of the sample data.
    It may be better to align to the next 128/256? */
    while(*srcptr == 0xff) {
        *dstptr++ = *srcptr++;
    }
    /* Then move the lap data */
    memmove(dstptr, srcptr, lap_offset * session->number_of_laps);
    
    /* Parse samples */
    unsigned char *sample_data = &data[128];
    session->sample = (rs400_sample **)malloc(sizeof(rs400_sample*) * session->number_of_samples);
    for(int i = 0; i < session->number_of_samples; i++) {
        session->sample[i] = (rs400_sample*)malloc(sizeof(rs400_sample));
        session->sample[i]->hr = sample_data[i * sample_offset];
        if (session->has_pace_data)
            /* The speed is stored in the HRM in multiples of 1/256 km/h */
            session->sample[i]->speed = toshort(&sample_data[i * sample_offset + 2]);
        /* Could add other data sources if available */
    };

    /* Start parsing laps, if any. The laps are stored at the end of the data. */
    if (session->number_of_laps > 0) {
        session->lap = (rs400_lap **)malloc(sizeof(rs400_lap*) * session->number_of_laps);
    }
    /* Lap data starts at the beginning of the next 256-byte block */
    unsigned char *lap_data = sample_data + (session->number_of_samples * sample_offset);
    while (((int)(lap_data - data) & 0xff) != 0x80) {
        lap_data++;
    }
    /* printf("I think lap data starts at offset 0x%x\n", lap_data - data); */
    int lap;
    for (lap = session->number_of_laps - 1; lap >= 0; lap--, lap_data += lap_offset) {
        session->lap[lap] = (rs400_lap *)malloc(sizeof(rs400_lap));
        rs400_lap *current_lap = session->lap[lap];
        current_lap->lap_no = lap + 1;
        current_lap->lap_end_time = short2sec(&lap_data[0]);
        /* Calculate lap time */
        if (lap == 0) {
            session->lap[lap]->laptime = session->lap[lap]->lap_end_time;
        } else {
            session->lap[lap]->laptime = session->lap[lap]->lap_end_time - short2sec(&lap_data[lap_offset]);
        }
        current_lap->autolap = (lap_data[2] & 0x80);
        current_lap->hr_end = (lap_data[3]);
        current_lap->hr_avg = (lap_data[4]);
        current_lap->hr_max = (lap_data[5]);

    }
    /* Finally, generate some other values from the data */
    find_lap_hr_min(session);
    find_lap_distances(session);
    find_lap_paces(session);
}
