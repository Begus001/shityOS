cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
set(CMAKE_C_COMPILER_WORKS TRUE)

project(kernel LANGUAGES C ASM_NASM)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(ARCH "x86")

set(BIN_NAME kernel.elf)

file(GLOB LINKERSCRIPT "${CMAKE_SOURCE_DIR}/kernel/${ARCH}/kernel.ld")
file(GLOB_RECURSE kernel_SRCS "kernel/${ARCH}/*.c")
file(GLOB_RECURSE kernel_ASMSRCS "kernel/${ARCH}/*.asm")

set(C_FLAGS -g -ffreestanding -O0 -Wall -Wextra -nostdlib)
set(LD_FLAGS -T ${LINKERSCRIPT} -ffreestanding -O0 -nostdlib)

add_executable(${BIN_NAME}
        ${kernel_SRCS}
        ${kernel_ASMSRCS})

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
