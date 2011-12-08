
bin_PROGRAMS =  \
	simulate \
	test \
	polarhrm

simulate_SOURCES =  \
	cli/simulate_watch.cpp

simulate_LDADD = libpolarhrm.la 

test_SOURCES =  \
	cli/test.cpp

test_LDFLAGS = libpolarhrm.la 

polarhrm_SOURCES =  \
	cli/polarhrm.cpp \
	cli/polarhrm.h \
	cli/polarhrm_config.h

polarhrm_LDADD = libpolarhrm.la 

lib_LTLIBRARIES =  \
	libpolarhrm.la

libpolarhrm_la_SOURCES =  \
	libpolarhrm/file_operations.cpp   \
	libpolarhrm/file_operations.h   \
	libpolarhrm/Driver/driver.h   \
	libpolarhrm/Driver/driver.cpp   \
	libpolarhrm/Driver/irda_driver.h   \
	libpolarhrm/Driver/irda_driver.cpp   \
	libpolarhrm/S625X_watch/S625X.cpp   \
	libpolarhrm/S625X_watch/S625X.h   \
	libpolarhrm/S625X_watch/S625X_comm.cpp   \
	libpolarhrm/S625X_watch/S625X_comm.h   \
	libpolarhrm/S625X_watch/S625X_commands.h   \
	libpolarhrm/S625X_watch/S625X_parse_overview.cpp   \
	libpolarhrm/S625X_watch/S625X_parse_session.cpp   \
	libpolarhrm/S625X_watch/S625X_parse.h   \
	libpolarhrm/S625X_watch/S625X_parse.cpp   \
	libpolarhrm/RS800CX_watch/RS800CX.cpp   \
	libpolarhrm/RS800CX_watch/RS800CX.h   \
	libpolarhrm/RS800CX_watch/RS800CX_comm.cpp   \
	libpolarhrm/RS800CX_watch/RS800CX_comm.h   \
	libpolarhrm/RS800CX_watch/RS800CX_commands.h   \
	libpolarhrm/RS800CX_watch/RS800CX_parse_overview.cpp   \
	libpolarhrm/RS800CX_watch/RS800CX_parse_session.cpp   \
	libpolarhrm/RS800CX_watch/RS800CX_parse.h   \
	libpolarhrm/RS800CX_watch/RS800CX_parse.cpp   \
	libpolarhrm/Watch/Overview.cpp   \
	libpolarhrm/Watch/Overview.h   \
	libpolarhrm/Watch/RawSessionFile.cpp   \
	libpolarhrm/Watch/RawSessionFile.h   \
	libpolarhrm/Watch/RawSessions.cpp   \
	libpolarhrm/Watch/RawSessions.h   \
	libpolarhrm/Watch/wDate.cpp   \
	libpolarhrm/Watch/wDate.h   \
	libpolarhrm/Watch/HrmFile.cpp   \
	libpolarhrm/Watch/HrmFile.h   \
	libpolarhrm/Watch/Lap.cpp   \
	libpolarhrm/Watch/Lap.h   \
	libpolarhrm/Watch/Limit.cpp   \
	libpolarhrm/Watch/Limit.h   \
	libpolarhrm/Watch/Sample.cpp   \
	libpolarhrm/Watch/Sample.h   \
	libpolarhrm/Watch/Session.cpp   \
	libpolarhrm/Watch/Session.h   \
	libpolarhrm/Watch/sportZone.cpp   \
	libpolarhrm/Watch/sportZone.h   \
	libpolarhrm/Watch/wTime.cpp   \
	libpolarhrm/Watch/wTime.h   \
	libpolarhrm/Watch/Watch.h   \
	libpolarhrm/Watch/Watch.cpp   \
	libpolarhrm/util_functions.cpp   \
	libpolarhrm/util_functions.h   \
	libpolarhrm/Datanode.cpp   \
	libpolarhrm/Datanode.h   \
	libpolarhrm/libpolarhrm.cpp   \
	libpolarhrm/libpolarhrm.h   \
	libpolarhrm/supported_devices.h
## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" 

AM_CFLAGS =\
	 -Wall\
	 -g




