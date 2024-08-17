#!/bin/sh
set -e
if [ -z "$1" ] ; then
	echo "Usage: $0 <name>"
	exit 1
fi

name=$1
asm=$1.asm
disk=$1.adf

if [ ! -e $asm ] ; then
	echo "$asm does not exist, aborting..."
	exit 1
fi

rm -f $name
vasmm68k_mot -kick1hunks -Fhunkexe -o $name -nosym $name.asm
echo "executable size: "
cat $name | wc -c
DISK=$name.adf
rm -f $DISK
xdftool $DISK create
xdftool $DISK format EXAMPLE
xdftool $DISK boot install
xdftool $DISK write $name
xdftool $DISK makedir s
echo $name > startup-sequence
xdftool $DISK write startup-sequence s/startup-sequence
rm -f startup-sequence
