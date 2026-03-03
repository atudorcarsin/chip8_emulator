#ifndef CHIP8_EMULATOR_PROCESSOR_H
#define CHIP8_EMULATOR_PROCESSOR_H

#include <stdint.h>
#include <stdbool.h>

struct Instructions {
    uint8_t opcode;
    uint8_t x;
    uint8_t y;
    uint8_t n;
    uint8_t nn;
    uint16_t nnn;
};

void fetch();
void decode();
void execute();

uint16_t instruction;
struct Instructions instructions;
bool shiftUsesVy;

#endif