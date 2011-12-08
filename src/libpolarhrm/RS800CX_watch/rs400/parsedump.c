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

#define MAX_SIZE 32768

#include "rs400.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Syntax: %s infile.dump outfile.hrm\n", argv[0]);
        exit(1);
    }

    unsigned char buf[MAX_SIZE];
    rs400_session *session = (rs400_session *)malloc(sizeof(rs400_session));
    
    /* Load file into memory */
    int fd = open(argv[1], O_RDONLY);
    read(fd, buf, MAX_SIZE);
    close(fd);    
    RS400_parse_session(buf, session);
    RS400_save_hrm(argv[2], session);
    return 0;
}
