#ifndef CHIP8_EMULATOR_STACK_H
#define CHIP8_EMULATOR_STACK_H

#include <stdint.h>

struct Stack {
    void (*push)(uint16_t value);
    uint16_t (*pop)();
    uint16_t (*peek)();
};

void push(uint16_t value);
uint16_t pop();
uint16_t peek();

extern const struct Stack stack;

#endif