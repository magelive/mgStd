#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.0])
AC_INIT([mgStd], [1.0], [magelive.jx@foxmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug], [enable debug mode @<:@default=no@:>@]), [], [enable_debug=no])
AS_IF([test "x$enable_debug" = "xyes"], [AC_DEFINE([DEBUG], [1], [Debugging mode enable])], 
		[AC_DEFINE([NDEBUG], [1], [Debugging and assertions disable])])
AM_CONDITIONAL([DEBUG], [test x$enable_debug = x"yes"])

# Checks for programs.
#AC_PROG_CC

#AM_INIT_AUTOMAKE
AC_PROG_LIBTOOL

# Checks for libraries.
AC_PROG_RANLIB

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h strings.h string.h stddef.h stdint.h sys/time.h time.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([bzero memset])

AC_SUBST([CFLAGS])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
