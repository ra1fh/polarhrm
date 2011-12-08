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

#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "convert_functions.h"
#include "rs400_struct.h"

extern void display_info(void);
extern void plotheader(int, char *);
extern void plotdata(int, char *, char *, ...);
extern void plotpacedata(int, char *, int, int);
extern void plotzonedata(int, char *, int, char *, int);
extern void plotzonestringdata(int, char *, int, char *, char *);
extern void xmlstarttag(char *, int, char *, char *, ...);
extern void xmlendtag(char *, int);
extern void xmlvaluetag(char *, int, char *, ...);
extern void update_status(char *, int);
extern void print_debug(char *, ...);
extern void print_deep_debug(char *, ...);
extern void update_level(double);
extern void update_progress(double);
extern void fail(char *);
extern void set_debug(char);
extern char get_debug();
extern void set_deep_debug(char);
extern char get_deep_debug();
extern void set_output_stream(FILE *);
extern FILE *get_output_stream();

#endif
