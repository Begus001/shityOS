cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
set(CMAKE_C_COMPILER_WORKS TRUE)
#set(CMAKE_C_COMPILER "$ENV{HOME}/os/gcc-cross-i686/bin/i686-elf-gcc")

set(CMAKE_BUILD_TYPE Debug)

project(kernel LANGUAGES C ASM_NASM)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(ARCH x86)

set(KERNEL_ELF kernel.elf)

if (${ARCH} MATCHES x86)

	message(STATUS "Generating x86 files")
	set(KERNEL_LINKER "${CMAKE_SOURCE_DIR}/kernel/arch/x86/kernel.ld")

endif ()

set(KERNEL_C_FLAGS -ffreestanding -O0 -Wall -Wextra -nostdlib -masm=intel)
set(KERNEL_LD_FLAGS -T${KERNEL_LINKER} -ffreestanding -O0 -nostdlib)

add_executable(${KERNEL_ELF}
		kernel/init.c
		kernel/include/tty/tty.h
		kernel/string.c
		kernel/include/sys/string.h
		kernel/include/tty/serial.h
		kernel/include/sys/int.h
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

if (${ARCH} MATCHES x86)

	target_sources(${KERNEL_ELF} PRIVATE
			kernel/arch/x86/boot/boot.asm
			kernel/arch/x86/tty/serial.c
			kernel/arch/x86/descriptor_tables/gdt.c
			kernel/arch/x86/tty/tty.c
			kernel/arch/x86/sys/util.c
			kernel/arch/x86/descriptor_tables/idt.c
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
		COMMAND qemu-system-i386 -kernel ${KERNEL_ELF} -s -S
		USES_TERMINAL
		)
