#ifndef TEST_ARENA_ALLOC_H
#define TEST_ARENA_ALLOC_H

#include "unity.h"

#define E_ARENA_ALLOC_IMPLEMENTATION
#include "e_arena_alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data {
    int a;
    int b;
};

static const size_t arena_size = 2;
// allocate enough memory for the header and 2 elements of struct Data
static uint8_t arena[ARENA_ALLOC_SIZE(arena_size * sizeof(struct Data))];

void test_arena_alloc(void);

#define RUN_TEST_ARENA_ALLOC() \
    RUN_TEST(test_arena_alloc);

#endif // !TEST_FILE_IO_H

#ifdef TEST_ARENA_ALLOC_IMPLEMENTATION

void test_arena_alloc(void) {

    e_arena_init(arena, arena_size * sizeof(struct Data));
    struct e_arena_header *header = (struct e_arena_header *) arena;
    TEST_ASSERT_NOT_NULL(header);
    TEST_ASSERT_EQUAL_INT(e_arena_capacity(arena), header->capacity);

    struct Data *data1 = e_arena_alloc(arena, sizeof(struct Data));
    TEST_ASSERT_NOT_NULL(data1);
    TEST_ASSERT_EQUAL_INT(sizeof(struct Data), e_arena_size(arena));

    struct Data *data2 = e_arena_alloc(arena, sizeof(struct Data));
    TEST_ASSERT_NOT_NULL(data2);
    TEST_ASSERT_EQUAL_INT(sizeof(struct Data) * 2, e_arena_size(arena));

    struct Data *data3 = e_arena_alloc(arena, sizeof(struct Data));
    TEST_ASSERT_NULL(data3);
    TEST_ASSERT_EQUAL_INT(sizeof(struct Data) * 2, e_arena_size(arena));

    e_arena_clear(arena);
    TEST_ASSERT_EQUAL_INT(0, e_arena_size(arena));
    TEST_ASSERT_EQUAL_INT(arena_size * sizeof(struct Data), e_arena_capacity(arena));
}

#endif // !TEST_ARENA_ALLOC_IMPLEMENTATION
