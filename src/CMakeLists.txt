cmake_minimum_required(VERSION 3.31)
project("minecraft-server-c" LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c23 -Wall -Wextra -Werror")

include_directories(include)

add_executable(main main.c
                    var_int.c
                    buffer.c
                    utils/utils.c
                    packets/c2s/handshaking.c
                    packets/c2s/status.c
                    packets/handlers/handshaking.c
                    packets/handlers/status.c
                    packets/s2c/status.c)
