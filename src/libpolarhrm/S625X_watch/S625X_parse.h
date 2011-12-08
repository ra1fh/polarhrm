#ifndef _S625XPARSE_H
#define _S625XPARSE_H



#include "../Watch/Overview.h"
#include "../Watch/RawSessions.h"
#include "../Watch/RawSessionFile.h"
#include "../Watch/Session.h"
#include "../Watch/sportZone.h"

#include "S625X_comm.h"

#include "../Datanode.h"
#include <list>


class S625Xparse {
  public:

	static RawSessions* parseRawSessions(Overview* w_overview, std::list<Datanode>*);
	static unsigned char find_minimum(unsigned char buf[], int len, int stepsize);


	Overview* parseOverview(unsigned char buf[], int len);
	Session* parseSession(RawSession *r_s);
	
//	Reminder* parseReminder(unsigned char buf[],int len);
//	void s625X_get_watch_user_parse(watch_user* w_user,unsigned char buf[],int len);

  private:

	void parse_samples(Session *w_session, RawSession *raw_sess);
	void parse_laps(Session *w_session, RawSession *raw_sess);
	int parse_sportzones(SportZone **w_lzone, unsigned char sportzone_array[], int count);

};


#endif
