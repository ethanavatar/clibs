#ifndef E_BLOCK_ALLOC_H
#define E_BLOCK_ALLOC_H

#include <stdint.h>

struct e_free_block {
    struct e_free_block* next;
};

struct e_block_alloc_header {
    size_t capacity;
    size_t block_size; // bytes
    size_t block_count;
    struct e_free_block* free;
};

#define BLOCK_ALLOC_SIZE(size) (sizeof(struct e_block_alloc_header) + size)

void e_block_alloc_init(void *memory, size_t capacity, size_t block_size);
void *e_block_alloc(void *memory, size_t size);
void e_block_free(void *memory, void *block);

#endif // !E_BLOCK_ALLOC_H

#ifdef E_BLOCK_ALLOC_IMPLEMENTATION

void e_block_alloc_init(void *memory, size_t capacity, size_t block_size) {
    *(struct e_block_alloc_header *) memory = (struct e_block_alloc_header)
        { capacity, block_size, 0, NULL };
}

void* e_block_alloc(void *memory, size_t size) {
    struct e_block_alloc_header *header = (struct e_block_alloc_header*) memory;
    if (header->block_size < size) {
        return NULL;
    }

    struct e_free_block *free_block = header->free;
    if (free_block) {
        header->free = free_block->next;
        return free_block;
    }

    size_t new_block_count = header->block_count + 1;
    if (new_block_count * header->block_size > header->capacity) {
        return NULL;
    }

    header->block_count = new_block_count;
    uint32_t block_offset = header->block_size * new_block_count;
    return (uint8_t*) memory + BLOCK_ALLOC_SIZE(block_offset);
}

void e_block_free(void *memory, void *block) {
    struct e_block_alloc_header *header = (struct e_block_alloc_header *) memory;
    struct e_free_block *free_block = (struct e_free_block *) block;
    free_block->next = header->free;
    header->free = free_block;
}

#endif // !E_BLOCK_ALLOC_IMPLEMENTATION

