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

#include "rs400_output.h"
#include "rs400_display.h"

#define RS400_SECTION_LENGTH 255

/* Prints out session data as a human-readable summary */
void session_summary(rs400_session sess_data) {
    static char *months[15] =
        { "00", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
"Sep", "Oct", "Nov", "Dec", "13?", "14?" };
    int format = OUTPUT_NORMAL;
    char time[16];
    int s;
    
    print_debug("Printing summary data.");

    plotheader(format, "SESSION DATA");
    plotheader(format, "User information");
    plotdata(format, "Weight (kg)", "%d", sess_data.user.weight);
    plotdata(format, "Height", "%d", sess_data.user.height);
    plotdata(format, "Birth date", "%s", sess_data.user.birth_date);
    plotdata(format, "Max pulse", "%d", sess_data.user.hr_max);
    plotdata(format, "VO max", "%d", sess_data.user.vo2_max);
    plotdata(format, "Sitting pulse", "%d", sess_data.user.hr_rest);
    plotdata(format, "Gender", "%s", sess_data.user.sex);
    plotdata(format, "Activity level", "%s",
             sess_data.user.activity_level);

    plotheader(format, "Session data");
    plotdata(format, "Session name", "%s", sess_data.name);

    plotdata(format, "Number of laps", "%d", sess_data.number_of_laps);

    sprintf(time, "%s %02d %d", months[sess_data.month],
            sess_data.day, sess_data.year);
    plotdata(format, "Session date", "%s", time);

    sprintf(time, "%02d:%02d:%02d", sess_data.start_hour,
            sess_data.start_minute,
            sess_data.start_second);
    plotdata(format, "Session start time", "%s", time);

    time_to_string(time, sess_data.duration);
    plotdata(format, "Session length", "%s", time);

    plotdata(format, "Average heart rate", "%d", sess_data.hr_avg);
    plotdata(format, "Max heart rate", "%d", sess_data.hr_max);
    plotdata(format, "Max set heart rate", "%d",
             sess_data.hr_max_set);
    plotdata(format, "Calories", "%d", sess_data.calories);

    plotheader(format, "Limiter Zones");
    plotdata(format, "Limiter type", "%d", (int)sess_data.limit_type);

    for (s = 0; s < 4; s++) {
        if (sess_data.limit_type == RS400_LIMIT_PACE) {
            plotpacedata(format, "Pace zone low (in min per km)",
                         sess_data.limiterzone[s].limit_low,
                         PACE_FORMAT_MIN_PER_KM);
            plotpacedata(format, "Pace zone high (in min per km)",
                         sess_data.limiterzone[s].limit_high,
                         PACE_FORMAT_MIN_PER_KM);
        } else if (sess_data.limit_type == RS400_LIMIT_HR) {
            plotzonedata(format, "Limiter zone", s + 1, "BPM limit low",
                         sess_data.limiterzone[s].limit_low);
            plotzonedata(format, "Limiter zone", s + 1, "BPM limit high",
                         sess_data.limiterzone[s].limit_high);
        } else {
            plotzonedata(format, "Limiter zone", s + 1, "SportZone",
                         sess_data.limiterzone[s].sport_zone);
        }
    }

    /* time spent on each zone */
    for (s = 0; s < 4; s++) {
        /* below */
        time_to_string(time, sess_data.limiterzone[s].below_zone_seconds);
        plotzonestringdata(format, "HR zone", s + 1, "below", time);
        /* on zone */
        time_to_string(time, sess_data.limiterzone[s].on_zone_seconds);
        plotzonestringdata(format, "HR zone", s + 1, "inside", time);
        /* above */
        time_to_string(time, sess_data.limiterzone[s].above_zone_seconds);
        plotzonestringdata(format, "HR zone", s + 1, "above", time);
    }

    for (s = 0; s < 5; s++) {
        plotzonedata(format, "Sport zone", s + 1, "low%",
                     sess_data.sportzone[s].low_percent);
        plotzonedata(format, "Sport zone", s + 1, "high%",
                     sess_data.sportzone[s].high_percent);
    }
    for (s = 0; s < 5; s++) {
        time_to_string(time, sess_data.sportzone[s].on_zone_seconds);
        plotzonestringdata(format, "Sport zone", s + 1, "on this zone",
                           time);
    }

    plotheader(format, "Lap summary");
    plotdata(format, "Total distance in meters", "%d",
             sess_data.distance);
    plotpacedata(format, "Max pace (in min per km)",
                 sess_data.pace_max, PACE_FORMAT_MIN_PER_KM);
    plotpacedata(format, "Avg pace (in min per km)",
                 sess_data.pace_avg, PACE_FORMAT_MIN_PER_KM);

    plotdata(format, "Best lap", "%d", sess_data.best_lap);

    time_to_string(time, sess_data.best_laptime);
    plotdata(format, "Best lap time", "%s", time);

    plotheader(format, "Laps");
    for (s = 0; s < sess_data.number_of_laps; s++) {

        plotheader(format, "");
        plotdata(format, "Lap no", "%d", sess_data.lap[s]->lap_no);

        time_to_string(time, sess_data.lap[s]->lap_end_time);
        plotdata(format, "Lap end time", "%s", time);
        time_to_string(time, sess_data.lap[s]->laptime);
        plotdata(format, "Laptime", "%s", time);
        plotdata(format, "End pulse", "%d", sess_data.lap[s]->hr_end);
        plotdata(format, "Avg pulse", "%d", sess_data.lap[s]->hr_avg);
        plotdata(format, "Max pulse", "%d", sess_data.lap[s]->hr_max);

        if (sess_data.has_pace_data) {
            plotpacedata(format, "End pace (in min / km)",
                         sess_data.lap[s]->pace_end,
                         PACE_FORMAT_MIN_PER_KM);
            plotpacedata(format, "Avg pace (in min / km)",
                         sess_data.lap[s]->pace_avg,
                         PACE_FORMAT_MIN_PER_KM);
            plotdata(format, "Lap length (in m)", "%d",
                     sess_data.lap[s]->distance * 10);
        }
    }
    plotheader(format, "END OF DATA");
}


/* Prints out session data as a comma separated values */
void session_as_csv(rs400_session sess_data) {
    int format = OUTPUT_CSV;
    int s;

    print_debug("Printing csv data.");

    /* in csv files, first start off with session_id */
    plotdata(format, "session_id", "%s", sess_data.id);

    plotheader(format, "SESSION DATA");
    plotheader(format, "User information");
    plotdata(format, "Weight (kg)", "%d", sess_data.user.weight);
    plotdata(format, "Height", "%d", sess_data.user.height);
    plotdata(format, "Birth date", "%s", sess_data.user.birth_date);
    plotdata(format, "Max pulse", "%d", sess_data.user.hr_max);
    plotdata(format, "VO max", "%d", sess_data.user.vo2_max);
    plotdata(format, "Sitting pulse", "%d", sess_data.user.hr_rest);
    plotdata(format, "Gender", "%s", sess_data.user.sex);
    plotdata(format, "Activity level", "%s",
             sess_data.user.activity_level);

    plotheader(format, "Session data");
    plotdata(format, "Session name", "%s", sess_data.name);

    if (sess_data.has_pace_data) {
        plotdata(format, "Has pace data", "%d", 1);
    } else {
        plotdata(format, "Has pace data", "%d", 0);
    }

    plotdata(format, "Number of laps", "%d", sess_data.number_of_laps);

    plotdata(format, "Session day", "%d", sess_data.day);
    plotdata(format, "Session month", "%d", sess_data.month);
    plotdata(format, "Session year", "%d", sess_data.year);
    plotdata(format, "Session start hour", "%d",
             sess_data.start_hour);
    plotdata(format, "Session start minute", "%d",
             sess_data.start_minute);
    plotdata(format, "Session start second", "%d",
             sess_data.start_second);

    plotdata(format, "Session length in seconds", "%.1f",
             sess_data.duration);

    plotdata(format, "Average heart rate", "%d", sess_data.hr_avg);
    plotdata(format, "Max heart rate", "%d", sess_data.hr_max);
    plotdata(format, "Max set heart rate", "%d",
             sess_data.hr_max_set);
    plotdata(format, "Calories", "%d", sess_data.calories);

    plotheader(format, "Limiter Zones");
    plotdata(format, "Limiter type", "%d", (int)sess_data.limit_type);

    for (s = 0; s < 4; s++) {
        if (sess_data.limit_type == RS400_LIMIT_PACE) {
            plotpacedata(format, "Pace zone low (in sec per km)",
                         sess_data.limiterzone[s].limit_low,
                         PACE_FORMAT_SEC_PER_KM);
            plotpacedata(format, "Pace zone low (in sec per km)",
                         sess_data.limiterzone[s].limit_high,
                         PACE_FORMAT_SEC_PER_KM);
        } else if (sess_data.limit_type == RS400_LIMIT_HR) {
            plotzonedata(format, "Limiter zone", s + 1, "BPM limit low",
                         sess_data.limiterzone[s].limit_low);
            plotzonedata(format, "Limiter zone", s + 1, "BPM limit high",
                         sess_data.limiterzone[s].limit_high);
        } else {
            plotzonedata(format, "Limiter zone", s + 1, "SportZone",
                         sess_data.limiterzone[s].sport_zone);
        }
    }

    /* time spent on each zone */
    for (s = 0; s < 4; s++) {
        plotdata(format, "Time below zone seconds", "%.1f",
                 sess_data.limiterzone[s].below_zone_seconds);
        plotdata(format, "Time on zone seconds", "%.1f",
                 sess_data.limiterzone[s].on_zone_seconds);
        plotdata(format, "Time above zone seconds", "%.1f",
                 sess_data.limiterzone[s].above_zone_seconds);
    }

    for (s = 0; s < 5; s++) {
        plotzonedata(format, "Sport zone", s + 1, "low%",
                     sess_data.sportzone[s].low_percent);
        plotzonedata(format, "Sport zone", s + 1, "high%",
                     sess_data.sportzone[s].high_percent);
    }
    for (s = 0; s < 5; s++) {
        plotdata(format, "On SportZone seconds", "%.1f",
                 sess_data.sportzone[s].on_zone_seconds);
    }

    plotheader(format, "Lap summary");
    plotdata(format, "Total distance in meters", "%d",
             sess_data.distance);
    plotpacedata(format, "Max pace (in sec per km)",
                 sess_data.pace_max, PACE_FORMAT_SEC_PER_KM);
    plotpacedata(format, "Avg pace (in sec per km)",
                 sess_data.pace_avg, PACE_FORMAT_SEC_PER_KM);
    plotdata(format, "Best lap", "%d", sess_data.best_lap);
    plotdata(format, "Best lap time", "%.1f",
             sess_data.best_laptime);

    plotheader(format, "Laps");
    for (s = 0; s < sess_data.number_of_laps; s++) {

        plotheader(format, "");
        plotdata(format, "Lap no", "%d", sess_data.lap[s]->lap_no);
        plotdata(format, "Lap end time", "%.1f",
                 sess_data.lap[s]->lap_end_time);
        plotdata(format, "Laptime seconds", "%.1f",
                 sess_data.lap[s]->laptime);
        plotdata(format, "Autolap", "%d", sess_data.lap[s]->autolap);
        plotdata(format, "End pulse", "%d", sess_data.lap[s]->hr_end);
        plotdata(format, "Avg pulse", "%d", sess_data.lap[s]->hr_avg);
        plotdata(format, "Max pulse", "%d", sess_data.lap[s]->hr_max);

        plotpacedata(format, "End pace (in sec / km)",
                     sess_data.lap[s]->pace_end, PACE_FORMAT_SEC_PER_KM);
        plotpacedata(format, "Avg pace (in sec / km)",
                     sess_data.lap[s]->pace_avg, PACE_FORMAT_SEC_PER_KM);
        plotdata(format, "Lap length (in m)", "%d",
                 sess_data.lap[s]->distance * 10);
    }
    plotheader(format, "END OF DATA");
}

/* Prints out session data as xml */
void session_as_xml(rs400_session sess_data) {
    int s;

    print_debug("Printing xml data.");

    xmlstarttag("rs200_session", 0, "id", "%s", sess_data.id);
    xmlstarttag("user", 1, NULL, NULL);
    xmlvaluetag("weight", 2, "%d", sess_data.user.weight);
    xmlvaluetag("height", 2, "%d", sess_data.user.height);
    xmlvaluetag("birth_date", 2, "%s", sess_data.user.birth_date);
    xmlvaluetag("hr_max", 2, "%d", sess_data.user.hr_max);
    xmlvaluetag("vo2_max", 2, "%d", sess_data.user.vo2_max);
    xmlvaluetag("sitting_hr", 2, "%d", sess_data.user.hr_rest);
    xmlvaluetag("gender", 2, "%s", sess_data.user.sex);
    xmlvaluetag("activity_level", 2, "%s", sess_data.user.activity_level);

    xmlendtag("user", 1);

    xmlstarttag("rs400_session_data", 1, NULL, NULL);

    xmlvaluetag("name", 2, "%s", sess_data.name);
    xmlvaluetag("day", 2, "%d", sess_data.day);
    xmlvaluetag("month", 2, "%d", sess_data.month);
    xmlvaluetag("year", 2, "%d", sess_data.year);
    xmlvaluetag("start_hour", 2, "%d", sess_data.start_hour);
    xmlvaluetag("start_minute", 2, "%d", sess_data.start_minute);
    xmlvaluetag("start_second", 2, "%d", sess_data.start_second);

    if (sess_data.has_pace_data) {
        xmlvaluetag("has_pace_data", 2, "%s", "TRUE");
    } else {
        xmlvaluetag("has_pace_data", 2, "%s", "FALSE");
    }

    xmlstarttag("summary", 2, NULL, NULL);
    xmlvaluetag("number_of_laps", 3, "%d", sess_data.number_of_laps);
    xmlvaluetag("distance", 3, "%d",
                sess_data.distance);
    xmlvaluetag("pace_max", 3, "%.1f",
                (double)sess_data.pace_max / 10);
    xmlvaluetag("pace_avg", 3, "%.1f",
                (double)sess_data.pace_avg / 10);
    xmlvaluetag("best_lap", 3, "%d", sess_data.best_lap);
    xmlvaluetag("best_laptime", 3, "%.1f", sess_data.best_laptime);
    xmlvaluetag("length", 3, "%.1f", sess_data.duration);
    xmlvaluetag("hr_avg", 3, "%d", sess_data.hr_avg);
    xmlvaluetag("hr_max", 3, "%d", sess_data.hr_max);
    xmlvaluetag("hr_max_set", 3, "%d", sess_data.hr_max_set);
    xmlvaluetag("calories", 3, "%d", sess_data.calories);
    xmlendtag("summary", 2);

    xmlstarttag("limiterzone", 2, NULL, NULL);
    xmlvaluetag("limiter_type", 3, "%d", (int)sess_data.limit_type);

    for (s = 0; s < 4; s++) {
        xmlstarttag("limiterzone", 3, "zoneno", "%d", s + 1);
        if (sess_data.limit_type == RS400_LIMIT_PACE) {
            xmlvaluetag("limit_low", 4, "%d",
                        sess_data.limiterzone[s].limit_low / 10);
            xmlvaluetag("limit_high", 4, "%d",
                        sess_data.limiterzone[s].limit_high / 10);
        } else if (sess_data.limit_type == RS400_LIMIT_HR) {
            xmlvaluetag("limit_low", 4, "%d",
                        sess_data.limiterzone[s].limit_low);
            xmlvaluetag("limit_high", 4, "%d",
                        sess_data.limiterzone[s].limit_high);
        } else {
            xmlvaluetag("limit_zone", 4, "%d",
                        sess_data.limiterzone[s].sport_zone);
        }
        xmlvaluetag("time_below", 4, "%.1f",
                    sess_data.limiterzone[s].below_zone_seconds);
        xmlvaluetag("time_on", 4, "%.1f",
                    sess_data.limiterzone[s].on_zone_seconds);
        xmlvaluetag("time_above", 4, "%.1f",
                    sess_data.limiterzone[s].above_zone_seconds);
        xmlendtag("limiterzone", 3);
    }

    xmlendtag("limiterzone", 2);
    xmlstarttag("sportzone", 2, NULL, NULL);
    for (s = 0; s < 5; s++) {
        xmlstarttag("sportzone", 3, "zoneno", "%d", s + 1);
        xmlvaluetag("low_percent", 4, "%d",
                    sess_data.sportzone[s].low_percent);
        xmlvaluetag("high_percent", 4, "%d",
                    sess_data.sportzone[s].high_percent);
        xmlvaluetag("time_on", 4, "%.1f",
                    sess_data.sportzone[s].on_zone_seconds);
        xmlendtag("sportzone", 3);
    }

    xmlendtag("sportzone", 2);

    xmlstarttag("laps", 2, NULL, NULL);
    for (s = 0; s < sess_data.number_of_laps; s++) {

        xmlstarttag("lap", 3, "lap_no", "%d", sess_data.lap[s]->lap_no);
        xmlvaluetag("lap_end_time", 4, "%.1f",
                    sess_data.lap[s]->lap_end_time);
        xmlvaluetag("laptime", 4, "%.1f", sess_data.lap[s]->laptime);
        xmlvaluetag("autolap", 4, "%d", sess_data.lap[s]->autolap);
        xmlvaluetag("hr_end", 4, "%d", sess_data.lap[s]->hr_end);
        xmlvaluetag("hr_avg", 4, "%d", sess_data.lap[s]->hr_avg);
        xmlvaluetag("hr_max", 4, "%d", sess_data.lap[s]->hr_max);

        xmlvaluetag("pace_end", 4, "%.1f",
                    (double)sess_data.lap[s]->pace_end / 10);
        xmlvaluetag("pace_avg", 4, "%.1f",
                    (double)sess_data.lap[s]->pace_avg / 10);
        xmlvaluetag("lap_length", 4, "%d",
                    sess_data.lap[s]->distance * 10);
        xmlendtag("lap", 3);
    }
    xmlendtag("laps", 2);
    xmlendtag("rs400_session_data", 1);
    xmlendtag("rs200_session", 0);
}


/* Prints out follow-up data as a human-readable summary */
void followup_summary(rs400_followup followup) {
    int format = OUTPUT_NORMAL;
    char time[16];
    char time2[16];
    int s, k;

    print_debug("Printing follow-up summary data.");

    plotheader(format, "FOLLOW-UP DATA");
    plotdata(format, "Decoding date", "%s", followup.decoding_date);

    /* OwnIndex values */
    plotheader(format, "OwnIndex history");
    for (s = 0; s < followup.number_of_indices; s++) {
        sprintf(time, "%02d.%02d.%d", followup.index[s]->recorded_day,
                followup.index[s]->recorded_month,
                followup.index[s]->recorded_year);
        plotdata(format, "OwnIndex", "%d (date %s)",
                 followup.index[s]->value, time);
    }

    /* All-time totals */
    plotheader(format, "Totals");

    sprintf(time, "%02d.%02d.%d",
            (int)followup.exercises_recording_start_day,
            (int)followup.exercises_recording_start_month,
            (int)followup.exercises_recording_start_year);
    plotdata(format, "Total sessions", "%d (recorded since %s)",
             followup.exercises, time);

    sprintf(time, "%02d.%02d.%d", followup.distance_recording_start_day,
            followup.distance_recording_start_month,
            followup.distance_recording_start_year);
    plotdata(format, "Total distance", "%.1f km (recorded since %s)",
             followup.distance, time);

    sprintf(time, "%02d.%02d.%d", followup.calories_recording_start_day,
            followup.calories_recording_start_month,
            followup.calories_recording_start_year);
    plotdata(format, "Total calories", "%.1f kcal (recorded since %s)",
             followup.calories, time);

    time_to_string(time, followup.duration);
    sprintf(time2, "%02d.%02d.%d", followup.duration_recording_start_day,
            followup.duration_recording_start_month,
            followup.duration_recording_start_year);
    plotdata(format, "Total duration", "%s (recorded since %s)", time,
             time2);

    plotdata(format, "All time SonicLink use", "%d",
             followup.all_time_soniclink_sends);
    plotdata(format, "All time session recordings", "%d",
             followup.all_time_number_of_sessions_stored);
    plotdata(format, "All time sessions with pod", "%d",
             followup.all_time_number_of_sessions_with_pod);
    plotdata(format, "All time tests performed", "%d",
             followup.all_time_number_of_tests_performed);


    /* Weekly totals */
    plotheader(format, "Weekly summaries");

    for (k = 0; k < followup.number_of_summaries; k++) {
        plotdata(format, "Week number", "%d", k + 1);
        time_to_string(time, followup.summary[k]->sum_session_time);
        plotdata(format, "Total duration", "%s", time);
        plotdata(format, "Total distance", "%.2f km",
                 followup.summary[k]->sum_session_distance);
        plotdata(format, "Total calories", "%.1f kcal",
                 followup.summary[k]->sum_session_calories);
        plotdata(format, "Total sessions", "%d",
                 followup.summary[k]->number_of_sessions);

        for (s = 0; s < 5; s++) {
            time_to_string(time,
                           followup.summary[k]->sportzone[s].
                           on_zone_seconds);
            plotzonestringdata(format, "Sport zone", s + 1, "on this zone",
                               time);
        }
        plotheader(format, "");

    }

    plotheader(format, "END OF DATA");
}


/* Prints out raw data without interpretation as comma separated values (for analysing fields etc.) */
void raw_data(int *data, int size) {
    int i;
    int header_size = 11;
    char binary[9];
    print_debug("Will print %d bytes of data and %d bytes of preamble.",
                size, header_size);

    fprintf(get_output_stream(), "# Raw data as csv:\n#\n");
    fprintf(get_output_stream(), "# byte,dec,hex,binary,comment\n");
    for (i = 0; i < size + header_size; i++) {
        if (i < header_size)
            fprintf(get_output_stream(), "%3d,", 0);
        else
            fprintf(get_output_stream(), "%3d,", i - (header_size - 1));

        byte_to_binary_str(binary, data[i]);
        fprintf(get_output_stream(), "%3d,%02X,%s", data[i], data[i],
                binary);

        if (i == 0)
            fprintf(get_output_stream(),
                    (i <
                     size - 1) ? ",section start" : ",no more sections");
        if (i == 1)
            fprintf(get_output_stream(), ",data format type");
        if (i == 3)
            fprintf(get_output_stream(), ",number of sections");
        if (i == 4)
            fprintf(get_output_stream(), ",number of data bytes (lsb)");
        if (i == 5)
            fprintf(get_output_stream(), ",number of data bytes (msb)");
        fprintf(get_output_stream(), "\n");
    }
}



/* Outputs a replica of the bytes (including headers and padding) from RS200 into a binary file */
void raw_binary_data(int *data, int size) {
    int i;
    int preamble_size = 8;
    int section_max_size = 220;
    int section_size = 0;
    int written_size = 0;
    int counter = 0;
    int total_counter = 0;
    int section_counter = 0;

    print_debug("Outputting preamble.");

    for (i = 0; i < preamble_size; i++) {
        fputc(data[counter], get_output_stream());
        counter++;
    }

    while (counter < size) {
        section_counter++;
        print_debug("Outputting next section.");
        if (size - written_size > section_max_size)
            section_size = section_max_size;
        else
            section_size = size - written_size;
        written_size = written_size + section_size;

        /* check if section header is missing from original data and if so, write it */
        if (data[counter] != 85 || data[counter + 1] != section_counter) {
            fputc(85, get_output_stream());
            fputc(section_counter, get_output_stream());
            fputc(section_size, get_output_stream());
            print_deep_debug("w: %d,header\n", 85);
            print_deep_debug("w: %d,header\n", section_counter);
            print_deep_debug("w: %d,header\n", section_size);
        } else {
            fputc(data[counter++], get_output_stream());
            fputc(data[counter++], get_output_stream());
            fputc(data[counter++], get_output_stream());
            print_deep_debug("w: %d,header\n", data[counter - 3]);
            print_deep_debug("w: %d,header\n", data[counter - 2]);
            print_deep_debug("w: %d,header\n", data[counter - 1]);
        }
        for (i = 0; i < section_size; i++) {
            fputc(data[counter], get_output_stream());
            print_deep_debug("w: %d,data\n", data[counter]);
            counter++;
        }
        /* add padding */
        total_counter = counter;
        for (i = section_size; i < RS400_SECTION_LENGTH - 3; i++) {

            fputc(0, get_output_stream());
            print_deep_debug("w: %d,padding\n", 0);
            total_counter++;
        }
    }
    /* finish off with a trailer byte */
    fputc(7, get_output_stream());
}
