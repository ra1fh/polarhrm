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

#include <cstdio>
#include "libpolarhrm.h"


const char *workingDir;

const char *tempFilename = "temp";

const char *dumpExtention = ".dump";
const char *hrmExtention = ".hrm";
const char *gpxExtention = ".gpx";


void setWorkingDir(const char* path) {
	workingDir = path;
}

const char*getWorkingDir(void) {
	return workingDir;
}


void setDumpExtention(const char* ext) {
	dumpExtention = ext;
}
const char* getDumpExtention(void){
	return dumpExtention;
}

void  setHRMExtention(const char* ext){
	hrmExtention = ext;
}
const char* getHRMExtention(void){
	return hrmExtention;
}
