cmake_minimum_required(VERSION 3.10)
project(game C)

set(SDL_DIR ${CMAKE_SOURCE_DIR}/vendors/SDL2)
set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(libs/vec2)

add_executable(game src/main.c)

set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})

target_include_directories(game PRIVATE ${SDL_DIR}/include)
target_link_directories(game PRIVATE ${SDL_DIR}/lib)
target_link_libraries(game PRIVATE vec2 mingw32 SDL2main SDL2 SDL2_image SDL2_gfx SDL2_ttf SDL2_mixer)
target_compile_options(game PRIVATE -Wall -Wextra)

# copy SDL DLLs next to the exe after each build
file(GLOB SDL_DLLS ${SDL_DIR}/bin/*.dll)
add_custom_command(TARGET game POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL_DLLS} ${OUT_DIR})