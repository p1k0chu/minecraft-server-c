cmake_minimum_required(VERSION 3.31)
project("project3" LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include_directories(include)

add_executable(main main.c
                    utils/utils.c
                    var_int.c
                    utils/protocol_utils.c)
