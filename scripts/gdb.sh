#!/bin/bash

./debug.sh

gdb -ex "lay asm" -ex "lay regs" -ex "set confirm off" -ex "target remote :1234" -ex "file ../build/kernel.elf"
