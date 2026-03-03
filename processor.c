#include "processor.h"
#include "chip8.h"
#include "stack.h"
#include <string.h>

uint16_t instruction;
struct Instructions instructions;
bool shiftUsesVy = false;

void fetch() {
    instruction = chip8.memory[chip8.programCounter] << 8 | chip8.memory[chip8.programCounter + 1];
    chip8.programCounter += 2;
}

void decode() {
    instructions.opcode = (instruction & 0xF000) >> 12;
    instructions.x = (instruction & 0x0F00) >> 8;
    instructions.y = (instruction & 0x00F0) >> 4;
    instructions.n = instruction & 0x000F;
    instructions.nn = instruction & 0x00FF;
    instructions.nnn = instruction & 0x0FFF;
}

void execute() {
    switch (instructions.opcode) {
        case 0x0:
            if (instructions.nnn == 0x0E0) {
                memset(chip8.display, 0, sizeof(chip8.display));
            }
            else if (instructions.nnn == 0x0EE) {
                chip8.programCounter = stack.pop();
            }
            break;
        case 0x1:
            chip8.programCounter = instructions.nnn;
            break;
        case 0x2:
            stack.push(chip8.programCounter);
            chip8.programCounter = instructions.nnn;
            break;
        case 0x3:
            if (chip8.generalPurposeRegisters[instructions.x] == instructions.nn) {
                chip8.programCounter += 2;
            }
            break;
        case 0x4:
            if (chip8.generalPurposeRegisters[instructions.x] != instructions.nn) {
                chip8.programCounter += 2;
            }
            break;
        case 0x5:
            if (chip8.generalPurposeRegisters[instructions.x] == chip8.generalPurposeRegisters[instructions.y]) {
                chip8.programCounter += 2;
            }
            break;
        case 0x6:
            chip8.generalPurposeRegisters[instructions.x] = instructions.nn;
            break;
        case 0x7:
            chip8.generalPurposeRegisters[instructions.x] += instructions.nn;
            break;
        case 0x8:
            if (instructions.n == 0x0) {
                chip8.generalPurposeRegisters[instructions.x] = chip8.generalPurposeRegisters[instructions.y];
            }
            else if (instructions.n == 0x1) {
               chip8.generalPurposeRegisters[instructions.x] = chip8.generalPurposeRegisters[instructions.x] | chip8.generalPurposeRegisters[instructions.y];
            }
            else if (instructions.n == 0x2) {
                chip8.generalPurposeRegisters[instructions.x] = chip8.generalPurposeRegisters[instructions.x] & chip8.generalPurposeRegisters[instructions.y];
            }
            else if (instructions.n == 0x3) {
                chip8.generalPurposeRegisters[instructions.x] = chip8.generalPurposeRegisters[instructions.x] ^ chip8.generalPurposeRegisters[instructions.y];
            }
            else if (instructions.n == 0x4) {
                uint16_t sum = chip8.generalPurposeRegisters[instructions.x] + chip8.generalPurposeRegisters[instructions.y];
                chip8.generalPurposeRegisters[instructions.x] = sum;
                if (sum > 0xFF) {
                    chip8.VCarryFlags[instructions.x] = true;
                }
                else {
                    chip8.VCarryFlags[instructions.x] = false;
                }
            }
            else if (instructions.n == 0x5) {
                if (chip8.generalPurposeRegisters[instructions.x] > chip8.generalPurposeRegisters[instructions.y]) {
                    chip8.VCarryFlags[instructions.x] = true;
                }
                else {
                    chip8.VCarryFlags[instructions.x] = false;
                }
                chip8.generalPurposeRegisters[instructions.x] = chip8.generalPurposeRegisters[instructions.x] - chip8.generalPurposeRegisters[instructions.y];
            }
            else if (instructions.n == 0x6 || instructions.n == 0xE) {
                if (shiftUsesVy) {
                    chip8.generalPurposeRegisters[instructions.x] = chip8.generalPurposeRegisters[instructions.y];
                }
                if (instructions.n == 0x6) {
                    uint8_t shiftedOutBit = chip8.generalPurposeRegisters[instructions.x] & 0x1;
                    chip8.generalPurposeRegisters[instructions.x] >>= 1;
                    chip8.VCarryFlags[instructions.x] = shiftedOutBit;
                }
                else {
                    uint8_t shiftedOutBit = (chip8.generalPurposeRegisters[instructions.x] & 0x80) >> 7;
                    chip8.generalPurposeRegisters[instructions.x] <<= 1;
                    chip8.VCarryFlags[instructions.x] = shiftedOutBit;
                }
            }
            else if (instructions.n == 0x7) {
                if (chip8.generalPurposeRegisters[instructions.y] > chip8.generalPurposeRegisters[instructions.x]) {
                    chip8.VCarryFlags[instructions.x] = true;
                }
                else {
                    chip8.VCarryFlags[instructions.x] = false;
                }
                chip8.generalPurposeRegisters[instructions.x] = chip8.generalPurposeRegisters[instructions.y] - chip8.generalPurposeRegisters[instructions.x];
            }
            break;


        case 0x9:
            if (chip8.generalPurposeRegisters[instructions.x] != chip8.generalPurposeRegisters[instructions.y]) {
                chip8.programCounter += 2;
            }
            break;
    }

}