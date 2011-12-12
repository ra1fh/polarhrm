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


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(polarhrmdocdir)
