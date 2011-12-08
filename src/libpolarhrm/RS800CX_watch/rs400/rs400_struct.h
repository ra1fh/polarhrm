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

#ifndef _RS400_STRUCT_H
#define _RS400_STRUCT_H

/* Summary data output formats */
#define OUTPUT_NORMAL 0
#define OUTPUT_CSV 1
#define OUTPUT_CSV_PRETTY 2

#define RS400_LIMIT_NONE 0
#define RS400_LIMIT_HR 1
#define RS400_LIMIT_PACE 2

typedef char boolean;

typedef struct rs400_sample_node {
    unsigned int hr;
    unsigned int speed;
} rs400_sample;

typedef struct {
    unsigned int lap_no;
    double lap_end_time;
    double laptime;
    boolean autolap;
    unsigned int hr_end;
    unsigned int hr_avg;
    unsigned int hr_max;
    unsigned int hr_min;
    unsigned int pace_end;
    unsigned int pace_avg;
    unsigned int distance;
} rs400_lap;

typedef struct {
    unsigned int zone_number;
    unsigned int low_percent;
    unsigned int high_percent;
    double on_zone_seconds;
} rs400_sportzone;

typedef struct {
    unsigned int limit_low;
    unsigned int limit_high;
    unsigned int sport_zone; /* what is this? */
    double below_zone_seconds;
    double on_zone_seconds;
    double above_zone_seconds;
} rs400_limiterzone;

typedef struct {
    unsigned int weight;
    unsigned int height;
    unsigned int birth_day;
    unsigned int birth_month;
    unsigned int birth_year;
    unsigned int hr_max;
    unsigned int hr_rest;
    unsigned int vo2_max;
    unsigned char sex[7];
    unsigned char activity_level[9];
} rs400_user;

typedef struct {
    char id[31];
    unsigned char name[9];
    unsigned char limit_type;
    boolean has_pace_data;
    unsigned int number_of_laps;
    unsigned int number_of_samples;
    unsigned int day;
    unsigned int month;
    unsigned int year;
    unsigned int start_hour;
    unsigned int start_minute;
    unsigned int start_second;
    double duration;
    unsigned int hr_avg;
    unsigned int hr_max;
    unsigned int hr_min;
    unsigned int user_hr_max;
    unsigned int user_hr_rest;
    unsigned int user_vo2_max;
    unsigned int calories;
    unsigned int si_units; /* true = kg/cm/km */
    unsigned int distance;
    unsigned int pace_max;
    unsigned int pace_avg;
    unsigned int best_lap;
    double best_laptime;
    unsigned int recording_interval;
    rs400_sportzone sportzone[5];
    rs400_limiterzone limiterzone[3];
    rs400_lap **lap;
    rs400_sample **sample;
} rs400_session;


typedef struct {
    unsigned short value;
    unsigned short recorded_day;
    unsigned short recorded_month;
    unsigned int recorded_year;
} rs400_ownIndex;

typedef struct {
    rs400_sportzone sportzone[5];
    double sum_session_time;
    double sum_session_distance;
    double sum_session_calories;
    unsigned short number_of_sessions;
} rs400_weekly_summary;

typedef struct {
    unsigned char decoding_date[11];
    unsigned int all_time_soniclink_sends;
    unsigned int all_time_number_of_sessions_stored;
    unsigned int all_time_number_of_sessions_with_pod;
    unsigned int all_time_number_of_tests_performed;
    double duration;
    unsigned short duration_recording_start_day;
    unsigned short duration_recording_start_month;
    unsigned short duration_recording_start_year;
    double calories;
    unsigned short calories_recording_start_day;
    unsigned short calories_recording_start_month;
    unsigned short calories_recording_start_year;
    unsigned int exercises;
    unsigned short exercises_recording_start_day;
    unsigned short exercises_recording_start_month;
    unsigned short exercises_recording_start_year;
    double distance;
    unsigned short distance_recording_start_day;
    unsigned short distance_recording_start_month;
    unsigned short distance_recording_start_year;
    unsigned short number_of_indices;
    unsigned short number_of_summaries;
    rs400_weekly_summary **summary;
    rs400_ownIndex **index;
} rs400_followup;

typedef struct {
    unsigned int number_of_sessions;
    rs400_user **user;
    rs400_session **session;
} rs400_data;

#endif
