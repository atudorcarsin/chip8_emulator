#ifndef CHIP8_EMULATOR_FONT_H
#define CHIP8_EMULATOR_FONT_H

#include <stdint.h>

const uint16_t FONT_START_ADDRESS;

const uint8_t font[80];

void loadFontToMemory(uint8_t* memory);

#endif //CHIP8_EMULATOR_FONT_H