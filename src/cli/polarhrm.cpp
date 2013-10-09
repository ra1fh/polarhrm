/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * polarhrm
 * Copyright (C) Thomas Foyth 2011 <thomas.foyth@gmail.com>
 * 
 * polarhrm is free software: you can redistribute it and/or modify it
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


#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <glib.h>
#include <glib/gfileutils.h>
#include <sys/stat.h>

#include "polarhrm.h"
#include "polarhrm_config.h"
#include <argp.h>

#include "libpolarhrm/libpolarhrm.h"
#include "libpolarhrm/libpolarhrm_debug.h"
#include "libpolarhrm/S625X_watch/S625X.h"
#include "libpolarhrm/RS800CX_watch/RS800CX.h"
#include "libpolarhrm/RCX5_watch/RCX5.h"

#include "libpolarhrm/Watch/Watch.h"
#include "libpolarhrm/file_operations.h"
#include "libpolarhrm/util_functions.h"


#include "../config.h" // just for the version number

// http://www.crasseux.com/books/ctutorial/Processing-command-line-options.html#Processing%20command-line%20options
// http://code.google.com/p/master-mind/wiki/argp

const char *argp_program_version = PACKAGE_STRING;

const char *argp_program_bug_address =
"http://code.google.com/p/polarhrm";

/* This structure is used by main to communicate with parse_opt. */
struct appoptions {
	char *args[1];			/* WATCH_MODEL */
	bool deleteSession;		/* the -e flag */
	bool overwriteHrmFiles; /* the -o flag */
	bool listDevices;	    /* the -l flag */
	int debuglevel;			/* Argument -d  */
	std::string rawfilepath;	/* Argument -r */
	std::string directory; /* Argument -t */
};

/*
	OPTIONS.  Field 1 in ARGP.
	Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] = {
	{"target-directory", 't', "DIRECTORY", 0, "working directory"},
	{"erase",      'e', 0, 0, "erase all sessions"},
	{"overwrite",  'o', 0, 0, "overwrite existing hrm & dump files (not implemented yet)"},
	{"rawfile",    'r', "RAWFILE", 0, "read a raw session file and parse it"},
	{"debug",      'd', "DEBUGLEVEL", 0,"Debuglevel from 0 to 3 (implementation started 17.02.2013)"},
	{"list",       'l', 0, 0, "list the supported Devices"},
	{0}
};


/*
	PARSER. Field 2 in ARGP.
	Order of parameters: KEY, ARG, STATE.
*/
static error_t parse_opt (int key, char *arg, struct argp_state *state) {

	appoptions *arguments =static_cast<appoptions *>(state->input);

	switch (key) {
		case 't':
			arguments->directory = arg;
			break;
		case 'd':
			arguments->debuglevel =  *arg - 48; // fix ASCII to int
			break;
		case 'r':
			arguments->rawfilepath = arg;
			break;
		case 'e':
			arguments->deleteSession = true;
			break;
		case 'o':
			arguments->overwriteHrmFiles = true;
			break;
		case 'l':
			arguments->listDevices = true;
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num >= 1) {
				argp_usage(state);
			}
			arguments->args[state->arg_num] = arg;
			break;
		case ARGP_KEY_END:
			if (state->arg_num < 1){
				//argp_usage (state);
			}
			break;
		default:
			return ARGP_ERR_UNKNOWN;
		}
	return 0;
}

/*
	ARGS_DOC. Field 3 in ARGP.
	A description of the non-option command-line arguments
	that we accept.
*/
static char args_doc[] = "WATCH_MODEL";

/*
	DOC.  Field 4 in ARGP.
	Program documentation.
*/
static char doc[] = "polarhrm -- get data out of your HRM!";

/*
	The ARGP structure itself.
*/
static struct argp argp = {options, parse_opt, args_doc, doc};

/*
   The main function.
   Notice how now the only function call needed to process
   all command-line options and arguments nicely
   is argp_parse.
*/
int main (int argc, char **argv){

	appoptions arguments;

	/* Set argument defaults */
	arguments.debuglevel = 0;
	arguments.rawfilepath = "";
	arguments.overwriteHrmFiles = false;
	arguments.deleteSession = false;
	arguments.listDevices = false;
	arguments.directory = ""; //default dir if not set will be $HOME/.polarhrm


	/* if someone calls the program without any parameter 
	   display help menu */
	if(1 == argc) {
		argc++;
		argv[1] = (char *)"--help";
	}

	
	/* Where the magic happens */
	argp_parse (&argp, argc, argv, 0, 0, &arguments);



	std::cout<<"polarhrm  Copyright (C) 2013  Thomas Foyth\nThis program comes with ABSOLUTELY NO WARRANTY; for details see COPYING file in source.\n"
				"This is free software, and you are welcome to redistribute it under certain conditions;\n\n";

	/* Print out a list */
	if (true == arguments.listDevices) {
			printSupportedDevices();
			return EXIT_SUCCESS;
		}

	/* set the default working dir */
	gchar *polarhrm_path = NULL;

	if (arguments.directory.length()==0)
		polarhrm_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), ".polarhrm", NULL);

	else {
		polarhrm_path = g_build_path(G_DIR_SEPARATOR_S, arguments.directory.c_str(), NULL);
	}

	if(g_mkdir_with_parents(polarhrm_path, S_IRUSR|S_IWUSR|S_IXUSR) == -1) {
		perror("couldn't create directory");
		return EXIT_FAILURE;
	}

	/* setup parameters of libpolarhrm */
	setWorkingDir(polarhrm_path);
	libpolarhrm_setDebuglevel(arguments.debuglevel);

	/* Print argument values */
	std::cout<< "version = " << PACKAGE_STRING << std::endl;
	std::cout<< "debug = " << arguments.debuglevel << std::endl;
	std::cout<< "rawread = " << arguments.rawfilepath << std::endl;
	if (NULL != arguments.args[0])
		std::cout<< "WATCH_MODEL = " <<toUpperCase(arguments.args[0]) << std::endl;
	std::cout << "getWorkingDir = " << getWorkingDir() << std::endl;
	std::cout << "getDumpExtention = " << getDumpExtention() << std::endl;
	std::cout << "getHRMExtention = " << getHRMExtention() << std::endl;


	dev connected_device;
	connected_device = createEnumDeviceNum(arguments.args[0]);

	Watch *watch;

		switch (connected_device) {

			case DEVICE_S625X:
				watch = new S625X();
				break;
			case DEVICE_S725X:
				printf("mapping to S625x\n");
				watch = new S625X();
				break;
			case DEVICE_RS800CX:
				watch = new RS800CX();
				break;
			case DEVICE_RCX5:
				watch = new RCX5();
				break;
			case DEVICE_NOT_SUPPORTED:
				printf("device is not supported so far. :-(\nif it has an irda interface you can help supporting this device.\n");
				return EXIT_SUCCESS;
		}


	// option save hrm
	if ( 0 == arguments.rawfilepath.length()
		 && false == arguments.deleteSession) {
		watch->saveHRM();
	}

	// option delete all sessions
	else if (true == arguments.deleteSession) {
		watch->eraseSessions();
	}

	// option read raw form given file
	else if (arguments.rawfilepath.length() != 0) { 

		if (true == does_file_exist(arguments.rawfilepath.c_str())){
			watch->openRaw(arguments.rawfilepath);
		}
		else {
			printf("file does not exist: %s\n",arguments.rawfilepath.c_str());
			return EXIT_SUCCESS;
		}
	}

return EXIT_SUCCESS;
}





void printSupportedDevices(void) {

	int i=0;

	printf("Device\tDescription/Status\n");
	while( 0 != strcmp(supportedDevices[i].name, "")){

		printf ("%s\t%s\n",supportedDevices[i].name,
		                   supportedDevices[i].description.c_str());
		i++;
	};
}






dev createEnumDeviceNum(char* devicestring){

	int i=0;
	int comp_result;

	dev deviceID = DEVICE_NOT_SUPPORTED;

	devicestring = toUpperCase(devicestring);

	while( 0 != strcmp(supportedDevices[i].name, "")){

		comp_result = strcmp(devicestring, supportedDevices[i].name);
		if (0 == comp_result) {
			deviceID = supportedDevices[i].id;
		}
		i++;
	}

return deviceID;
}


