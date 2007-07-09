#!/bin/sh

dd if=/dev/zero of=$1 bs=18k count=80 1>/dev/null 2>&1
echo "drive u: file=\"$1\" 1.44M filter" > mtoolsrc
export MTOOLSRC=mtoolsrc
mformat U:
mmd U:/boot
mmd U:/boot/grub
mcopy /boot/grub/stage1 U:/boot/grub/
mcopy /boot/grub/stage2 U:/boot/grub/
echo "timeout 0 
default 0
title  akernel
root   (fd0)
kernel /system/$2" >menu.lst
mcopy -bo menu.lst U:/boot/grub/
mmd U:/system
mcopy -bo $2 U:/system/
grub --batch <<EOT 1>/dev/null 2>/dev/null || exit 1
device (fd0) $1
install (fd0)/boot/grub/stage1 (fd0) (fd0)/boot/grub/stage2 p (fd0)/boot/grub/menu.lst
quit

