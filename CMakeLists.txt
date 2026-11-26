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

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_NAME "Cokoban")
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_HTML NO)
    set(DOXYGEN_GENERATE_LATEX YES)
    set(DOXYGEN_USE_PDFLATEX YES)
    set(DOXYGEN_LATEX_BATCHMODE YES)
    set(DOXYGEN_PDF_HYPERLINKS YES)
    set(DOXYGEN_INPUT_ENCODING UTF-8)
    set(DOXYGEN_FILE_PATTERNS *.c *.h *.md)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/docs/index.md)
    set(DOXYGEN_EXCLUDE_PATTERNS */debugmalloc.h */econio*)

    set(DOXYGEN_RECURSIVE YES)      # needed so src/tui_windows and the libs' subfolders are scanned
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/docs_out)

    doxygen_add_docs(docs
        ${CMAKE_SOURCE_DIR}/docs
        ${CMAKE_SOURCE_DIR}/libs/vec2
        ${CMAKE_SOURCE_DIR}/libs/darr
        ${CMAKE_SOURCE_DIR}/libs/tui
        ${CMAKE_SOURCE_DIR}/src
        COMMENT "GENERATING DOCS"
    )
endif()