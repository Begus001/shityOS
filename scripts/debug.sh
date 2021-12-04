#!/bin/bash
cd "$(dirname "$0")"
./build.sh
gnome-terminal -- /usr/bin/qemu-system-i386 -drive file=../build/kernel.iso,format=raw -s -S -serial stdio

