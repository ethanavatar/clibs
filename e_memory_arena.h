#ifndef E_MEMORY_ARENA_H
#define E_MEMORY_ARENA_H

#include <stdint.h>

struct e_arena_header {
    size_t capacity;
    size_t size;
};

#define ARENA_SIZE(size) (sizeof(e_arena_header) + size)

void e_arena_init(void *memory, size_t size);
void* e_arena_alloc(void *memory, size_t size);

#endif // !E_MEMORY_ARENA_H

#ifdef E_MEMORY_ARENA_IMPLEMENTATION

void e_arena_init(void *memory, size_t size) {
    *(e_arena_header*) memory = (e_arena_header) { size, 0 };
}

void* e_arena_alloc(void *memory, size_t size) {
    e_arena_header* header = (e_arena_header*) memory;
    size_t new_size = header->size + size;
    if (new_size > header->capacity) {
        return NULL;
    }
    void* result = (uint8_t*) memory + ARENA_SIZE(header->size);
    header->size = new_size;
    return result;
}

#endif // !E_MEMORY_ARENA_IMPLEMENTATION
