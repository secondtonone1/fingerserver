#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ([2.63])

AC_INIT(GameServer,1.0,www.zhijian.com)  
AC_CHECK_LIB([pthread], [pthread_rwlock_init])

AC_PROG_RANLIB


AC_PROG_LIBTOOL
AM_INIT_AUTOMAKE(GameServer,1.0)

AC_CONFIG_SRCDIR([IOTimer.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netinet/in.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset select socket])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
