#ifndef _S625XCOMM_H
#define _S625XCOMM_H

#include "S625X.h"
#include "S625X_commands.h"

#include "../Watch/Overview.h"
#include "../../cli/polarhrm_config.h"

#include <list>
#include "../Datanode.h"

class Driver;
/*
	struct raw_data_node {
		unsigned char rawbuf[S625X_MAX_PACKET_SIZE];
		int len;
		struct raw_data_node* next;
	};
*/

class S625Xcomm {
  public:



	S625Xcomm(Driver * d);

	void setDriver(Driver * d);
	void initDriver(void);
	void closeDriver(void);
		
	void getOverview(unsigned char *raw_buffer, int &len);

	std::list<Datanode> getFiles(Overview* w_o);
	void deleteFile(unsigned char year,unsigned char month,unsigned char day,unsigned char hour,unsigned char minute,unsigned char second);
	void deleteAllFiles(void);

	void getUser(unsigned char &raw_user_data, int &len);

	int getReminder(unsigned char data[], unsigned char rem_num);	
	int setReminder(unsigned char data[], unsigned char len);

	// send the disconnect command to the watch
	// explecid closing of the driver required 
	void disconnect(void);

  protected:


  private:
	Driver * driver;
/*	void appendnode( raw_data_node** headref,unsigned char rawdata[], int rawdatalen);
	void delete_all_nodes ( raw_data_node** headref);
*/

};
#endif
