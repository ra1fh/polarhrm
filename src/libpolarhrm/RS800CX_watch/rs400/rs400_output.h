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

#ifndef _RS400_OUTPUT_H
#define _RS400_OUTPUT_H

#include <string.h>
#include <stdlib.h>

#include "rs400_struct.h"

#define OUTPUT_NORMAL 0
#define OUTPUT_CSV 1
#define OUTPUT_CSV_PRETTY 2

void session_summary(rs400_session);
void followup_summary(rs400_followup);
void session_as_csv(rs400_session);
void session_as_xml(rs400_session);
void raw_data(int *, int);
void raw_binary_data(int *, int);

#endif
