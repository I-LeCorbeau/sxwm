#!/bin/sh

CURWIN=$(/usr/local/bin/pfw)
PREVWIN=$(/usr/local/bin/lsw | grep -v "$CURWIN" | sed '$ p;d')
NEXTWIN=$(/usr/local/bin/lsw | grep -v "$CURWIN" | sed '1 p;d')

case $1 in
	next) wtf "$NEXTWIN" ;;
	prev) wtf "$PREVWIN" ;;
	*) printf '%s\n' "You must provide an argument. 'next' or 'prev'."
esac
