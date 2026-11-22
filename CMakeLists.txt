cmake_minimum_required(VERSION 3.10)
project(Cokoban VERSION 1.0 LANGUAGES C)

add_compile_options(-Wall -Wextra )
set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(libs/vec2)
add_subdirectory(libs/tui)
add_subdirectory(libs/econio)

add_executable(game src/main.c
    src/debugmalloc.h
    src/game.c
    src/game.h
    src/events.c
    src/events.h
    src/darr.c
    src/darr.h
    src/tui.h
    src/tui.c
    src/window_start.h
    src/window_levels.h
    src/window_game.h
    src/constants.h
    )

set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})

target_link_libraries(game vec2 tui econio)

# from: https://stackoverflow.com/questions/42476765/copying-assets-directory-from-source-to-build-directory-in-cmake
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${OUT_DIR}/assets
)
add_dependencies(game copy_assets)