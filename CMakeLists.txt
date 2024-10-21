cmake_minimum_required(VERSION 3.28)
project(SimpleChess_SDL C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)
set(CMAKE_PREFIX_PATH "C:/SDL2_ttf-2.22.0/x86_64-w64-mingw32")

set(SDL2_PATH "C:/SDL2-2.30.7/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_executable(SimpleChess_SDL main.c
        res.c
        res.h
        game.c
        game.h
        ui.c
        ui.h
        board.c
        board.h
        pieces.c
        pieces.h
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})