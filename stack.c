#include "stack.h"
#include "chip8.h"
#include <stddef.h>

void push(uint16_t value) {
    chip8.stack[chip8.stackPointer++] = value;
}

uint16_t pop() {
    return chip8.stack[--chip8.stackPointer];
}

uint16_t peek() {
    return chip8.stack[chip8.stackPointer - 1];
}

const struct Stack stack = {
    .push = push,
    .pop = pop,
    .peek = peek
};