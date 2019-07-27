add_library(
    jclib
    ${CMAKE_CURRENT_SOURCE_DIR}/string.c
)

add_subdirectory(net)
add_subdirectory(math)

install(TARGETS jclib DESTINATION lib)
