cmake_minimum_required(VERSION 4.1)
project(chip8_emulator C)

set(CMAKE_C_STANDARD 99)

find_package(SDL2 REQUIRED)

add_executable(chip8_emulator main.c
        font.h
        font.c
        processor.c
        processor.h
        chip8.c
        chip8.h
        stack.c
        stack.h)

target_link_libraries(chip8_emulator PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_include_directories(chip8_emulator PRIVATE ${SDL2_INCLUDE_DIRS})