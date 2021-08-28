#!/bin/sh

CURWIN=$(/usr/local/bin/pfw)
PREVWIN=$(/usr/local/bin/lsw | grep -v "$CURWIN" | sed '$ p;d')
NEXTWIN=$(/usr/local/bin/lsw | grep -v "$CURWIN" | sed '1 p;d')

case $1 in
	next) /usr/local/bin/wtf "$NEXTWIN" ;;
	prev) /usr/local/bin/wtf "$PREVWIN" ;;
	*) printf '%s\n' "You must provide an argument. 'next' or 'prev'."
esac
