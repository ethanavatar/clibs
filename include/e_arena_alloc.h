#ifndef E_ARENA_ALLOC_H
#define E_ARENA_ALLOC_H

#include <stdint.h>

struct e_arena_header {
    size_t capacity;
    size_t size;
};

#define ARENA_ALLOC_SIZE(size) (sizeof(e_arena_header) + size)

void e_arena_init(void *memory, size_t capacity);
void *e_arena_alloc(void *memory, size_t size);

#endif // !E_ARENA_ALLOC_H

#ifdef E_ARENA_ALLOC_IMPLEMENTATION

void e_arena_init(void *memory, size_t capacity) {
    *(e_arena_header*) memory = (e_arena_header) { capacity, 0 };
}

void *e_arena_alloc(void *memory, size_t size) {
    e_arena_header* header = (e_arena_header*) memory;
    size_t new_size = header->size + size;
    if (new_size > header->capacity) {
        return NULL;
    }
    void* result = (uint8_t*) memory + ARENA_ALLOC_SIZE(header->size);
    header->size = new_size;
    return result;
}

#endif // !E_ARENA_ALLOC_IMPLEMENTATION

