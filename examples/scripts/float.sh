#!/bin/sh
#
# Float.sh: floating layout
# Ian LeCorbeau - 2021 (c) wtfpl

# Parsing some config variables from sxwmrc
CONFIG=.config/sxwm/sxwmrc
BAR=$(grep "BARPADDING" "$CONFIG" | cut -f2 -d '=')

PFW=$(pfw)
BW=$(wattr b "$PFW")
WW=$(wattr w "$PFW")
WH=$(wattr h "$PFW")

ROOT=$(lsw -r)
SW=$(wattr w "$ROOT")
SH=$(wattr h "$ROOT")

# Making sure the window size doesn't exceed the size of the screen
if [ "$WW" -gt "$SW" ] || [ "$WH" -gt "$SH" ]; then
	W="$((SW - 2*BW))"
	H="$((SH - BAR - 2*BW))"
else
	W=700
	H=460
fi

Y="$BAR"

wtp 0 "$Y" "$W" "$H" "$PFW"
