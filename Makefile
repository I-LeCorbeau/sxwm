# sxwm - Simple/Scriptable X Window Manager
# See LICENSE file for copyright and license details

include config.mk

SRC = sxwm.c events.c
OBJ = ${SRC:.c=.o}

all: options sxwm

options:
	@echo sxwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"


.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

sxwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f sxwm ${OBJ} sxwm-${VERSION}.tar.gz

dist: clean
	mkdir -p sxwm-${VERSION}
	cp -R LICENSE Makefile config.h config.mk\
		sxwm.1 sxwm.h ${SRC} sxwm-${VERSION}
	tar -cf sxwm-${VERSION}.tar sxwm-${VERSION}
	gzip sxwm-${VERSION}.tar
	rm -rf sxwm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f sxwm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/sxwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < sxwm.1 > ${DESTDIR}${MANPREFIX}/man1/sxwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/sxwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/sxwm\
		${DESTDIR}${MANPREFIX}/man1/sxwm.1

.PHONY: all options clean dist install uninstall
	rm -f ${DESTDIR}${PREFIX}/bin/${BIN}
