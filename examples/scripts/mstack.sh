#!/bin/sh
#
# z3bra - 2014 (c) wtfpl
# Ian LeCorbeau - 2021 (c) wtfpl
# arrange windows in a tiled pattern

# default values for gaps and master area
PANEL=${PANEL:-20}
GAP=${GAP:-8}

# get current window id and its borderwidth
PFW=$(pfw)
BW=$(wattr b $PFW)

# get root window's size (beware, multi-head setups...)
ROOT=$(lsw -r)
SW=$(wattr w $ROOT)
SH=$(wattr h $ROOT)

# get the number of windows to put in the stacking area
MAX=$(lsw|grep -v $PFW|wc -l)
WINNUM=$(lsw | wc -l)

# calculate usable screen size (without borders and gaps)
SW=$((SW - GAP - 2*BW))
SH=$((SH - GAP - PANEL))

MASTER=$((SW / 2))

Y=$((0 + GAP + PANEL))

if [ "$WINNUM" -lt 2 ]; then 	# Maximize and center window if there's no window on screen
	wtp "$GAP" "$Y" "$((SW - GAP - 2*BW))" "$((SH - GAP - 2*BW))" $PFW
else
	# put current window in master area
	wtp "$GAP" "$Y" "$((MASTER - GAP - 2*BW))" "$((SH - GAP - 2*BW))" "$PFW"

	# and now, stack up all remaining windows on the right
	X=$((MASTER + GAP))
	W=$((SW - MASTER - GAP))
	H=$((SH / MAX - GAP - 2*BW))

	for wid in $(lsw|grep -v $PFW); do
	    wtp "$X" "$Y" "$W" "$H" "$wid"
	    Y=$((Y + H + GAP + 2*BW))
	done
fi
