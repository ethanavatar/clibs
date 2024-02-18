/// NOTE: Still not sure if I want to do this kind of thing
#ifndef E_ALLOCATOR_H
#define E_ALLOCATOR_H

#include <stdint.h>

struct e_allocator {
    void *memory[static 1];
    void *(*alloc)(void *memory, size_t size));
    void (*free)(void *memory, void *ptr);
};

#define NEW_ALLOCATOR(memory, alloc, free) ((struct e_allocator) { memory, alloc, free })

#endif // !E_ALLOCATOR_H

