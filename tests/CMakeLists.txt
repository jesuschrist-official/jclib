add_subdirectory(algorithm)


add_executable(
    jclib.exe
    ${CMAKE_CURRENT_SOURCE_DIR}/jclib.c
)

target_link_libraries(
    jclib.exe
    jclib
)


