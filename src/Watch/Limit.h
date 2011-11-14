#ifndef _LIMIT_H
#define _LIMIT_H

#include "wTime.h"

class Limit {

  public:
	unsigned int limit_low;
	unsigned int limit_high;
	unsigned int sport_zone; /* what is this? */

	wTime below_zone;
	wTime on_zone;
	wTime above_zone;

};
#endif
