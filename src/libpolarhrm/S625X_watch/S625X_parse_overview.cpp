
#include <stdio.h>

#include "S625X_parse.h"



#include "../util_functions.h"
#include "../Watch/Overview.h"

/*


Subtype 0x15: "Get overview" response: Length = 11 bytes (6 payload):
-----------------------------------------------------------------------------
Payload byte	Meaning
=============================================================================
0		? always 0x22
1		? always 0x20
2		Number of files stored
3		? always 0x00
4		Bytes of workout data (MSB)
5		Bytes of workout data (LSB)


send command 15 
length 7 bytes
15 13 70 2 0 7 9C 0 
(2 Sessions)

send command 15 
length 7 bytes
15 13 70 3 0 8 2B 0 
(3 Sessions)
 */

//parse overview
Overview* S625Xparse::parseOverview(unsigned char buf[], int len) {

	int number_of_sessions=unbcd(buf[3]);
	int used_bytes=(buf[5] << 8) +buf[6];

	Overview *w_overview = new Overview(number_of_sessions, used_bytes);

	// DEBUG print 
	#if defined(DEBUGPRINT)
	printf("w_overview->getSessionNumber() %d\n",w_overview->getSessionNumber());
	printf("w_overview->getUsedBytes() %X\n",w_overview->getUsedBytes());
	#endif

return w_overview;
}