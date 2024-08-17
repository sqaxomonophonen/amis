#!/bin/sh
set -e
if [ -z "$1" ] ; then
	echo "Usage: $0 <name>"
	exit 1
fi
fs-uae --warp-mode=1 --keyboard-input-grab=0 --window-resizable=1 --kickstart-file=./roms/kickstart13.rom --floppy-drive-0=$1.adf
