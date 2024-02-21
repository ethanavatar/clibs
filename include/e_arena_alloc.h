#ifndef E_ARENA_ALLOC_H
#define E_ARENA_ALLOC_H

#include <stdint.h>

struct e_arena_header {
    size_t capacity;
    size_t size;
};

#define ARENA_ALLOC_SIZE(size) (sizeof(struct e_arena_header) + (size))

void e_arena_init(void *memory, size_t capacity);
void *e_arena_alloc(void *memory, size_t size);
void e_arena_clear(void *memory);

size_t e_arena_size(void *memory);
size_t e_arena_capacity(void *memory);

#endif // !E_ARENA_ALLOC_H

#ifdef E_ARENA_ALLOC_IMPLEMENTATION

void e_arena_init(void *memory, size_t capacity) {
    *(struct e_arena_header *) memory = (struct e_arena_header) { capacity, 0 };
}

void *e_arena_alloc(void *memory, size_t size) {
    struct e_arena_header* header = (struct e_arena_header *) memory;
    size_t new_size = header->size + size;
    //printf("new_size: %zu, capacity: %zu\n", new_size, header->capacity);
    if (new_size > header->capacity) {
        return NULL;
    }
    void* result = (uint8_t*) memory + ARENA_ALLOC_SIZE(header->size);
    header->size = new_size;
    return result;
}

void e_arena_clear(void *memory) {
    struct e_arena_header* header = (struct e_arena_header *) memory;
    size_t start = sizeof(struct e_arena_header);
    for (size_t i = start; i < header->size; i++) {
        ((uint8_t *) memory)[i] = 0;
    }
    header->size = 0;
}

size_t e_arena_size(void *memory) {
    return ((struct e_arena_header *) memory)->size;
}

size_t e_arena_capacity(void *memory) {
    return ((struct e_arena_header *) memory)->capacity;
}

#endif // !E_ARENA_ALLOC_IMPLEMENTATION

