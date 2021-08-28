# Sxwm version
VERSION = 0.1

# paths
PREFIX	 = /usr/local
MANPREFIX = ${PREFIX}/share/man

XCBINC = /usr/X11R6/include
XCBLIB = /usr/X11R6/lib

# Includes and Libs
INCS = -I${XCBINC}
LIBS = -L${XCBLIB} -lxcb -lxcb-util

# Flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=2 -DVERSION=\"${VERSION}\"
CFLAGS = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS = ${LIBS}

#Compiler and linker
CC = cc
LD = ${CC}

