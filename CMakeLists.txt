cmake_minimum_required(VERSION 3.10)
project(Cokoban VERSION 1.0 LANGUAGES C)

set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(libs/vec2)
add_subdirectory(libs/tui)
add_subdirectory(libs/econio)

add_executable(game src/main.c)

set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})

target_link_libraries(game vec2 tui econio)