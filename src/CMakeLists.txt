add_library(lib lib.c)
target_link_libraries(lib m)
add_executable(P1 main.c)
target_link_libraries(P1 lib)
