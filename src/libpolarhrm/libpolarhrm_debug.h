/***************************************************************************
 *            libpolarhrm_debug.h
 *
 *  Sun February 17 16:05:57 2013
 *  Copyright  2013  thomas
 *  <thomas.foyth@gmail.com>
 ****************************************************************************/
/*
 * libpolarhrm_debug.h
 *
 * Copyright (C) 2013 - thomas
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

#ifndef _LIBPOLARHRM_DEBUG_H_
#define _LIBPOLARHRM_DEBUG_H_


#include <iostream>
#include <cstdio>


#define DEBUG_INFO 1
#define DEBUG_WARN 2
#define DEBUG_CRITICAL 3


// http://stackoverflow.com/questions/1644868/c-define-macro-for-debug-printing
// http://blog.coly.li/docs/gcc_macro.pdf

#define dprint_info(fmt, ...) \
            do { if (libpolarhrm_debuglevel>=DEBUG_INFO) \
			fprintf(stderr, "debug info: "), \
			fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)

#define dprint_warn(fmt, ...) \
            do { if (libpolarhrm_debuglevel>=DEBUG_WARN) \
			fprintf(stderr, "debug warining: "), \
			fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)

#define dprint_critical(fmt, ...) \
            do { if (libpolarhrm_debuglevel>=DEBUG_CRITICAL) \
			fprintf(stderr, "debug critical: "), \
			fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)

//#define printf(fmt, ...) \
            do { if (globaldebug<=4) printf(fmt, ##__VA_ARGS__); } while (0)


extern int libpolarhrm_debuglevel;

void libpolarhrm_setDebuglevel(int);
int libpolarhrm_getDebuglevel(void);



#endif // _LIBPOLARHRM_DEBUG_H_
