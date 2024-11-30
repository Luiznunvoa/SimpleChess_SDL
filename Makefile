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

PROJECT_NAME = SimpleChess
SRC_DIR = src
BIN_DIR = bin
FILES = board game input pieces ui main

CC = gcc
LIB = SDL2 SDL2_ttf 

LIB_PATHS = C:\\SDL2-2.30.7\\x86_64-w64-mingw32 C:\SDL2_ttf-2.22.0\x86_64-w64-mingw32  # Must match the order of LIB
ARGUMENTS = 

INCLUDE_DIRS = $(foreach dir,$(LIB_PATHS),-I"$(dir)\\include") 	# Check if the library's header files are actually here
LIB_DIRS = $(foreach dir,$(LIB_PATHS),-L"$(dir)\\lib") 			
LIB_LINKERS = $(foreach lib,$(LIB),-l$(lib)) 					

DLL_FILES = $(foreach lib,$(LIB),$(firstword $(foreach dir,$(LIB_PATHS),$(wildcard $(dir)\\bin\\$(lib).dll))))
DLL_TARGET = $(foreach lib,$(LIB),$(BIN_DIR)\\$(lib).dll) # Target paths for the DLLs in the binary directory

SRC_FILES = $(addprefix $(SRC_DIR)\\, $(addsuffix .c, $(FILES))) 	
OBJ_FILES = $(SRC_FILES:.c=.o) 										

all: $(BIN_DIR)\\$(PROJECT_NAME).exe $(DLL_TARGET)

$(BIN_DIR)\\$(PROJECT_NAME).exe: $(OBJ_FILES) | $(BIN_DIR) 
	$(CC) $(INCLUDE_DIRS) -o $@ $(OBJ_FILES) $(LIB_DIRS) $(LIB_LINKERS)
	@echo "Compilation successful"
	@del /Q $(subst /,\,$(OBJ_FILES)) 2>nul   

$(DLL_TARGET): | $(BIN_DIR) 
	@for %%F in ($(DLL_FILES)) do @( \
		copy /Y "%%F" "$(BIN_DIR)" >nul && \
		echo DLL %%~nF found! \
	)

$(BIN_DIR): 
	mkdir $(BIN_DIR)

%.o: %.c 
	$(CC) $(ARGUMENTS) $(INCLUDE_DIRS) -c $< -o $@

run:
	$(BIN_DIR)\\$(PROJECT_NAME).exe

clean:
	del /Q $(BIN_DIR)\\* 2>nul
	del /Q $(subst /,\,$(OBJ_FILES)) 2>nul
	@echo "All files have been deleted"