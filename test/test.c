#include "unity.h"

#define E_FILE_IO_IMPLEMENTATION
#include "e_file_io.h"

#include <stdio.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

void test_get_current_dir(void) {
    char cwd[512] = { 0 };
    struct EIOResult result = e_get_current_directory(cwd, sizeof(cwd));

    TEST_ASSERT_EQUAL_INT(IO_OK, result.type);
    TEST_ASSERT_GREATER_THAN(0, strlen(cwd));
}

void test_file_size_valid(void) {
    FILE *file = fopen("include/e_file_io.h", "rb");
    TEST_ASSERT_NOT_NULL(file);

    size_t size = 0;
    struct EIOResult result = e_file_size(file, &size);

    TEST_ASSERT_EQUAL_INT(IO_OK, result.type);
    TEST_ASSERT_GREATER_THAN(0, size);

    fclose(file);
}

void test_file_size_invalid(void) {
    FILE *file = NULL;

    size_t size = 0;
    struct EIOResult result = e_file_size(file, &size);

    TEST_ASSERT_EQUAL_INT(IO_FILE_INVALID, result.type);
    TEST_ASSERT_EQUAL_INT(0, size);
}

int main() {
    UNITY_BEGIN();
        RUN_TEST(test_get_current_dir);
        RUN_TEST(test_file_size_valid);
        RUN_TEST(test_file_size_invalid);
    return UNITY_END();
}

