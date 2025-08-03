cmake_minimum_required(VERSION 3.31)
project("minecraft-server-c" LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c23 -Wall -Wextra -Werror")

find_package(OpenSSL REQUIRED)
find_package(cJSON REQUIRED)
find_package(CURL REQUIRED)

include_directories(include)

add_library(buffer STATIC
    buffer.c
)

add_library(varint STATIC
    var_int.c
)
target_link_libraries(varint buffer)

add_library(md5 STATIC lib/md5.c)

add_library(my_uuid STATIC lib/uuid.c)
target_link_libraries(my_uuid
    PUBLIC uuid
    PRIVATE md5
)

add_executable(main main.c
    crypto.c
    utils.c
    packets/c2s/handshaking.c
    packets/c2s/status.c
    packets/handlers/handshaking.c
    packets/handlers/status.c
    packets/s2c/status.c
)

target_link_libraries(main 
    OpenSSL::Crypto 
    ${CJSON_LIBRARY}
    CURL::libcurl
    my_uuid
    buffer
    varint
)

