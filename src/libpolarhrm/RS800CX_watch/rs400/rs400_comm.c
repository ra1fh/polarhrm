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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "rs400_comm.h"
#include "rs400_file.h"

static int rs400_comm_fd;

/* Send raw bytes to the device. Return the number of bytes written */
int RS400_sendbytes(const unsigned char query[], int size) {
    int sent = write(rs400_comm_fd, query, size);
    return sent;
}

/* Receive raw bytes from the device. Returns the number of bytes read */
/* Note that it's the caller's duty to clear the receive buffer */
int RS400_recvbytes(unsigned char buf[]) {
    return read(rs400_comm_fd, buf, RS400_MAX_PACKET_SIZE);
}

/* Initialise the tty socket */
int RS400_init(char socket[]) {
    /* Open socket for bidirectional communication */
    rs400_comm_fd = open(socket, O_RDWR | O_NOCTTY);
    if (rs400_comm_fd < 0) {
        perror(socket);
        exit(1);
    }
    /* Set port settings */
    struct termios tio;
    memset(&tio, 0, sizeof(tio));
    tio.c_cflag = B9600 | CS8 | CLOCAL | CREAD | CSTOPB;
    tio.c_iflag = IGNPAR;
    tio.c_oflag = 0;
    tio.c_lflag = 0;
    tio.c_cc[VTIME] = 100;
    tio.c_cc[VMIN] = 0;

    tcflush(rs400_comm_fd, TCIFLUSH);
    tcsetattr(rs400_comm_fd, TCSANOW, &tio);
    printf("Opened socket %s\n", socket);
    return rs400_comm_fd;
}

int RS400_factoryreset() {
    unsigned char query[] = { RS400_CMD_FACTORYRESET };
    return RS400_sendbytes(query, sizeof(query));
};

int RS400_watchmode() {
    unsigned char query[] = { RS400_CMD_WATCHMODE };
    return RS400_sendbytes(query, sizeof(query));
}

int RS400_sleepmode() {
    unsigned char query[] = { RS400_CMD_SLEEPMODE };
    return RS400_sendbytes(query, sizeof(query));
}

int RS400_get_overview(unsigned char data[]) {
    unsigned char query[] = { RS400_CMD_GETOVERVIEW };
    RS400_sendbytes(query, sizeof(query));
    return RS400_recvbytes(data);
}

int RS400_get_session(char id, unsigned char data[]) {
    unsigned char buf[RS400_MAX_PACKET_SIZE];
    int pos = 0;

    unsigned char query1[] = { RS400_CMD_GETSESSION, id, 0x00, 0x00 };
    RS400_sendbytes(query1, sizeof(query1));
    int size = RS400_recvbytes(buf) - 4; /* Skip first 4 bytes of header */
#ifdef RS400_DUMP_FILES
    char dumpfile[30];
    sprintf(dumpfile, "session%02d_offset%05d.dump", id, pos);
    RS400_dumpraw(dumpfile, buf, size + 4);
#endif
    memcpy(data, buf + 4, size);
    unsigned char query2[] = { RS400_CMD_CONTINUEXFER, id };
    pos = size;
    RS400_sendbytes(query2, sizeof(query2));
    size = RS400_recvbytes(buf) - 5; // Skip first 5 bytes of header */
    while(size > 2) {
#ifdef RS400_DUMP_FILES
        sprintf(dumpfile, "session%02d_offset%05d.dump", id, pos);
        RS400_dumpraw(dumpfile, buf, size + 5);
#endif
        memcpy(data + pos, buf + 5, size);
        pos += size;
        RS400_sendbytes(query2, sizeof(query2));
        size = RS400_recvbytes(buf) - 5;
    }
    return pos; /* Contains final size of the session file */
}

int RS400_get_user(unsigned char data[]) {
    unsigned char buf[20];
    unsigned char query[] = { RS400_CMD_GETSETTINGS, 0x00, 0x7e, 0x01 };
    RS400_sendbytes(query, sizeof(query));
    return RS400_recvbytes(buf);
}

int RS400_close() {
    return close(rs400_comm_fd);
}
