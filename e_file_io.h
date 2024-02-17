#ifndef E_FILE_IO_H
#define E_FILE_IO_H

#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

enum EIOErrorType {
    IO_OK,
    IO_FILE_INVALID,
    IO_FILE_READ_ERROR,
    IO_FILE_WRITE_ERROR,
    IO_FILE_TOO_LARGE
};

struct EIOError {
    enum EIOErrorType type;
    char *message;
};

struct EIOError eio_file_size(FILE *file, size_t *out_size);
struct EIOError eio_file_read_all(FILE *file, char *out_buffer, size_t file_size);
struct EIOError eio_file_write_all(FILE *file, char *buffer, size_t buffer_size);

#endif // !E_FILE_IO_H

#ifdef E_FILE_IO_IMPLEMENTATION
#define E_FILE_IO_IMPLEMENTATION

struct EIOError eio_file_size(FILE *file, size_t *out_size) {
    if (file == NULL) {
        return (struct EIOError) { IO_FILE_INVALID, "File is NULL" };
    }

    int32_t descriptor = fileno(file);
    struct stat file_stat;
    if (fstat(descriptor, &file_stat) != 0) {
        return (struct EIOError) { IO_FILE_INVALID, "Failed to get file size" };
    }

    size_t file_size = file_stat.st_size;
    if (file_size > UINT64_MAX) {
        char *msg = "";
        sprintf(msg, "File size exceeds UINT64_MAX (%zu bytes)", UINT64_MAX);
        return (struct EIOError) { IO_FILE_TOO_LARGE, msg };
    }

    *out_size = file_stat.st_size + 1;

    return (struct EIOError) { IO_OK, NULL };
}

struct EIOError eio_file_read_all(FILE *file, char *out_buffer, size_t file_size) {
    if (file == NULL) {
        return (struct EIOError) { IO_FILE_INVALID, "File is NULL" };
    }
    if (file_size > UINT64_MAX) {
        char *msg = "";
        sprintf(msg, "File size exceeds UINT64_MAX (%zu bytes)", UINT64_MAX);
        return (struct EIOError) { IO_FILE_TOO_LARGE, msg };
    }

    rewind(file);

    size_t read = fread(out_buffer, 1, file_size, file);
    if (read != file_size - 1) {
        return (struct EIOError) { IO_FILE_READ_ERROR, "Read %zu bytes, expected %zu" };
    }

    out_buffer[file_size] = '\0';

    return (struct EIOError) { IO_OK, NULL };
}

struct EIOError eio_file_write_all(FILE *file, char *buffer, size_t buffer_size) {
    if (file == NULL) {
        return (struct EIOError) { IO_FILE_INVALID, "File is NULL" };
    }
    if (buffer_size > UINT64_MAX) {
        char *msg = "";
        sprintf(msg, "Buffer size exceeds UINT64_MAX (%zu bytes)", UINT64_MAX);
        return (struct EIOError) { IO_FILE_TOO_LARGE, msg };
    }

    size_t written = fwrite(buffer, 1, buffer_size, file);
    if (written != buffer_size) {
        return (struct EIOError) { IO_FILE_WRITE_ERROR, "Wrote %zu bytes, expected %zu" };
    }

    return (struct EIOError) { IO_OK, NULL };
}

#endif // !E_FILE_IO_IMPLEMENTATION

