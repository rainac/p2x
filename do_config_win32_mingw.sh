#! /bin/sh

# WIN32PREFIX should point to a directory where FlexLexer.h is linked
# to. Since this file is only included it may be the system one,
# i.e. /usr/include/FlexLexer.h. However, doing -I/usr/include will
# fail. Therefor this solution.
WIN32PREFIX=${WIN32PREFIX:-$HOME/sw/win32}

#old mingw
#HOST=i586-mingw32msvc
#new MingW, 32 bit
#HOST=i686-w64-mingw32
#new MingW, 64 bit
HOST=x86_64-w64-mingw32

./configure --prefix=$WIN32PREFIX --host=$HOST CPPFLAGS="-I$WIN32PREFIX/include" $*
