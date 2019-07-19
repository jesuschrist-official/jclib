cmake_minimum_required(VERSION 3.0)
project(jclib)

set(CMAKE_C_FLAGS "-Wall -Werror -Wfatal-errors -std=gnu99 ${CMAKE_C_FLAGS}")

include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(src)
add_subdirectory(tests)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")

