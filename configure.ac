#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

C_PREREQ([2.63])

AC_INIT(GameServer,1.0,www.zhijian.com)  
AC_CHECK_LIB([pthread], [pthread_rwlock_init])

AC_PROG_RANLIB


AC_PROG_LIBTOOL
AM_INIT_AUTOMAKE(GameServer,1.0)

AC_CONFIG_SRCDIR([ItemMsgHandler.h])
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
# FIXME: Replace `main' with a function in `-lcommon':
AC_CHECK_LIB([common], [main])
# FIXME: Replace `main' with a function in `-ljemalloc':
AC_CHECK_LIB([jemalloc], [main])
# FIXME: Replace `main' with a function in `-lmysqlclient':
AC_CHECK_LIB([mysqlclient], [main])
# FIXME: Replace `main' with a function in `-lnetwork':
AC_CHECK_LIB([network], [main])
# FIXME: Replace `main' with a function in `-lplatform':
AC_CHECK_LIB([platform], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([arpa/inet.h fcntl.h fenv.h float.h limits.h locale.h mach/mach.h malloc.h memory.h netinet/in.h stddef.h stdint.h stdlib.h string.h strings.h sys/ioctl.h sys/socket.h sys/time.h sys/timeb.h syslog.h unistd.h utime.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_TYPE_UINT32_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STRCOLL
#AC_FUNC_STRNLEN
AC_CHECK_FUNCS([alarm bzero dup2 floor gettimeofday isascii localtime_r memchr memmove memset modf rint select setlocale socket sqrt stpcpy strcasecmp strchr strcspn strdup strerror strpbrk strrchr strspn strstr strtol strtoul strtoull])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_SUBDIRS([CommonLib
                   NetworkLib
                   PlatformLib])
AC_OUTPUT
