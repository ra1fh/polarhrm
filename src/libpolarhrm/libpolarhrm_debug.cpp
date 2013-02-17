// libpolarhrm_debug.cxx
//
// Copyright (C) 2013 - Thomas Foyth
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.


#include "libpolarhrm_debug.h"


int libpolarhrm_debuglevel=0;

void libpolarhrm_setDebuglevel(int debuglevel){

	if(debuglevel > 0) {
		
		libpolarhrm_debuglevel = debuglevel;

	}

}





int libpolarhrm_getDebuglevel(void){

	return libpolarhrm_debuglevel;

}
