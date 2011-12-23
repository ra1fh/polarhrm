




#include <iostream>
#include <list>
#include <cstring>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <bitset>

#include "libpolarhrm/Watch/wTime.h"
#include "libpolarhrm/Watch/wDate.h"

#include "libpolarhrm/Watch/RawSessions.h"
#include "libpolarhrm/Watch/RawSessionFile.h"

#include "libpolarhrm/S625X_watch/S625X_comm.h"
#include "libpolarhrm/S625X_watch/S625X_parse.h"


#include "libpolarhrm/Watch/Overview.h"
#include "libpolarhrm/Watch/RawSessions.h"
#include "libpolarhrm/Watch/RawSessionFile.h"
#include "libpolarhrm/Watch/HrmFile.h"
#include "libpolarhrm/file_operations.h"
#include "libpolarhrm/util_functions.h"

#include "libpolarhrm/libpolarhrm.h"
#include "libpolarhrm/Datanode.h"

using namespace std;

/* 
  this area is for testing code sipplets that are used in the main program

  add an testfunction with a short description and use some usefull status messages
  that allow following the test


  for shorter output just go to main and comment unsed function calls

 */



/* prototypes */

/* read out two raw sessions from file, stick them together 
   and process them like they come directly out of the watch */
void test_run_two_sess_parsing (void);

/* test the date and time object behaviour */
void test_date_and_time_obj(void);

/* test the object behaviour */
void test_session_obj(void);

/* code for deleting a single session */
void test_delete_single_session(void); 

/* test my datanode object to replace the raw_data node structure and stl list instead */
std::list<Datanode> test_my_datanode_obj(void);


/* test a the unencoding of gps data */
void test_gps_uncode(void);

/* test the init setup for the libpolarhrm */
void test_libpolarhrm_global_settings(void);

/* inplementation here */


void test_run_two_sess_parsing (void){

	cout << "Test: test_run_two_sess_parsing\n";

	unsigned char *buf1;
	unsigned char *buf2;
	int len1, len2, len;

	S625Xparse *parser = new S625Xparse;

	RawSession *rawsession1 = new RawSession();
	//rawsession1->readRaw("/home/thomas/praw/old/2011-5-15_12_1.dump");
	rawsession1->readRaw("/home/thomas/praw/11100901.dump");
	len1 = rawsession1->getRawBufferlen();
	buf1 = rawsession1->getRawBuffer();

					printf ("\nprint buffer 1\n");
				for (int k=0; k<len1; k++ ){
					printf("%X",buf1[k]);
				}

	RawSession *rawsession2 = new RawSession();
	rawsession2->readRaw("/home/thomas/praw/old/2011-5-28_10_17.dump");
	len2 = rawsession2->getRawBufferlen();
	buf2 = rawsession2->getRawBuffer();
	
					printf ("\nprint buffer 2\n");
				for (int n=0; n<len2; n++ ){
					printf("%X",buf2[n]);
				}


	len = len1 + len2;

	unsigned char *buf = new unsigned char[len]; //-1

	memcpy (buf, buf1, len1);
	memcpy (&buf[len1], buf2, len2);

					printf ("\nprint buffer gesamt\n");
				for (int n=0; n<len; n++ ){
					printf("%X",buf[n]);
				}

	
	RawSessions *all_rawsessions = new RawSessions();

	all_rawsessions->setUsedBytes(len);
	all_rawsessions->setSessionNumber(2);
	all_rawsessions->setRawBuffer(buf, len);
	all_rawsessions->split();


		for (int i=1; i<= 2; i++ ) {

			RawSession *raw_session;
			raw_session = all_rawsessions->getRawSession(i);

			//raw_session->print();

			std::string path;
			path.assign(workingDir);path.append(tempFilename);path.append(dumpExtention);

			raw_session->saveRaw(path);

			Session *session;// = new Session(); 
			session = parser->parseSession(raw_session);

			//create the filename string for the session
			session->setFileExtention(hrmExtention);
			session->createFilename(workingDir);

			std::string newpath;
			newpath.assign(workingDir);
			newpath.append(session->getFilename());
			newpath.append(dumpExtention);
			rename(path.c_str(), newpath.c_str());


			path.assign(workingDir);
			path.append(session->getFilename());
			path.append(hrmExtention);

			HrmFile *hrmfile = new HrmFile(22,106);
			hrmfile->setPath(path);
			hrmfile->save(session);

			std::cout << "saved hrm session number " << i << " @ " << path << std::endl;

			// emty memory

			delete raw_session;
			delete session; 
		}

}

void test_date_and_time_obj(void){

	cout << "Test: test_date_and_time_obj\n";

	wDate *date = new wDate(2011,12,1);
	wTime *time = new wTime(1,25,29,0);
	cout << date->toString() << endl;
	cout << time->toString() << endl;

	cout << date->getshortYear() << endl;

}


void test_session_obj(void){

	cout << "Test: test_session_obj\n";

	Session *ses = new Session();

	wDate* start_date = new wDate(2011,02,19);

	ses->setStartDate(start_date);

	ses->setFileExtention(hrmExtention);
	ses->createFilename(workingDir);

	cout << ses->getFilename()<< endl;

}


void test_delete_single_session(void) {

/*
	wDate *start_date = new wDate;
	start_date = session->getStartDate();

	this->watchcomm->deleteFile(bcd((unsigned char)(start_date->getYear()-BASE_YEAR)),
					start_date->getMonth(), // for some reason this is not bcd coded
					bcd(start_date->getDay()),
					bcd(start_date->getHour()),
					bcd(start_date->getMinute()),
					bcd(start_date->getSecond()));


	this->watchcomm->deleteFile(bcd(11),
	                            10,
	                            bcd(4),
	                            bcd(22),
	                            bcd(18),
	                            bcd(42));


	unsigned char buf1[BUF_SIZE];
	int len1;
	Overview *w_overview1 = new Overview;
	this->watchcomm->getOverview(buf1, len1);
	w_overview1 = this->parser->parseOverview(buf1, len1);

*/

}


std::list<Datanode> test_my_datanode_obj(void){

	// http://www.cs.uregina.ca/Links/class-info/210/STLList/

	unsigned char buf1[] = {'1', '2', '3'}; // represent the raw data
	unsigned char buf2[] = {'4', '5', '6', '7'}; 
	unsigned char buf3[] = {'8', '9'}; 

	unsigned char* abcbuf;

	abcbuf = (unsigned char*) new unsigned char[3];

	abcbuf[0]=(unsigned char)'A';
	abcbuf[1]=(unsigned char)'B';
	abcbuf[2]=(unsigned char)'C';

	std::list<Datanode> l;

	Datanode node1 = Datanode(buf1,sizeof(buf1));
	l.push_back(node1);

	node1 = Datanode(buf2,sizeof(buf2));
	l.push_back(node1);

	node1 = Datanode(buf3,sizeof(buf3));
	l.push_back(node1);

	node1 = Datanode(abcbuf, 3);
	l.push_back(node1);



	// Output the list values, by repeatedly getting the item from
	// the "front" of the list, outputting it, and removing it
	// from the front of the list.

	unsigned char* bufx;

	// Loop as long as there are still elements in the list.
	while (l.size() > 0) {
		// Get the value of the "front" list item.
		Datanode current = l.front();

		 bufx = current.getBuffer();
		int max = current.getBufLen();
			cout << "length " << max << ":  ";
		
		for (int i=0; i<max; i++) {
			cout<< bufx[i] << "_" ; }
			cout << endl;

		//delete[] bufx;
		// Remove the item from the front of the list ("pop_front"
		// member function).
		l.pop_front();

	}
	return l;
}


void test_gps_uncode(void) {

/* 
	
	http://transition.fcc.gov/mb/audio/bickel/DDDMMSS-decimal.html
	https://groups.google.com/forum/?hl=de#!searchin/comp.lang.c/gps/comp.lang.c/nidxKt7khQA/ll6xED_MavwJ
	http://transition.fcc.gov/mb/audio/bickel/DDDMMSS-decimal.html
	http://objectmix.com/java/72734-convert-hex-decimal-latitude-longitude.html

Prater Standing
<trkpt lat="48.217256667" lon="16.395040000">
00 00 00 E3 9D 73 10 2E FD 41 30 lat 48.217256667 48° 13' 2.1246" 
00 00 00 E0 9D 73 10 32 FD 41 30  diff to citycenter -0.008191667
00 00 00 E2 9D 83 10 37 FD 41 30 
00 00 00 E8 9D 83 10 35 FD 41 30 
00 00 00 EA 9D 73 10 34 FD 41 30 
00 00 00 EB 9D 73 10 32 FD 41 30 
00 00 00 EC 9D 73 10 31 FD 41 30 
00 00 00 F7 9D 73 10 31 FD 41 30 
00 00 00 F6 9D 73 10 2D FD 41 30 
00 00 00 F8 9D 73 10 29 FD 41 30 
00 00 00 F5 9D 73 10 24 FD 41 30 
00 00 00 EC 9D 73 10 1E FD 41 30 

citycenter standing
	<trkpt lat="48.209065000" lon="16.372276667">
00 00 00 90 68 83 10 FB E9 41 30 48.209065000	48° 12' 32.634"
00 00 00 86 68 83 10 FF E9 41 30 48.209061667	48° 12' 32.6232"
00 00 00 88 68 83 10 FD E9 41 30 48.209070000	48° 12' 32.6514"
00 00 00 7E 68 83 10 02 EA 41 30 48.209068333	48° 12' 32.6448"
00 00 00 6F 68 83 10 01 EA 41 30 48.209041667	48° 12' 32.547"
00 00 00 32 68 83 10 F1 E9 41 30 48.209001667	48° 12' 32.4066"
04 00 00 DF 67 83 10 D9 E9 41 30 
07 00 00 C2 67 73 10 D0 E9 41 30 
00 00 00 BE 67 83 10 CF E9 41 30 
00 00 00 B0 67 83 10 CD E9 41 30 
00 00 00 BF 67 73 10 D0 E9 41 30 
00 00 00 D8 67 83 10 D6 E9 41 30 

*/


//4adaee12 = 74°26.3003' = 74.438338 = 74° 26' 18.0168"
//	unsigned char lat[]={ 0x12, 0xee, 0xda, 0x4a};

	//unsigned char lat[]={ 0x2E, 0xFD, 0x41, 0x30};
	unsigned char lon[]={ 0xE3, 0x9D, 0x73, 0x10};

	// 48,216830000
	unsigned char lat[] = {0x32,0xFC,0x41,0x30};
	double lon_final, lat_final;

	/* // just for testing -> implementation
	   // can be seen in util_functions.cpp
	
	lat_final += lnib(lat[2])*0x10000;
	lat_final += lat[1]*0x100;
	lat_final += lat[0]*0x1;
	lat_final  = lat_final / 600000;
	lat_final += lat[3];

	*/


	printf("lat final %.9f %x\n", toGpsDec(lat), unib(lat[2]));
	printf("lon final %.9f %x\n", toGpsDec(lon), unib(lon[2]));




// http://www.cplusplus.com/forum/beginner/18566/
union UStuff
{
	float   f;
	unsigned char c[0];
	signed int i;
};
	UStuff vala, b;

// FF E9 41 30 48.209061667	48° 12' 32.6232"
//	float xx = 2.1246;
/*
	vala.c[3] = 0xFF;
	vala.c[2] = 0xE9;
	vala.c[1] = 0x41;
	vala.c[0] = 0x30;

	for (size_t i = 0; i < sizeof(UStuff); ++i){
		std::cout << "byte " << i << ": "
		<< hex << (unsigned int)(vala.c[i]) << " "<< std::bitset<8>(vala.c[i]) 
		<< std::endl;
		b.c[i] = vala.c[i];
	}


*/

}





void test_libpolarhrm_global_settings(void){

//setWorkingDir("/usr/x");
printf("getWorkingDir %s\n", getWorkingDir());
printf("getDumpExtention %s\n", getDumpExtention());
printf("getHRMExtention %s\n", getHRMExtention());


}








int main(void) {

	/* read out two raw sessions from file, stick them together 
	   and process them like they come directly out of the watch */
//	test_run_two_sess_parsing();

	/* test the date and time object behaviour */
//	test_date_and_time_obj();

	/* test the object behaviour */
//	test_session_obj();


//	test_my_datanode_obj();

	test_gps_uncode ();

//	test_libpolarhrm_global_settings();



return 0;
}
