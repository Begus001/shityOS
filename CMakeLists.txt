cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_C_COMPILER $ENV{HOME}/os/gcc-cross-i686/bin/i686-elf-gcc)

set(CMAKE_BUILD_TYPE Debug)

project(kernel LANGUAGES C ASM_NASM)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(ARCH x86)

set(BIN_NAME kernel.elf)

file(GLOB LINKERSCRIPT ${CMAKE_SOURCE_DIR}/kernel/kernel.ld)

set(C_FLAGS -ffreestanding -O0 -Wall -Wextra -nostdlib)
set(LD_FLAGS -T ${LINKERSCRIPT} -ffreestanding -O0 -nostdlib)

add_executable(${BIN_NAME}
        kernel/arch/x86/boot/boot.asm
        kernel/init.c
        kernel/tty.c
        kernel/include/tty/tty.h
        kernel/include/tty/vga.h
        kernel/string.c
        kernel/include/string.h
        kernel/util.c
        kernel/include/util.h
        kernel/debug/serial.c
        kernel/include/debug/serial.h
        )

target_compile_options(${BIN_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>
        )

target_link_options(${BIN_NAME} PRIVATE
        ${LD_FLAGS})

target_include_directories(${BIN_NAME} PRIVATE
        kernel/include
        )

add_custom_target(run
        DEPENDS ${BIN_NAME}
        COMMENT "Running kernel..."
        COMMAND qemu-system-i386 -kernel ${BIN_NAME}
        USES_TERMINAL
        )

add_custom_target(debug
        DEPENDS ${BIN_NAME}
        COMMENT "Running kernel in debug mode..."
        COMMAND qemu-system-i386 -kernel ${BIN_NAME} -s -S
        USES_TERMINAL
        )
