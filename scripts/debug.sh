#!/bin/bash

/usr/bin/qemu-system-i386 -kernel ../build/kernel.elf -s -S -serial stdio > serial.log &

