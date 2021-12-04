#!/bin/bash
cd "$(dirname "$0")"
/usr/bin/qemu-system-i386 -kernel ../build/kernel.elf -serial stdio

