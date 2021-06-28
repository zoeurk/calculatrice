#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([calcule], [1.0], [zoeurk@gmail.com])
AC_CONFIG_SRCDIR([calcule/calcule.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
LT_INIT
AM_INIT_AUTOMAKE([foreign])
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lparsearg':
AC_CHECK_LIB([parsearg], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_STRTOD
AC_CHECK_FUNCS([atexit floor memset sqrt strchr])

AC_CONFIG_FILES([Makefile
                 lib/Makefile
                 calcule/Makefile
		 compare/Makefile])
AC_OUTPUT
