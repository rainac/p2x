#! /bin/sh

# WIN32PREFIX should point to a directory where FlexLexer.h is linked
# to. Since this file is only included it may be the system one,
# i.e. /usr/include/FlexLexer.h. However, doing -I/usr/include will
# fail. Therefor this solution.
WIN32PREFIX=$HOME/sw/win32/include

./configure --host=i586-mingw32msvc CPPFLAGS="-I$WIN32PREFIX" $*
