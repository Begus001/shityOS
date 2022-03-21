cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_C_COMPILER "$ENV{HOME}/opt/gcc-cross-i686/bin/i686-elf-gcc")

set(CMAKE_BUILD_TYPE Debug)

project(kernel LANGUAGES C ASM_NASM)

# DEBUG FLAGS #

#add_compile_definitions(DBG_INTR)
#add_compile_definitions(DBG_TTY)
#add_compile_definitions(DBG_VMM)


set(CMAKE_VERBOSE_MAKEFILE ON)  # Does not work for ninja

set(KERNEL_ELF kernel.elf)

set(KERNEL_LINKER "${CMAKE_SOURCE_DIR}/kernel/kernel.ld")

set(KERNEL_C_FLAGS -ffreestanding -O0 -Wall -Wextra -nostdlib -masm=intel -std=gnu11 -pedantic
		-ggdb)
set(KERNEL_LD_FLAGS -T${KERNEL_LINKER} -ffreestanding -O0 -nostdlib -lgcc -ggdb)

add_executable(${KERNEL_ELF}
		kernel/src/boot/init.c
		kernel/src/mm/string.c
		kernel/src/boot/boot.asm
		kernel/src/io/serial.c
		kernel/src/mm/gdt.c
		kernel/src/tty/tty.c
		kernel/src/intr/idt.c
		kernel/src/intr/pic.c
		kernel/src/intr/handler.c
		kernel/src/intr/exceptions.asm
		kernel/src/mm/pmm.c
		kernel/src/mm/vmm.c
		kernel/src/mm/heap.c
		kernel/src/io/keyboard.c
		kernel/src/task/task.c
		kernel/src/intr/intr.asm
		kernel/include/def/assert.h
		)

target_compile_options(${KERNEL_ELF} PRIVATE
		$<$<COMPILE_LANGUAGE:C>:${KERNEL_C_FLAGS}>
		)

target_link_options(${KERNEL_ELF} PRIVATE
		${KERNEL_LD_FLAGS}
		)

target_include_directories(${KERNEL_ELF} PRIVATE
		kernel/include
		)

add_custom_command(TARGET ${KERNEL_ELF} POST_BUILD
		COMMAND mkdir -p sysroot/boot/grub
		COMMAND cp ${CMAKE_SOURCE_DIR}/grub.cfg sysroot/boot/grub
		COMMAND cp ${KERNEL_ELF} sysroot/boot
		COMMAND grub-mkrescue -o kernel.iso sysroot
		)
