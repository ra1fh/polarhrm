## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src \
	samples \
	doc



polarhrmdocdir = ${prefix}/doc/polarhrm
polarhrmdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

##polarhrmsamplesdir = ${prefix}/samples/polarhrm
##polarhrmsamples_DATA = \
##	11051501.hrm\
##	11073101.hrm\
##	2011-5-28_10_17.dump\
##	sample_log_s625x.txt\
##	11052801.hrm\
##	2011-5-15_12_1.dump\
##	2011-7-31_11_13.dump


##	doc      \
##	uml



EXTRA_DIST = $(polarhrmdoc_DATA) \
	TODO.tasks


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(polarhrmdocdir)
