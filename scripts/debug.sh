#!/bin/bash
cd "$(dirname "$0")"
gnome-terminal -- /usr/bin/qemu-system-i386 -kernel ../build/kernel.elf -s -S -serial stdio

