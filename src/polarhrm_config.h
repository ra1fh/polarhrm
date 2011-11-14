#ifndef _POLARHRM_CONFIG_H_
#define _POLARHRM_CONFIG_H_

#include <iostream>

/*
 *
 *  USER SETTINGS 
 *  update them to your accourding to your needs
 *
 */

/* the data are stored in PPP software and are used to wirte the HRM file */
static unsigned char USER_HR_MAX = 195;
static unsigned char USER_HR_REST = 86;
static unsigned char USER_V02_MAX = 51;
static unsigned char USER_WEIGHT = 77;

/* there is also data set at PPP software reffering to your Sport Zones 
 the values are compared with your HR Samples and are for statistical purpose 
 for graphics.
 I don t know if sportstracker or Golden Cheetah store this values themselve or
 just take the values form the given HR file (section [HRZones])

 NOTE: form highest to sallest value -> starting at HR MAX not more than 10
 values allowed because they are wirtten to HRM file 

 176 - USER_HR_MAX  -> MAX
 156 - 175 			-> HARD
 137 - 155			-> MODERAT

 and so on ...
 there is a structure for future purpose sportzone (some newer watches 
 have this values stored at watchsession e.g. rs400)
 */
#define USER_NUMBER_OF_SPORTZONES 11
static unsigned char user_sportzones[USER_NUMBER_OF_SPORTZONES] =
					{USER_HR_MAX,176,156,137,117,USER_HR_REST,0,0,0,0,0 };


/* set up a wirtable dir to store files. 
   raw data and hrm are stored in this dir
   don t forget the last slash!!! 
   software does not check if dir exists or is wirtable
   (usabillity comes later)  */  
static const char *MYPATH = "/home/thomas/praw/";

/* comment this is you don t want a raw file 
   at this point of develompment please leave it as it is, and send 
   your raw data and original HRM processed by PPP 
   especially when you have cycling data by s625X */
#define DUMP_RAW 

/* temp filename
   is used to store session data before sessioninformation is parsed 
   after parsing the file will be renamed to corrct name */
static const char *TEMP_FILENAME = "temp";

/* extentions */
static const char *DUMP_EXTENTION = ".dump";
static const char *HRM_EXTENTION = ".hrm";


/* just prepared to get a more silent output
   but maybe I will do this with a command line switch */
#define DEBUGPRINT


/* HR Data are corrected during the parsing process 
 * when the HR sensor is not correctly messuring the HR 
 * this is during the training you don t see a value of HR on your watch display
 *
 * this gets fixed during processing at ppp software 
 * unforutnately I don t know the algorithem
 * see the session parsing function for details
 */
#define ENABLE_HR_FIXING




/* store general information about the supported watches 
   the information are used to write the out the HRM file */


/* witch devices are supported by now? */
enum dev {
	DEVICE_S625X=22, //use monitor number
	DEVICE_S725X, //www says it is exactly the same as S625X just has cycling gear packed instead of s1 footpod
	DEVICE_RS800CX=38,
	DEVICE_NOT_SUPPORTED = 99
};


typedef struct suppDevices{
	dev id;
	char name[20];// device name
	std::string description; 
} suppDevices;



static suppDevices supportedDevices[] = 
{
	{ DEVICE_S625X, "S625X", "hrm writting, run speed, altituede is working"},
	{ DEVICE_S725X, "S725X", "same as S625X"},
	{ DEVICE_RS800CX, "RS800CX", "current developing"}
};




/*
 *
 * SYSTEM SETTINGS
 * there should be no need to change them from this point.
 *
 */



/* Buffer size for many buffers used in the application */
#define BUF_SIZE 1024

/* Year is used for various calculations 
   sometimes only the last two digits are given */
#define BASE_YEAR 2000



#endif