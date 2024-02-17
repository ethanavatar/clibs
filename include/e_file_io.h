#ifndef E_FILE_IO_H
#define E_FILE_IO_H

#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

#ifdef _WIN32
#  define fileno _fileno
#  include <direct.h>
#  define getcwd _getcwd
#endif

enum EIOError {
    IO_OK,
    IO_FILE_INVALID,
    IO_FILE_READ_ERROR,
    IO_FILE_WRITE_ERROR,
    IO_FILE_TOO_LARGE
};

struct EIOResult {
    enum EIOError type;
    char *message;
};

struct EIOResult e_get_current_directory(char *out_buffer, size_t bytes);

struct EIOResult e_file_size(FILE *file, size_t *out_size);
struct EIOResult e_file_read(FILE *file, char *out_buffer, size_t bytes);
struct EIOResult e_file_write(FILE *file, char *buffer, size_t bytes);

#endif // !E_FILE_IO_H

#ifdef E_FILE_IO_IMPLEMENTATION

struct EIOResult e_get_current_directory(char *out_buffer, size_t bytes) {
    if (getcwd(out_buffer, bytes) == NULL) {
        return (struct EIOResult)
            { IO_FILE_INVALID, "Failed to get current directory" };
    }

    return (struct EIOResult)
        { IO_OK, NULL };
}

struct EIOResult e_file_size(FILE *file, size_t *out_size) {
    if (file == NULL) {
        return (struct EIOResult)
            { IO_FILE_INVALID, "File is NULL" };
    }

    int32_t descriptor = fileno(file);
    struct stat file_stat;
    if (fstat(descriptor, &file_stat) != 0) {
        return (struct EIOResult)
            { IO_FILE_INVALID, "Failed to get file size" };
    }

    size_t file_size = file_stat.st_size;
    if (file_size > UINT64_MAX) {
        char *msg = "";
        sprintf(msg, "File size exceeds %llu bytes", UINT64_MAX);
        return (struct EIOResult)
            { IO_FILE_TOO_LARGE, msg };
    }

    *out_size = file_stat.st_size + 1;

    return (struct EIOResult)
        { IO_OK, NULL };
}

struct EIOResult e_file_read(FILE *file, char *out_buffer, size_t bytes) {
    if (file == NULL) {
        return (struct EIOResult)
            { IO_FILE_INVALID, "File is NULL" };
    }

    if (bytes > UINT64_MAX) {
        char *msg = "";
        sprintf(msg, "The number of bytes to read exceeds %llu", UINT64_MAX);
        return (struct EIOResult)
            { IO_FILE_TOO_LARGE, msg };
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        return (struct EIOResult)
            { IO_FILE_READ_ERROR, "Failed to seek to the start of the file" };
    }

    size_t read = fread(out_buffer, 1, bytes - 1, file);
    if (read != bytes - 1) {
        char *msg = "";
        sprintf(msg, "Read %zu bytes, expected %zu", read, bytes);
        return (struct EIOResult)
            { IO_FILE_READ_ERROR, msg };
    }

    out_buffer[bytes] = '\0';

    return (struct EIOResult)
        { IO_OK, NULL };
}

struct EIOResult e_file_write(FILE *file, char *buffer, size_t bytes) {
    if (file == NULL) {
        return (struct EIOResult)
            { IO_FILE_INVALID, "File is NULL" };
    }
    if (bytes > UINT64_MAX) {
        char *msg = "";
        sprintf(msg, "The number of bytes to write exceeds %llu", UINT64_MAX);
        return (struct EIOResult)
            { IO_FILE_TOO_LARGE, msg };
    }

    size_t written = fwrite(buffer, 1, bytes, file);
    if (written != bytes) {
        char *msg = "";
        sprintf(msg, "Wrote %zu bytes, expected %zu", written, bytes);
        return (struct EIOResult)
            { IO_FILE_WRITE_ERROR, msg };
    }

    return (struct EIOResult)
        { IO_OK, NULL };
}

#endif // !E_FILE_IO_IMPLEMENTATION

