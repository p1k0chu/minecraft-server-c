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

#define COOL_PERROR(msg) perror(__FILE_NAME__ ":" XSTR(__LINE__) " " msg "\n")

#define ERROR(msg)        \
    {                     \
        COOL_PERROR(msg); \
        exit(1);          \
    }

