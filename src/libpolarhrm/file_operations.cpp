/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */


#include <stdio.h>
#include <errno.h>
 
#include <sys/types.h>
#include <sys/stat.h>/* struct stat, fchmod (), stat (), S_ISREG, S_ISDIR */
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//#include "polarhrm.h"

// need to change this to c++ style

//   http://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c

int fsize(const char* filename){

	FILE *fp;

	fp=fopen(filename,"r");

	int prev=ftell(fp);
	fseek(fp, 0L, SEEK_END);

	int sz=ftell(fp);

	fseek(fp,prev,SEEK_SET); //go back to where we were
	fclose(fp);

	return sz;
}




/* 
 http://www.linuxquestions.org/questions/programming-9/checking-if-a-file-exists-in-c-21700/
 */

bool does_file_exist (const char* path) {

	struct stat sts;


	if ((stat (path, &sts)) == -1 && errno == ENOENT) {

		return false; /* file does not exist */
	//	printf ("The file %s doesn't exist...\n", path);
	}

	else {

		 return true; /* file exists */
	}

} 


const char* create_filepath (const char path[], const char filename[], const char ext[]){

	char *mypath;
	int pathlen;

	pathlen = strlen(path)+strlen(filename)+strlen(ext);

	mypath =( char*) malloc(sizeof( char)*pathlen);

	strcpy(mypath,path);
			 
	strcat(mypath, filename);

	strcat(mypath, ext);
	
	return mypath;
}


