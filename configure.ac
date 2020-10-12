#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([msh], [v0.1], [mitchellclay@protonmail.com])
AM_INIT_AUTOMAKE
#AC_CONFIG_SRCDIR([world2.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Check for readline library
PKG_CHECK_MODULES([READLINE], [readline],
  [
  CFLAGS="$CFLAGS $READLINE_CFLAGS"
  LDFLAGS="$LDFLAGS $READLINE_LDFLAGS"
  ],
)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
