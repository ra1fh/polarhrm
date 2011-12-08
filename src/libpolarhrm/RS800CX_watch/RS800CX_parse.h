/*
 * RS800CX_parse.h
 *
 * Copyright (C) 2011 - Thomas Foyth
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _RS800CXPARSE_H
#define _RS800CXPARSE_H



#include "../Watch/Overview.h"
#include "../Watch/RawSessions.h"
#include "../Watch/RawSessionFile.h"
#include "../Watch/Session.h"
#include "../Watch/sportZone.h"

#include <list>
#include "../Datanode.h"




class RS800CXparse {
  public:

	static RawSession* parseRawSession(std::list<Datanode>*);
	static unsigned char find_minimum(unsigned char buf[], int len, int stepsize);


	Overview* parseOverview(unsigned char buf[], int len);
	Session* parseSession(RawSession *r_s);
	
//	Reminder* parseReminder(unsigned char buf[],int len);
//	void RS800CX_get_watch_user_parse(watch_user* w_user,unsigned char buf[],int len);

  private:

	void parse_samples(Session *w_session, RawSession *raw_sess);
	void parse_laps(Session *w_session, RawSession *raw_sess);
	void parse_sportzones(Session *w_session, RawSession *raw_sess);
	void test(unsigned char pos[]);
	void show(int, unsigned char *pos);
};
#endif
