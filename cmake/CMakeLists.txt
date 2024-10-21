#
# Copyright (c) 2024 Luiz Gabriel Moraes "Luiz".
#
# This file is part of SimpleChess_SDL.
#
# SimpleChess_SDL is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License, version 3
# as published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# SimpleChess_SDL is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>..
#
cmake_minimum_required(VERSION 3.28)

project(SimpleChess_SDL C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

set(CMAKE_PREFIX_PATH "C:/SDL2_ttf-2.22.0/x86_64-w64-mingw32")

set(SDL2_PATH "C:/SDL2-2.30.7/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)

find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_executable(SimpleChess_SDL ../main.c
        ../res.c
        ../res.h
        ../game.c
        ../game.h
        ../ui.c
        ../ui.h
        ../board.c
        ../board.h
        ../pieces.h
        ../pieces.c
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})