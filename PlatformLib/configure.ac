#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])

AC_INIT(GameServer,1.0,www.zhijian.com)  
AC_CHECK_LIB([pthread], [pthread_rwlock_init])

AC_PROG_RANLIB


AC_PROG_LIBTOOL
AM_INIT_AUTOMAKE(GameServer,1.0)

AC_CONFIG_SRCDIR([ConfigManager.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h sys/time.h sys/timeb.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday memmove memset sqrt strchr strdup strncasecmp strrchr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
