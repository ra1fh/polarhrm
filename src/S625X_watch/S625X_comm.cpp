
#include <cstdio>
#include <cstdlib>
#include <cstring>


#include "Driver/driver.h"
#include "Watch/Overview.h"

#include "S625X_comm.h"
#include "S625X_commands.h"

#include "polarhrm_config.h"
#include "../util_functions.h"


S625Xcomm::S625Xcomm(Driver * d){

	this->driver = d;

}



void S625Xcomm::setDriver(Driver * d){

	this->driver = d;
}


void S625Xcomm::initDriver(void){

	this->driver->init();
}


void S625Xcomm::closeDriver(void){

	this->driver->close();
}



void S625Xcomm::getUser(unsigned char & raw_user_data, int & len) {

}




// get the watch overview with some basic data
void S625Xcomm::getOverview(unsigned char *raw_buffer, int &len) {

	unsigned char query[] = { S625X_GET_OVERVIEW };
	this->driver->sendbytes(query, sizeof(query));
	
	// return the length
	len =  driver->recvbytes(raw_buffer);
}




/* get the sessiondata out of the watch and store them in a node list. 
 the idea is to have a clear separation and any datamaipulation 
 data are manipulated at the with parse functions */
std::list<Datanode> S625Xcomm::getFiles(Overview* w_o) {

		std::list<Datanode> l;

		int checksum = 0;

	    // orginal transmitted data
 //       FILE *fp_org;  //just for deep debugging
 
	if ((int)w_o->number_of_sessions>0) {

		
		int len;
		unsigned char buf[BUF_SIZE];
		
		unsigned char query[]  = { S625X_GET_SESSIONS };
		unsigned char cquery[] = { S625X_CMD_CONTINUE_TRANSMISSION };


//		fp_org=fopen("/home/thomas/orginal.dump","w");

				//emty buffer
				for (int j= 0;j<=BUF_SIZE;j++)
					buf[j]=0;
			
				// initial query 
				driver->sendbytes(query, sizeof(query));

				len=driver->recvbytes(buf);

				// need to subtract the protocol specific bytes
				// but ONLY for checksum.
				//will be managed in parse function
				checksum=checksum+len-7;
		

//				fwrite(buf,sizeof(unsigned char), len,fp_org);
		
				//put the received data into a node list
				Datanode node(buf, len);
				l.push_back(node);


		// after sending the init query go ahead a long as all bytes get received
		while (checksum < w_o->used_bytes) {

				//emty buffer
				for (int j=0;j<=BUF_SIZE;j++)
					buf[j]=0;
			
				// send continue command 
				driver->sendbytes(cquery, sizeof(cquery));
				len=driver->recvbytes(buf);

				// need to subtract the protocol specific bytes
				// but ONLY for checksum.
				//will be managed in parse function
				checksum=checksum+len-3;
						
				#if defined(DEBUGPRINT)
				printf("get sessiondata bytes: %d / %d\n",checksum, w_o->used_bytes);
				#endif

//				fwrite(buf,sizeof(unsigned char), len,fp_org);
				
				//put the received data into a node list
				Datanode node(buf, len);
				l.push_back(node);
				//nodecount++;
		   }
	
	} //end if

    #if defined(DEBUGPRINT)
      printf("\nsession raw data received %d / %d\n",checksum,w_o->used_bytes);
    #endif

//fclose(fp_org);

return l;
}



int S625Xcomm::getReminder(unsigned char data[], unsigned char rem_num){

	unsigned char query[1]; 

	query[0]=S625X_GET_RIMINDER;
	query[1]=rem_num;
	
	this->driver->sendbytes(query, sizeof(query));
	
return driver->recvbytes(data);
}



int S625Xcomm::setReminder(unsigned char data[], unsigned char len){

/* TODO implement later ...	
	unsigned char query[] = { S625X_SET_RIMINDER,data[],0x00 };
	sendbytes(query, sizeof(query));
*/	
return 0;
}

/*

 disconnect the watch

*/
void S625Xcomm::disconnect(void) {

	unsigned char query[] = { S625X_CMD_WATCHMODE };
	this->driver->sendbytes(query, sizeof(query));

}


/*
 delete a session file from the watch
*/
void S625Xcomm::deleteFile(unsigned char bcdyear,
                           unsigned char month,
                           unsigned char bcdday,
                           unsigned char bcdhour,
                           unsigned char bcdminute,
                           unsigned char bcdsecond){

/*
delete session 1 (File 20110913_20:56:38.0.dump)
0000   00 0c fe 38 56 20 13 11 09                       ...8V ...

delete session 2 (File 20110913_20:56:28.0.dump)
0000   00 0c fe 28 56 20 13 11 09                       ...(V ...

*/

	unsigned char query[7];
	//unsigned char confirm_query[] = { 0x00 };
	//unsigned char confirm_query[] = { 0x6 };

	unsigned char confirm_query[] = { 0x18, 0x00 };
	//unsigned char confirm_query[] = { 0x0, 0x18, 0x0 };

	query[0] = S625X_DELETE_SESSION;
	query[1] = 0xfe; // I triggert this value at ppp software for specific session deletion



/*FIXME don t know how the delete works in detail */
//	query[1] = 0xff; // delete all files (don t know the rest of the parameters has to be)

	query[2] = bcdsecond; // second
	query[3] = bcdminute; // minute
	query[4] = bcdhour; // hour
	query[5] = bcdday; // day
	query[6] = bcdyear; // year last 2 digits
	query[7] = month; // month

	#ifdef DEBUGPRINT
	std::cout<< "delete command: ";
	for (int i=0; i<=7; i++) {
		printf("%02x ", query[i]);
	} 
	std::cout<<"\n";
	#endif
	this->driver->sendbytes(confirm_query, sizeof(confirm_query));


	unsigned char buf[BUF_SIZE]; 
	int len;
	len = this->driver->recvbytes(buf);

		for (int i=0; i<=len; i++) {

			std::cout << hex <<(int) buf[i] << " ";
		}

	 this->driver->sendbytes(query, sizeof(query));

	
				

}


/*
 delete all sessionfiles from the watch
*/
void S625Xcomm::deleteAllFiles(void){

	unsigned char query[7];

	query[0] = 0x0c;

	query[1] = 0xff; // delete all files
	query[2] = bcd(00); // second
	query[3] = bcd(00); // minute
	query[4] = bcd(00); // hour
	query[5] = bcd(02); // day
	query[6] = bcd(00); // year last 2 digits
	query[7] = bcd(00); // month

	#ifdef DEBUGPRINT
	std::cout<< "delete command: ";
	for (int i=0; i<=7; i++) {
		std::cout<< hex <<(int) query[i]<<" ";
	} 
	std::cout<<"\n";
	#endif

	this->driver->sendbytes(query, sizeof(query));

}



