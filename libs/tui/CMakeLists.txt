add_compile_options(-Wall -Wextra -Werror)
add_library(tui STATIC 
    src/debugmalloc.h
    src/tui.c
    src/events.c
)
target_link_libraries(tui econio darr vec2)
target_include_directories(tui PUBLIC include)
