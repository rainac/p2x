# Process this file with autoconf to produce a configure script.
#
# This file is part of P2X.
# Copyright © 2013 Johannes Willkomm 
# See the file p2x.cc for copying conditions.  
#

AC_INIT(p2x, 0.4.1, [johannes@johannes-willkomm.de], 
                p2x, [http://github.com/rainac/p2x])

AC_CONFIG_AUX_DIR(config)
AC_CANONICAL_TARGET
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE()

AC_PROG_MAKE_SET
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LN_S
AC_PROG_RANLIB
AM_PROG_LEX

AC_CHECK_TOOL([STRIP], [strip], [strip-for-host-system-was-not-found])
AC_CHECK_PROGS([CP], [cp copy])
AC_CHECK_PROGS([RM], [rm del])
AC_CHECK_PROGS([MV], [mv rename])
AC_CHECK_PROGS([BASENAME], [basename])
AC_CHECK_PROGS([GHEAD], [ghead head])

AC_CHECK_PROGS([GENGETOPT], [gengetopt])
AM_CONDITIONAL(acond_GENGETOPT_AVAIL, [test "x$GENGETOPT" != "x"])

AC_CHECK_PROGS([TRANG], [trang])
AM_CONDITIONAL(acond_TRANG_AVAIL, [test "x$TRANG" != "x"])

AC_ARG_ENABLE([static], [  --enable-static         Build statically linked executables.
  --disable-static        Build dynamically linked executables.], [
      static_build=$enableval
], [ 
      static_build=no
])
AC_MSG_CHECKING([setting static_build])
AC_MSG_RESULT([static_build=$static_build])
if test "$static_build" = "yes"; then
   LDFLAGS="$LDFLAGS -static"
   defval_STATIC_BUILD=1
else
   defval_STATIC_BUILD=0
fi
AC_DEFINE_UNQUOTED([ADIMAT_STATIC_BUILD], [$defval_STATIC_BUILD], [1: static build])
AM_CONDITIONAL(acond_STATIC_BUILD, [test "x$static_build" = "xyes"])


AC_ARG_ENABLE([tests], [  --enable-tests         Build test code.], [
      build_tests=$enableval
], [ 
      build_tests=no
])
AC_MSG_CHECKING([setting build_tests])
AC_MSG_RESULT([build_tests=$build_tests])
if test "$build_tests" = "yes"; then
   defval_BUILD_TESTS=1
else
   defval_BUILD_TESTS=0
fi
AC_DEFINE_UNQUOTED([ADIMAT_BUILD_TESTS], [$defval_BUILD_TESTS], [1: static build])
AM_CONDITIONAL(acond_BUILD_TESTS, [test "x$build_tests" = "xyes"])


AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([stddef.h unistd.h float.h ctype.h], , AC_MSG_ERROR([mandatory header file not found!]))
AC_CHECK_HEADERS([cfloat cstdlib cstring cctype cunistd])

# Create variables telling the operating system name in a string.
AC_DEFINE_UNQUOTED(HOST_ARCH, "$target", [Host id string.])
AC_DEFINE_UNQUOTED(HOST_CPU, "$target_cpu", [Host cpu type])
AC_DEFINE_UNQUOTED(HOST_VENDOR, "$target_vendor", [Host vendor type])
AC_DEFINE_UNQUOTED(HOST_OS_LONG, "$target_os", [Long host operating system type])
target_os_short=`echo "$target_os" |sed -e s/-.*//`
AC_DEFINE_UNQUOTED(HOST_OS_SHORT, "$target_os_short", [Short host os type])
AC_DEFINE_UNQUOTED(install_prefix, "$prefix", [The prefix where P2X is installed.])

if test -z "$SVN_REVISION"; then
   SVN_REVISION=`./vcs-version.sh`
fi                         
AC_MSG_CHECKING([value for SVN_REVISION])
AC_MSG_RESULT([$SVN_REVISION])
AC_SUBST([SVN_REVISION])

echo "creating version.txt"
echo "$VERSION" > version.txt

echo "creating svn-version.txt"
echo "$SVN_REVISION" > svn-version.txt
av_LONG_VERSION="$VERSION-`cat svn-version.txt | tr ':' '_'`"
AC_MSG_CHECKING([value for LONG_VERSION])
AC_MSG_RESULT([$av_LONG_VERSION])
AC_DEFINE_UNQUOTED(LONG_VERSION, "$av_LONG_VERSION", [Long version string])

echo "creating VERSION_NUMBER"
VERSION_NUMBER=`echo "$VERSION" | sed -e 's/\.//2'`
AC_DEFINE_UNQUOTED(P2X_VERSION_NUMBER, $VERSION_NUMBER, [A string describing the current version as a floating point number.])
AC_SUBST(VERSION_NUMBER)
AC_MSG_CHECKING([value for VERSION_NUMBER])
AC_MSG_RESULT([$VERSION_NUMBER])

AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_PID_T
AC_CHECK_TYPES(long double)
# Find a type that is 32 bit wide
AC_CHECK_TYPES(uint32_t)
AC_CHECK_SIZEOF(unsigned int)
AC_CHECK_SIZEOF(unsigned long)

AC_CHECK_FUNCS([strchr strerror strrchr strstr strtol strtod strtoul], ,AC_MSG_ERROR([mandatory function not found!]))
AC_CHECK_FUNCS([atexit setenv getenv strdup stricmp strcasecmp memset])

# check for presence of the open function and its header fcntl.h
AC_CHECK_HEADERS([sys/stat.h])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 examples/Makefile
                 tests/Makefile
                 tests/sh/Makefile
])

AC_OUTPUT

