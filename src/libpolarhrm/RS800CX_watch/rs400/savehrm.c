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
#include <fcntl.h>
#include <unistd.h>

#include "rs400.h"

#define MAX_SIZE 32768

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Syntax: %s /dev/ircommX\n", argv[0]);
        exit(1);
    }

    unsigned char buf[MAX_SIZE];
    rs400_data *rs400 = (rs400_data *)malloc(sizeof(rs400_data));
    int size;
    int i;
    
    /* Read file from the HRM */
    RS400_init(argv[1]);
    
    size = RS400_get_overview(buf);
#ifdef RS400_DUMP_FILES
    RS400_dumpraw("overview.dump", buf, size);
#endif
    RS400_parse_overview(buf, rs400);
    
    rs400_session **session = (rs400_session **)malloc(sizeof(rs400_session*) * rs400->number_of_sessions);
    rs400->session = session;
    
    /* Now download all sessions */
    for (i = 0; i < rs400->number_of_sessions; i++) {
        char filename[35];
        rs400->session[i] = (rs400_session *)malloc(sizeof(rs400_session));
        printf("Session %d: downloading...", i + 1); fflush(stdout);
        size = RS400_get_session(i, buf);
        printf("parsing..."); fflush(stdout);
        RS400_parse_session(buf, rs400->session[i]);
        sprintf(filename, "%s.dump", rs400->session[i]->id);
#ifdef RS400_DUMP_FILES
        RS400_dumpraw(filename, buf, size);
#endif
        sprintf(filename, "%s.hrm", rs400->session[i]->id);
        printf("saving as %s...", filename); fflush(stdout);
        RS400_save_hrm(filename, rs400->session[i]);
        printf("done\n");
    }
    
    RS400_watchmode();
    RS400_close();

    return 0;
}
