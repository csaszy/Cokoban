cmake_minimum_required(VERSION 3.10)
project(Cokoban VERSION 1.0 LANGUAGES C)

add_compile_options(-Wall -Wextra -Werror)
set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(libs/vec2)
add_subdirectory(libs/darr)
add_subdirectory(libs/tui)
add_subdirectory(libs/econio)

add_executable(game src/main.c
    src/debugmalloc.h
    src/game.c
    src/game.h
    src/tui_windows/window_context.h
    src/tui_windows/window_context.c
    src/tui_windows/window_start.h
    src/tui_windows/window_start.c
    src/tui_windows/window_levels.h
    src/tui_windows/window_levels.c
    src/tui_windows/window_game.h
    src/tui_windows/window_game.c
    src/tui_windows/window_scoreboard.h
    src/tui_windows/window_scoreboard.c
    src/tui_windows/window_win.h
    src/tui_windows/window_constants.h
    src/tui_windows/window_constants.c
    src/tui_windows/window_win.c
    )

set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})
target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(game vec2 tui econio)

# from: https://stackoverflow.com/questions/42476765/copying-assets-directory-from-source-to-build-directory-in-cmake
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${OUT_DIR}/assets
)
add_dependencies(game copy_assets)