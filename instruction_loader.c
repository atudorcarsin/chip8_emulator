#include "instruction_loader.h"
#include <stdio.h>
#include "chip8.h"

void loadInstructionsToMemory(const char* filename) {
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error: Could not seek to end of file %s\n", filename);
        fclose(file);
        return;
    }

    const long size = ftell(file);
    rewind(file);
    if (size < 0) {
        fprintf(stderr, "Error: Could not determine size of file %s\n", filename);
        fclose(file);
        return;
    }


    if (size > sizeof(chip8.memory) - 0x200) {
        fprintf(stderr, "Error: file %s is too large to fit in memory\n", filename);
        fclose(file);
        return;
    }

    size_t bytesRead = fread(&chip8.memory[0x200], 1, size, file);
    if (bytesRead != (size_t) size) {
        fprintf(stderr, "Error: Could not read file %s\n", filename);
        fclose(file);
        return;
    }

    fclose(file);
}
