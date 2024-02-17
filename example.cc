#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Must compile as C++ to use DEFER
// otherwise, a preprocessor error will occur
#include "e_defer.hh"

#define E_FILE_IO_IMPLEMENTATION
#include "e_file_io.h"

#define E_MEMORY_ARENA_IMPLEMENTATION
#include "e_memory_arena.h"

int main(void) {
    FILE *file = fopen("e_file_io.h", "rb");
    DEFER(fclose(file));

    size_t file_size = 0;
    struct EIOResult res = e_file_size(file, &file_size);
    if (res.type != IO_OK) {
        fprintf(stderr, "%s", res.message);
        return EXIT_FAILURE;
    }

    char *file_data = (char *) malloc(file_size);
    DEFER(free(file_data));

    if (file_data == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        return EXIT_FAILURE;
    }

    res = e_file_read(file, file_data, file_size);
    if (res.type != IO_OK) {
        fprintf(stderr, "%s", res.message);
        return EXIT_FAILURE;
    }

    fprintf(stdout, "%s", file_data);
    return EXIT_SUCCESS;
}

