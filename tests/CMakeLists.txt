add_subdirectory(algorithm)
add_subdirectory(math)


add_executable(
    jclib.exe
    ${CMAKE_CURRENT_SOURCE_DIR}/jclib.c
)

target_link_libraries(
    jclib.exe
    jclib
)


add_executable(
    string.exe
    ${CMAKE_CURRENT_SOURCE_DIR}/string.c
)

target_link_libraries(
    string.exe
    jclib
)

