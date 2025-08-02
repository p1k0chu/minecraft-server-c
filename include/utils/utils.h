#pragma once

#define check(condition, error_str) \
    {                               \
        if (!(condition)) {          \
            fprintf(stderr, (error_str));\
            exit(1);\
        }                           \
    }

void error(const char *msg);

