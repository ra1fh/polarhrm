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

#include <stdarg.h>

#include "rs400_display.h"

static boolean debug = FALSE;
static boolean deep_debug = FALSE;
static FILE *output_stream = NULL;

static char *v_status = "";
static double v_level = 0.0;
static double v_progress = 0.0;


/* Displays progress info during data read */
void display_info(void) {
    if (!debug) {
        fprintf(stderr,
                "Volume level: %5.3f    Progress: %3.0f%%   Status: %-24s\r",
                v_level, 100.0 * v_progress, v_status);
    } else {
        fprintf(stderr, ".");
    }
}


/* Prints out a data header in standard format */
void plotheader(int format, char *header_text) {
    if (output_stream != NULL) {
        switch (format) {
        case OUTPUT_NORMAL:
            fprintf(output_stream, "\n--------------- %s -------------\n",
                    header_text);
            break;
        case OUTPUT_CSV_PRETTY:
            fprintf(output_stream, "#%s\n", header_text);
            break;
        }
    }
}


/* Prints out a data field */
void plotdata(int format, char *title, char *value_formats, ...) {
    va_list argp;

    if (output_stream != NULL) {
        va_start(argp, value_formats);
        switch (format) {
        case OUTPUT_NORMAL:
            fprintf(output_stream, "%-27s: ", title);
            vfprintf(output_stream, value_formats, argp);
            fprintf(output_stream, "\n");
            break;
        case OUTPUT_CSV:
            vfprintf(output_stream, value_formats, argp);
            fprintf(output_stream, ",%s\n", title);
            break;
        case OUTPUT_CSV_PRETTY:
            fprintf(output_stream, "%-27s,", title);
            vfprintf(output_stream, value_formats, argp);
            fprintf(output_stream, "\n");
            break;
        }
        va_end(argp);
    }
}


/* Prints out pace data given as string (such as pace in mm:ss / km) */
void plotpacedata(int display_format, char *title, int pace,
                  int number_format) {
    char output_string[40] = "";
    switch (number_format) {
    case PACE_FORMAT_MIN_PER_KM:
        sprintf(output_string, "%.2d:%.2d",
                minutes_in_sec_per_km(pace / 10),
                seconds_in_sec_per_km(pace / 10));
        break;
    case PACE_FORMAT_SEC_PER_KM:
        sprintf(output_string, "%d", pace / 10);
        break;
    case PACE_FORMAT_KM_PER_HOUR:
        sprintf(output_string, "%.3f", 3600 / ((double)pace / 10));
        break;
    }
    if (output_stream != NULL) {
        switch (display_format) {
        case OUTPUT_NORMAL:
            fprintf(output_stream, "%-27s: %s\n", title, output_string);
            break;
        case OUTPUT_CSV:
            fprintf(output_stream, "%s,%s\n", output_string, title);
            break;
        case OUTPUT_CSV_PRETTY:
            fprintf(output_stream, "%-27s,%s\n", title, output_string);
            break;
        }
    }
}


/* Prints out HR zone data given as number (such as zone limit) */
void plotzonedata(int format, char *zone_name, int zone_number,
                  char *title, int value) {
    char str[50];
    if (output_stream != NULL) {
        sprintf(str, "%s %d %s", zone_name, zone_number, title);
        switch (format) {
        case OUTPUT_NORMAL:
            fprintf(output_stream, "%-27s: %d\n", str, value);
            break;
        case OUTPUT_CSV:
            fprintf(output_stream, "%d,%s %s\n", value, zone_name, title);
            break;
        case OUTPUT_CSV_PRETTY:
            fprintf(output_stream, "%-27s,%d\n", str, value);
            break;
        }
    }
}


/* Prints out HR zone data given as string (such as time in format hh:mm:ss) */
void plotzonestringdata(int format, char *zone_name, int zone_number,
                        char *title, char *value) {
    char str[50];
    if (output_stream != NULL) {
        sprintf(str, "%s %d %s", zone_name, zone_number, title);
        switch (format) {
        case OUTPUT_NORMAL:
            fprintf(output_stream, "%-27s: %s\n", str, value);
            break;
        case OUTPUT_CSV:
            fprintf(output_stream, "%s,%s %s value\n", value, zone_name,
                    title);
            break;
        case OUTPUT_CSV_PRETTY:
            fprintf(output_stream, "%-27s,%s\n", str, value);
            break;
        }
    }
}

/* XML output functions */
void xmlstarttag(char *tagname, int indent, char *param,
                 char *paramformatstr, ...) {
    int t;
    va_list argp;

    if (output_stream != NULL) {
        for (t = 0; t < indent; t++) {
            fprintf(output_stream, "    ");
        }
        va_start(argp, paramformatstr);

        if (param != NULL && paramformatstr != NULL) {
            fprintf(output_stream, "<%s %s=\"", tagname, param);
            vfprintf(output_stream, paramformatstr, argp);
            fprintf(output_stream, "\">\n");
        } else {
            fprintf(output_stream, "<%s>\n", tagname);
        }
        va_end(argp);
    }
}


void xmlendtag(char *tagname, int indent) {
    int t;
    if (output_stream != NULL) {
        for (t = 0; t < indent; t++) {
            fprintf(output_stream, "    ");
        }
        fprintf(output_stream, "</%s>\n", tagname);
    }
}


void xmlvaluetag(char *tagname, int indent, char *valueformatstr, ...) {
    int t;
    va_list argp;

    if (output_stream != NULL) {
        for (t = 0; t < indent; t++) {
            fprintf(output_stream, "    ");
        }
        va_start(argp, valueformatstr);
        fprintf(output_stream, "<%s>", tagname);
        vfprintf(output_stream, valueformatstr, argp);
        fprintf(output_stream, "</%s>\n", tagname);
    }
}


/* Prints debug messages during program execution */
void print_debug(char *s, ...) {
    va_list argp;
    if (debug) {
        fprintf(stderr, "\nDebug: ");
        va_start(argp, s);
        vfprintf(stderr, s, argp);
        va_end(argp);
        fprintf(stderr, "\n");
    }
}

void print_deep_debug(char *s, ...) {
    va_list argp;
    if (deep_debug) {
        va_start(argp, s);
        vfprintf(stdout, s, argp);
        va_end(argp);
    }
}

/* Updates the level field */
void update_level(double l) {
    static double i_level = 0.0;
    static int c = 0;

    if (l > i_level)
        i_level = l;
    if (++c >= 4410) {          // update level every 0.1 seconds (sampling frequency = 44100)
        v_level = i_level;
        i_level = 0.0;
        c = 0;
        display_info();
    }
}


/* Updates the progress field */
void update_progress(double p) {
    v_progress = p;
    display_info();
}


/* Updates the status field  */
void update_status(char *s, int new_line) {
    v_status = s;
    display_info();
    if (new_line)
        fprintf(stderr, "\n");
}


/* fatal error handling  */
void fail(char *message) {
    fprintf(stderr, "\n%s\n", message);
    exit(1);
}


/* Getter and setter functions for variables related to output modes */
void set_debug(boolean value) {
    debug = value;
}

void set_deep_debug(boolean value) {
    deep_debug = value;
}

boolean get_debug() {
    return debug;
}

boolean get_deep_debug() {
    return deep_debug;
}

void set_output_stream(FILE * value) {
    output_stream = value;
}

FILE *get_output_stream() {
    return output_stream;
}
