/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * polarhrm
 * Copyright (C) Thomas Foyth 2011 <thomas.foyth@gmail.com>
 * 
polarhrm is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * polarhrm is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GPXFILE_H_
#define _GPXFILE_H_

#include <iostream>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include "Session.h"

class GpxFile
{
public:

	void setPath(string path);
	void save(Session *session);

	void WriteHead(const char *, const char *);
	void WriteWayPoint(double lon, double lat, const char *wptTime, const char *fix, int sat);
	void WriteFooter(void);



protected:

private:

	int rc; // return code 
	xmlTextWriterPtr writer;
	xmlChar *tmp;
	string filename;

xmlChar * ConvertInput(const char *in, const char *encoding);

};

#endif // _GPXFILE_H_
