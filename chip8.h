#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H

#include <stdint.h>
#include <stdbool.h>

struct Chip8 {
    uint8_t memory[4096];
    bool display[64][32];
    uint16_t programCounter;
    uint16_t indexRegister;
    uint16_t stack[16];
    uint8_t stackPointer;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t V[16];
    bool VCarryFlag;
    bool keypad[16];
};

extern struct Chip8 chip8;

#endif