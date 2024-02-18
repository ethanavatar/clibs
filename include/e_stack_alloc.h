#ifndef E_STACK_ALLOC_H
#define E_STACK_ALLOC_H

#include <stdint.h>

struct e_stack_alloc_header {
    size_t capacity;
    size_t size;
    size_t last_alloc_size;
};

#define STACK_ALLOC_SIZE(size) (sizeof(struct e_stack_alloc_header) + size)

void e_stack_alloc_init(void *memory, size_t capacity);
void *e_stack_alloc(void *memory, size_t size);
void e_stack_alloc_free(void *memory, void *ptr);

#endif // !E_STACK_ALLOC_H

#ifdef E_STACK_ALLOC_IMPLEMENTATION

void e_stack_alloc_init(void *memory, size_t capacity) {
    *(struct e_stack_alloc_header *) memory = (struct e_stack_alloc_header) { capacity, 0, 0 };
}

void *e_stack_alloc(void *memory, size_t size) {
    struct e_stack_alloc_header *header = (struct e_stack_alloc_header *) memory;
    if (header->size + size > header->capacity) {
        return NULL;
    }
    void *ptr = (char*) memory + STACK_ALLOC_SIZE(header->size);
    header->size += size;

    return ptr;
}

void e_stack_alloc_free(void *memory, void *ptr) {
    struct e_stack_alloc_header *header = (struct e_stack_alloc_header *) memory;
    if (ptr == NULL) {
        return;
    }
    if (ptr == (char*) memory + STACK_ALLOC_SIZE(header->size)) {
        header->size -= header->last_alloc_size;
    }
}

#endif // !E_STACK_ALLOC_IMPLEMENTATION

