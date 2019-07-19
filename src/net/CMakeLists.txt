add_library(
    jclib-net
    ${CMAKE_CURRENT_SOURCE_DIR}/net.c
)

install(TARGETS jclib-net DESTINATION lib)
