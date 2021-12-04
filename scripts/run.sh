#!/bin/bash
cd "$(dirname "$0")"
./build.sh
printf "\n\n"
/usr/bin/qemu-system-i386 -drive file=../build/kernel.iso,format=raw -serial stdio

