#	Makefile for compiling & running Paint32 program
#	By Elias Mawa
#
#	IMPORTANT! MAKEFILE WAS MADE FOR USE FOR THE CLANG COMPILER
#
# MAKE TARGETS
# ------------
# the following targets are available :
#	$ make           	link and compile to linux
#   $ make windows	 	link and compile to windows
#   $ make run		 	run program (linux)
#   $ make run_windows	run program (windows)

OBJ_NAME = Paint
OBJ_DIR = bin/
SRC_DIR = src/
BUILD_DIR = build/

LINKER_FLAGS = -lGLU -lGL -lglut -pthread
LINKER_FLAGS_WINDOWS = -lglu32 -lopengl32 -lfreeglut
CXX = clang++

SOURCES = $(wildcard src/*.cxx) \
       $(wildcard src/*/*.cxx) \
       $(wildcard src/*/*/*.cxx)

TARGET = $(patsubst src/%.cxx,%.o,$(SOURCES))
OBJS = $(addprefix $(OBJ_DIR), $(TARGET))

.PHONY: directories

all : directories $(OBJS)
	mkdir -p ${BUILD_DIR}
	$(CXX) $(OBJS) $(LINKER_FLAGS) -o $(BUILD_DIR)$(OBJ_NAME)

windows : directories $(OBJS)
	mkdir -p ${BUILD_DIR}
	$(CXX) $(OBJS) $(LINKER_FLAGS_WINDOWS) -o $(BUILD_DIR)$(OBJ_NAME)
#%.o: %.cxx
#	$(CXX) -c $? -o $@

directories: ${OBJ_DIR}

${OBJ_DIR}: # make directory if does not exist
	mkdir -p ${OBJ_DIR}

# create object files from source files
bin/%.o: src/%.cxx
	mkdir -p $(dir $@)
	$(CXX) -c $? -o $@

# clean object files and outputs
clean :
	-rm -rf $(OBJ_DIR)
	-rm -rf $(BUILD_DIR)

run:
	./$(BUILD_DIR)$(OBJ_NAME)

run_windows:
	./$(OBJ_NAME)
