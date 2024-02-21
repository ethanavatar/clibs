#include "unity.h"

#define TEST_FILE_IO_IMPLEMENTATION
#include "test_file_io.h"

#define TEST_ARENA_ALLOC_IMPLEMENTATION
#include "test_arena_alloc.h"

void setUp(void) { }
void tearDown(void) { }

int main(void) {
    UNITY_BEGIN();
        RUN_TEST_FILE_IO();
        RUN_TEST_ARENA_ALLOC();
    return UNITY_END();
}

