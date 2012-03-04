dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(polarhrm, 2012-03-03, mailto:thomas.foyth@gmail.com)


AC_SUBST(MYPATH)

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

AC_PROG_CXX






AM_PROG_LIBTOOL






PKG_CHECK_MODULES(LIBXML,libxml-2.0)

PKG_CHECK_MODULES(LIBUSB,libusb)


AC_OUTPUT([
Makefile
doc/Makefile
src/Makefile
samples/Makefile
src/libpolarhrm/Makefile
src/libpolarhrm/RS400_watch/Makefile
])
