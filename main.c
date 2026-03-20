#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "font.h"
#include "chip8.h"
#include "processor.h"
#include "instruction_loader.h"

void init();
void shutdown();

// Change this to load a different program
const char* INSTRUCTION_FILE = "Maze [David Winter, 199x].ch8";

const uint8_t DISPLAY_SCALE = 10;
const uint32_t FRAME_TIME_MS = (uint32_t) (1000.0f / 60.0f);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int main(void) {
    init();

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        uint32_t startTime = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_0:
                        chip8.keypad[0x0] = 1;
                        break;
                    case SDLK_1:
                        chip8.keypad[0x1] = 1;
                        break;
                    case SDLK_2:
                        chip8.keypad[0x2] = 1;
                        break;
                    case SDLK_3:
                        chip8.keypad[0x3] = 1;
                        break;
                    case SDLK_4:
                        chip8.keypad[0x4] = 1;
                        break;
                    case SDLK_5:
                        chip8.keypad[0x5] = 1;
                        break;
                    case SDLK_6:
                        chip8.keypad[0x6] = 1;
                        break;
                    case SDLK_7:
                        chip8.keypad[0x7] = 1;
                        break;
                    case SDLK_8:
                        chip8.keypad[0x8] = 1;
                        break;
                    case SDLK_9:
                        chip8.keypad[0x9] = 1;
                        break;
                    case SDLK_a:
                        chip8.keypad[0xA] = 1;
                        break;
                    case SDLK_b:
                        chip8.keypad[0xB] = 1;
                        break;
                    case SDLK_c:
                        chip8.keypad[0xC] = 1;
                        break;
                    case SDLK_d:
                        chip8.keypad[0xD] = 1;
                        break;
                    case SDLK_e:
                        chip8.keypad[0xE] = 1;
                        break;
                    case SDLK_f:
                        chip8.keypad[0xF] = 1;
                        break;
                    default:
                        break;
                }
            }
            else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_0:
                        chip8.keypad[0x0] = 0;
                        break;
                    case SDLK_1:
                        chip8.keypad[0x1] = 0;
                        break;
                    case SDLK_2:
                        chip8.keypad[0x2] = 0;
                        break;
                    case SDLK_3:
                        chip8.keypad[0x3] = 0;
                        break;
                    case SDLK_4:
                        chip8.keypad[0x4] = 0;
                        break;
                    case SDLK_5:
                        chip8.keypad[0x5] = 0;
                        break;
                    case SDLK_6:
                        chip8.keypad[0x6] = 0;
                        break;
                    case SDLK_7:
                        chip8.keypad[0x7] = 0;
                        break;
                    case SDLK_8:
                        chip8.keypad[0x8] = 0;
                        break;
                    case SDLK_9:
                        chip8.keypad[0x9] = 0;
                        break;
                    case SDLK_a:
                        chip8.keypad[0xA] = 0;
                        break;
                    case SDLK_b:
                        chip8.keypad[0xB] = 0;
                        break;
                    case SDLK_c:
                        chip8.keypad[0xC] = 0;
                        break;
                    case SDLK_d:
                        chip8.keypad[0xD] = 0;
                        break;
                    case SDLK_e:
                        chip8.keypad[0xE] = 0;
                        break;
                    case SDLK_f:
                        chip8.keypad[0xF] = 0;
                        break;
                    default:
                        break;
                }
            }
        }

        fetch();
        decode();
        execute();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 32; j++) {
                if (chip8.display[i][j]) {
                    SDL_Rect pixel = {
                        i * DISPLAY_SCALE,
                        j * DISPLAY_SCALE,
                        DISPLAY_SCALE,
                        DISPLAY_SCALE
                    };
                    SDL_RenderFillRect(renderer, &pixel);
                }
            }
        }

        SDL_RenderPresent(renderer);

        uint32_t frameTime = SDL_GetTicks() - startTime;
        if (frameTime < FRAME_TIME_MS) {
            SDL_Delay(FRAME_TIME_MS - frameTime);
        }
    }

    shutdown();

    return 0;
}

void init() {
    chip8.programCounter = 0x200;

    loadFontToMemory(chip8.memory);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not load. SDL_Error: %s\n", SDL_GetError());
        exit(-1);
    }

    window = SDL_CreateWindow("Chip8_Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64 * DISPLAY_SCALE, 32 * DISPLAY_SCALE, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
        exit(-2);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created. SDL_Error: %s\n", SDL_GetError());
        exit(-3);
    }

    loadInstructionsToMemory(INSTRUCTION_FILE);
}

void shutdown() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}