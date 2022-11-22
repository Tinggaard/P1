add_library(lib lib.c)
add_executable(P1 main.c)
target_link_libraries(P1 lib)
