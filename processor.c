#include "processor.h"
#include "chip8.h"
#include "stack.h"
#include "font.h"
#include <string.h>
#include <stdlib.h>

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
            if (chip8.V[instructions.x] == instructions.nn) {
                chip8.programCounter += 2;
            }
            break;
        case 0x4:
            if (chip8.V[instructions.x] != instructions.nn) {
                chip8.programCounter += 2;
            }
            break;
        case 0x5:
            if (chip8.V[instructions.x] == chip8.V[instructions.y]) {
                chip8.programCounter += 2;
            }
            break;
        case 0x6:
            chip8.V[instructions.x] = instructions.nn;
            break;
        case 0x7:
            chip8.V[instructions.x] += instructions.nn;
            break;
        case 0x8:
            if (instructions.n == 0x0) {
                chip8.V[instructions.x] = chip8.V[instructions.y];
            }
            else if (instructions.n == 0x1) {
               chip8.V[instructions.x] = chip8.V[instructions.x] | chip8.V[instructions.y];
            }
            else if (instructions.n == 0x2) {
                chip8.V[instructions.x] = chip8.V[instructions.x] & chip8.V[instructions.y];
            }
            else if (instructions.n == 0x3) {
                chip8.V[instructions.x] = chip8.V[instructions.x] ^ chip8.V[instructions.y];
            }
            else if (instructions.n == 0x4) {
                uint16_t sum = chip8.V[instructions.x] + chip8.V[instructions.y];
                chip8.V[instructions.x] = sum;
                if (sum > 0xFF) {
                    chip8.VCarryFlag = true;
                }
                else {
                    chip8.VCarryFlag = false;
                }
            }
            else if (instructions.n == 0x5) {
                if (chip8.V[instructions.x] > chip8.V[instructions.y]) {
                    chip8.VCarryFlag = true;
                }
                else {
                    chip8.VCarryFlag = false;
                }
                chip8.V[instructions.x] = chip8.V[instructions.x] - chip8.V[instructions.y];
            }
            else if (instructions.n == 0x6 || instructions.n == 0xE) {
                if (shiftUsesVy) {
                    chip8.V[instructions.x] = chip8.V[instructions.y];
                }
                if (instructions.n == 0x6) {
                    uint8_t shiftedOutBit = chip8.V[instructions.x] & 0x1;
                    chip8.V[instructions.x] >>= 1;
                    chip8.VCarryFlag = shiftedOutBit;
                }
                else {
                    uint8_t shiftedOutBit = (chip8.V[instructions.x] & 0x80) >> 7;
                    chip8.V[instructions.x] <<= 1;
                    chip8.VCarryFlag = shiftedOutBit;
                }
            }
            else if (instructions.n == 0x7) {
                if (chip8.V[instructions.y] > chip8.V[instructions.x]) {
                    chip8.VCarryFlag = true;
                }
                else {
                    chip8.VCarryFlag = false;
                }
                chip8.V[instructions.x] = chip8.V[instructions.y] - chip8.V[instructions.x];
            }
            break;
        case 0x9:
            if (chip8.V[instructions.x] != chip8.V[instructions.y]) {
                chip8.programCounter += 2;
            }
            break;
        case 0xA:
            chip8.indexRegister = instructions.nnn;
            break;
        case 0xB:
            chip8.programCounter = instructions.nnn + chip8.V[instructions.x];
            break;
        case 0xC:
            chip8.V[instructions.x] = (rand() % 256) & instructions.nn;
            break;
        case 0xD:
            uint8_t xCoordinate = chip8.V[instructions.x] % 64;
            uint8_t yCoordinate = chip8.V[instructions.y] % 32;
            chip8.VCarryFlag = false;

            uint8_t rows = instructions.n;
            for (int i = 0; i < rows && yCoordinate + i < 32; i++) {
                uint8_t spriteData = chip8.memory[chip8.indexRegister + i];
                for (int j = 0; j < 8 && xCoordinate + j < 64; j++) {
                    if (chip8.display[xCoordinate + j][yCoordinate + i] && (spriteData & (0x80 >> j))) {
                        chip8.display[xCoordinate + j][yCoordinate + i] = 0;
                        chip8.VCarryFlag = true;
                    }
                    else if (spriteData & (0x80 >> j)) {
                        chip8.display[xCoordinate + j][yCoordinate + i] = 1;
                    }
                }
            }
            break;
        case 0xE:
            if (instructions.nn == 0x9E) {
                if (chip8.keypad[chip8.V[instructions.x]]) {
                    chip8.programCounter += 2;
                }
            }
            else if (instructions.nn == 0xA1) {
                if (!chip8.keypad[chip8.V[instructions.x]]) {
                    chip8.programCounter += 2;
                }
            }
            break;
        case 0xF:
            if (instructions.nn == 0x07) {
                chip8.V[instructions.x] = chip8.delayTimer;
            }
            else if (instructions.nn == 0x15) {
                chip8.delayTimer = chip8.V[instructions.x];
            }
            else if (instructions.nn == 0x18) {
                chip8.soundTimer = chip8.V[instructions.x];
            }
            else if (instructions.nn == 0x1E) {
                chip8.indexRegister += chip8.V[instructions.x];
            }
            else if (instructions.nn == 0x0A) {
                bool keyPressed = false;
                for (int i = 0; i < 16; i++) {
                    if (chip8.keypad[i]) {
                        chip8.V[instructions.x] = i;
                        keyPressed = true;
                        break;
                    }
                }
                if (!keyPressed) {
                    chip8.programCounter -= 2;
                }
            }
            else if (instructions.nn == 0x29) {
                chip8.indexRegister = FONT_START_ADDRESS + ((chip8.V[instructions.x] & 0xF) * 5);
            }
            else if (instructions.nn == 0x33) {
                uint8_t number = chip8.V[instructions.x];
                chip8.memory[chip8.indexRegister] = (number / 100) % 10;
                chip8.memory[chip8.indexRegister + 1] = (number / 10) % 10;
                chip8.memory[chip8.indexRegister + 2] = number % 10;
            }
            else if (instructions.nn == 0x55) {
                for (int i = 0; i <= instructions.x; i++) {
                    chip8.memory[chip8.indexRegister + i] = chip8.V[i];
                }

            }
            else if (instructions.nn == 0x65) {
                for (int i = 0; i <= instructions.x; i++) {
                    chip8.V[i] = chip8.memory[chip8.indexRegister + i];
                }
            }
            break;
    }

}