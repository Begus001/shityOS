cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
set(CMAKE_C_COMPILER_WORKS TRUE)
#set(CMAKE_C_COMPILER "$ENV{HOME}/os/gcc-cross-i686/bin/i686-elf-gcc")

set(CMAKE_BUILD_TYPE Debug)

project(kernel LANGUAGES C ASM_NASM)

set(ARCH x86)
add_compile_definitions(${ARCH})  # Set arch for preprocessor


# DEBUG FLAGS #

#add_compile_definitions(DBG_INTR)
#add_compile_definitions(DBG_TTY)


set(CMAKE_VERBOSE_MAKEFILE ON)  # Does not work for ninja

set(KERNEL_ELF kernel.elf)

if (${ARCH} MATCHES x86)  # Select arch linkerscript

	message(STATUS "Generating x86 files")
	set(KERNEL_LINKER "${CMAKE_SOURCE_DIR}/kernel/arch/x86/kernel.ld")

endif ()

set(KERNEL_C_FLAGS -ffreestanding -O0 -Wall -Wextra -nostdlib -masm=intel -std=gnu11 -ggdb)
set(KERNEL_LD_FLAGS -T${KERNEL_LINKER} -ffreestanding -O0 -nostdlib -ggdb)

add_executable(${KERNEL_ELF}
		kernel/init.c
		kernel/include/tty/tty.h
		kernel/string.c
		kernel/include/sys/string.h
		kernel/include/tty/serial.h
		kernel/include/sys/def/int.h
		kernel/include/intr/intr.h
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

if (${ARCH} MATCHES x86)  # Select arch source files

	target_sources(${KERNEL_ELF} PRIVATE
			kernel/arch/x86/boot/boot.asm
			kernel/arch/x86/tty/serial.c
			kernel/arch/x86/gdt/gdt.c
			kernel/arch/x86/tty/tty.c
			kernel/arch/x86/intr/idt.c
			kernel/arch/x86/intr/pic.c
			kernel/arch/x86/intr/handler.c
			kernel/arch/x86/intr/intr_stub.asm
			kernel/arch/x86/mm/pmm.c
			)

	target_include_directories(${KERNEL_ELF} PRIVATE
			kernel/arch/x86
			)

endif ()

add_custom_target(run
		DEPENDS ${KERNEL_ELF}
		COMMENT "Running kernel..."
		COMMAND qemu-system-i386 -kernel ${KERNEL_ELF}
		USES_TERMINAL
		)

add_custom_target(debug
		DEPENDS ${KERNEL_ELF}
		COMMENT "Running kernel in debug mode..."
		COMMAND gnome-terminal -- gdb
		COMMAND qemu-system-i386 -kernel ${KERNEL_ELF} -s -S
		USES_TERMINAL
		)
