add_executable(
    vec.exe
    ${CMAKE_CURRENT_SOURCE_DIR}/vec.c
)

target_link_libraries(
    vec.exe
    jclib-math
    m
)


