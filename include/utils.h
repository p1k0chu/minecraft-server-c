#pragma once

#define check(condition, error_str)       \
    {                                     \
        if (!(condition)) {               \
            fprintf(stderr, (error_str)); \
            exit(1);                      \
        }                                 \
    }

#define STR(s)  #s
#define XSTR(s) STR(s)

#define COOL_PERROR(msg) perror(__FILE_NAME__ ":" XSTR(__LINE__) " " msg)

#define LOG(file, msg) fprintf(file, __FILE_NAME__ ":" XSTR(__LINE__) " " msg "\n");
#define LOGF(file, msg, ...) \
    fprintf(file, __FILE_NAME__ ":" XSTR(__LINE__) " " msg "\n", __VA_ARGS__);

#define ERROR(msg)        \
    {                     \
        COOL_PERROR(msg); \
        exit(1);          \
    }

#define COPY_STR(dst, src)                  \
    {                                       \
        const size_t len = strlen((src));   \
        (dst)            = malloc(len + 1); \
        if (!(dst)) ERROR("malloc");        \
        memcpy((dst), (src), len);          \
        (dst)[len] = 0;                     \
    }


