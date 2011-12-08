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

#ifndef _LIBPOLARHRM_H_
#define _LIBPOLARHRM_H_

/* set up a wirtable dir to store files. 
   raw data and hrm are stored in this dir
   don t forget the last slash!!! 
   software does not check if dir exists or is wirtable
   (usabillity comes later)  */  
static const char *workingDir = "/home/thomas/praw/";


/* temp filename
   is used to store session data before sessioninformation is parsed 
   after parsing the file will be renamed to corrct name */
static const char *tempFilename = "temp";

/* extentions */
static const char *dumpExtention = ".dump";
static const char *hrmExtention = ".hrm";

void setWorkingDir(const char*);
const char*getWorkingDir(void);
void setDumpExtention(const char* ext);
const char* getDumpExtention(void);
void setHRMExtention(const char* ext);
const char* getHRMExtention(void);

/* comment this is you don t want a raw file 
   at this point of develompment please leave it as it is, and send 
   your raw data and original HRM processed by PPP 
   especially when you have cycling data by s625X */
#define DUMP_RAW 



/* just prepared to get a more silent output
   but maybe I will do this with a command line switch */
#define DEBUGPRINT


/* Buffer size for many buffers used in the application */
#define BUF_SIZE 1024

/* Year is used for various calculations 
   sometimes only the last two digits are given */
#define BASE_YEAR 2000


#endif // _LIBPOLARHRM_H_
