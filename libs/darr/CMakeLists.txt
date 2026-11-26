add_library(darr STATIC 
    src/debugmalloc.h
    src/darr.c
)
target_include_directories(darr PUBLIC includes)
add_compile_options(-Wall -Wextra -Werror)