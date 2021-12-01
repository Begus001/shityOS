#!/bin/bash

./debug.sh

gdb -ex "lay asm" -ex "lay regs" -ex "focus cmd" -ex "set confirm off" -ex "set pagination off" -ex "target remote :1234" -ex "file ../build/kernel.elf" -ex "b $1" -ex "c"
pkill qemu
