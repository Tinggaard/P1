add_library(lib_compare lib.c)
target_link_libraries(lib_compare m)
add_executable(P1_compare main.c)
target_link_libraries(P1_compare lib_compare)
