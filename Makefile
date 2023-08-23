##################################################################
# Declare Makefile variables
##################################################################

SDL2_CFLAGS := $(shell sdl2-config --cflags )
SDL2_LIB_PATH := $(shell sdl2-config --libs )

CC = clang++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs" -I"./libs/lua"
SRC_FILES = src/*.cpp \
			src/Game/*.cpp \
			src/Logger/*.cpp \
 			src/ECS/*.cpp \
			src/AssetStore/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.4 $(SDL2_LIB_PATH)
OBJ_NAME = gameengine 

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SDL2_CFLAGS) $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./gameengine

clean:
	rm ./gameengine
