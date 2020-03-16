#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([homeis], [1.0.0], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE([-Werror foreign subdir-objects])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

#sudo ./configure CC=arm-linux-gnueabihf-gcc CCP=arm-linux-gnueabihf-g++ --host=arm-linux-gnueabihf

#init libtoolize
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])

#AC_CHECK_LIB([httpserver], [webserver],,AC_MSG_ERROR(["httpserver lib not found"]))

AC_DEFINE([USE_THREADS],[1],[description])
AC_DEFINE([LUA_USE_MKSTEMP],[1],[description])
AC_DEFINE([MEMWATCH],[1],[description])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP

CPPFLAGS="-Ilib/libxml2-2.8.0/include/libxml2 \
-Ilib/libconfig/include \
-Ilib/libcurl/include
"

AC_LANG_PUSH([C++])

AC_CHECK_HEADER([libxml/parser.h],[],
	AC_MSG_ERROR(["parser.h not found"])
)

AC_CHECK_HEADER([libconfig.h++],[],
	AC_MSG_ERROR(["libconfig.h++ not found"])
)

AC_CHECK_HEADER([curl/curl.h],[],
	AC_MSG_ERROR(["curl.h not found. Install libcurl-dev package."])
)

#AC_CHECK_HEADER([httpserver.hpp],[],
#	AC_MSG_ERROR(["httpserver.hpp not found"])
#)

AC_LANG_POP([C++])


# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h inttypes.h limits.h locale.h mach/mach.h malloc.h netdb.h netinet/in.h stddef.h stdint.h stdlib.h string.h strings.h sys/file.h sys/ioctl.h sys/socket.h sys/time.h sys/timeb.h syslog.h termios.h unistd.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])
AC_CONFIG_FILES([Makefile src_3party/Makefile src/Makefile])
AC_OUTPUT
