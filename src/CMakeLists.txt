cmake_minimum_required(VERSION 3.31)
project("minecraft-server-c" LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c23 -Wall -Wextra -Werror")

include_directories(include)

add_executable(main main.c
                    utils/utils.c
                    var_int.c
                    utils/protocol_utils.c
                    packet_handlers.c
                    packet_definition.c)
