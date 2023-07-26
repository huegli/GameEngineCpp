##################################################################
# Declare Makefile variables
##################################################################
CC = clang++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs" -I"./libs/lua" -I"/opt/homebrew/include/SDL2"
SRC_FILES = src/*.cpp \
			src/Game/*.cpp \
			src/Logger/*.cpp 
LINKER_FLAGS = -L"/opt/homebrew/lib" -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua 
OBJ_NAME = gameengine 

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./gameengine

clean:
	rm ./gameengine
