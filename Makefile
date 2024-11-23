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

CC = gcc
INCLUDE_DIR = -I"C:\\SDL2-2.30.7\\x86_64-w64-mingw32\\include"
DEBUG_FLAGS = -Wall -Wextra

LIB_DIR = -L"C:\\SDL2-2.30.7\\x86_64-w64-mingw32\\lib"
LIBS = -lSDL2 -lSDL2main 

PROJECT_NAME = SimpleChess
SRC_DIR = src
BIN_DIR = bin
FILES = board game input pieces ui main

SRC_FILES = $(addprefix $(SRC_DIR)\\, $(addsuffix .c, $(FILES)))
OBJ_FILES = $(SRC_FILES:.c=.o)

all: $(BIN_DIR)\\$(PROJECT_NAME).exe

$(BIN_DIR)\\$(PROJECT_NAME).exe: $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(INCLUDE_DIR) -o $@ $(OBJ_FILES) $(LIB_DIR) $(LIBS)
	@echo "Compilation successful: $@ has been created."
	@del /Q $(subst /,\,$(OBJ_FILES)) 2>nul  

$(BIN_DIR):
	mkdir $(BIN_DIR)

%.o: %.c
	$(CC) $(DEBUG_FLAGS) $(INCLUDE_DIR) -c $< -o $@

run: $(BIN_DIR)\\$(PROJECT_NAME).exe
	$(BIN_DIR)\\$(PROJECT_NAME).exe

clean:
	@if exist $(BIN_DIR)\\$(PROJECT_NAME).exe del /Q $(BIN_DIR)\\$(PROJECT_NAME).exe
	@if exist $(OBJ_FILES) del /Q $(subst /,\,$(OBJ_FILES)) 2>nul
