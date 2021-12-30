#!/bin/sh

## snap.sh by Ian LeCorbeau

ROOT=$(lsw -r)
MONW=$(wattr w "$ROOT")
MONH=$(wattr h "$ROOT")
CURWIN=$(pfw)
BARPX=20
BORDERPX=2
GAPPX=0
SCRW=$((MONW - GAPPX - BORDERPX))
SCRH=$((MONH - BARPX - GAPPX - BORDERPX))

case $1 in
	left) /usr/local/bin/wtp "$GAPPX" "$((BARPX + GAPPX))" "$(((SCRW / 2) - BORDERPX - GAPPX))" "$((SCRH - BORDERPX - GAPPX))" "$CURWIN" ;;
	down) /usr/local/bin/wtp "$GAPPX" "$(((SCRH / 2) + BARPX + GAPPX))" "$((SCRW - GAPPX - BORDERPX))" "$(((SCRH / 2) - BORDERPX - GAPPX))" "$CURWIN" ;;
	up) /usr/local/bin/wtp "$GAPPX" "$((BARPX + GAPPX))" "$((SCRW - GAPPX - BORDERPX))" "$(((SCRH / 2) - BORDERPX - GAPPX))" "$CURWIN" ;;
	right) /usr/local/bin/wtp "$(((SCRW / 2) + GAPPX))" "$((BARPX + GAPPX))" "$(((SCRW / 2) - BORDERPX - GAPPX))" "$((SCRH - BORDERPX - GAPPX)) " "$CURWIN" ;;
	center) /usr/local/bin/wtp "$GAPPX" "$((BARPX + GAPPX))" "$((SCRW - GAPPX - BORDERPX))" "$((SCRH - GAPPX - BORDERPX))" "$CURWIN" ;;
	*) printf '%s\n' "You must provide an argument (left, down, up, right or center)"

esac
