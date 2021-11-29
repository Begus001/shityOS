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

set(KERNEL_C_FLAGS -ffreestanding -O0 -Wall -Wextra -nostdlib -masm=intel -std=gnu11 -ggdb)
set(KERNEL_LD_FLAGS -T${KERNEL_LINKER} -ffreestanding -O0 -nostdlib -lgcc -ggdb)

add_executable(${KERNEL_ELF}
		kernel/impl/boot/init.c
		kernel/impl/mm/string.c
		kernel/impl/boot/boot.asm
		kernel/impl/io/serial.c
		kernel/impl/intr/gdt.c
		kernel/impl/tty/tty.c
		kernel/impl/intr/idt.c
		kernel/impl/intr/pic.c
		kernel/impl/intr/handler.c
		kernel/impl/intr/exceptions.asm
		kernel/impl/mm/pmm.c
		kernel/impl/mm/vmm.c
		kernel/impl/mm/heap.c
		kernel/impl/io/keyboard.c
		kernel/impl/task/task.c
		kernel/impl/task/task.asm
		kernel/impl/intr/intr.asm
		kernel/include/def/err.h
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
