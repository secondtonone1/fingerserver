#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])

AC_INIT(libcommon_a,1.0,www.zhijian.com)  
AC_CHECK_LIB([pthread], [pthread_rwlock_init])

AC_PROG_RANLIB


AC_PROG_LIBTOOL
AM_INIT_AUTOMAKE(libcommon_a,1.0)

AC_CONFIG_SRCDIR([FashionData.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
