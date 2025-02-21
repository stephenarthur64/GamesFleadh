# ----------------------------------------
# Silence command echoing
# ----------------------------------------
.SILENT:

# ----------------------------------------
# Default Compiler and Directories
# ----------------------------------------
CC						:= gcc

# ----------------------------------------
# General Configuration
# ----------------------------------------
# Directories
RAYLIB_DIR		:= ./raylib

DEBUG_DIR 				:= ./GamesFleadh/x64/Debug
RELEASE_DIR 			:= ./release
OBJECTS_DIR				:= ./GamesFleadh/x64/Debug

SRC_DIR					:= ./src

RESOURCE_DIR 			:= ./GamesFleadh/GamesFleadh/ASSETS

WEB_DIR					:= ./web

RAYLIB_INCLUDE			:= $(RAYLIB_DIR)/include
RAYLIB_LIBRARY			:= $(RAYLIB_DIR)/lib
RAYLIB_LIBRARY_WEB		:= $(RAYLIB_DIR)/raylib/build_web/raylib


# ----------------------------------------
# OS detection
# ----------------------------------------
UNAME_S					:= $(shell uname -s)
IS_WINDOWS				:= $(if $(findstring MINGW,$(UNAME_S)),TRUE,FALSE)
IS_LINUX				:= $(if $(findstring Linux,$(UNAME_S)),TRUE,FALSE)
IS_WSL					:= $(shell grep -qi Microsoft /proc/version && echo TRUE || echo FALSE)
IS_MACOS				:= $(if $(findstring Darwin,$(UNAME_S)),TRUE,FALSE)

# ----------------------------------------
# OS detection
# ----------------------------------------
INCLUDES				:= -I. -I$(RAYLIB_INCLUDE) -I$(CUTEHEADERS_INCLUDE)
LIBS					:= -L$(RAYLIB_LIBRARY)
LIBRARIES				:= -lraylib -lm -lpthread

# ----------------------------------------
# Configuration Compiler Flags
# ----------------------------------------
CONFIG					?= debug
OPTIMISATION_LEVEL		?= 2

ifeq ($(CONFIG), debug)
	BUILD_DIR := $(DEBUG_DIR)
	CFLAGS := -std=c11 -Wall -Wextra -Werror -g -DDEBUG $(INCLUDES)
else ifeq ($(CONFIG), release)
	BUILD_DIR := $(RELEASE_DIR)
	CFLAGS := -std=c11 -O$(OPTIMISATION_LEVEL) -DNDEBUG $(INCLUDES)
else
	$(error Invalid CONFIG value: $(CONFIG))
endif

# ----------------------------------------
# Platform-specific variables
# ----------------------------------------
ifeq ($(IS_WINDOWS),TRUE)
	# Windows-specific settings
	LIBRARIES			+= -lglfw3 -lopengl32 -lgdi32 -lwinmm
	TARGET				:= $(BUILD_DIR)/GamesFleadh.exe
	WEB_APP				:= start http://localhost:8000/

else ifeq ($(IS_MACOS),TRUE)
	# macOS-specific settings
	CC 					:= clang
	LIBRARIES			+= -framework IOKit -framework CoreFoundation -framework Cocoa -framework CoreGraphics
	TARGET				:= $(BUILD_DIR)/GamesFleadh.bin
	WEB_APP				:= open http://localhost:8000/

else ifeq ($(IS_WSL),TRUE)
	# WSL settings
	LIBRARIES			+= -lGL -ldl
	TARGET				:= $(BUILD_DIR)/GamesFleadh.bin
	WEB_APP				:= wslview http://localhost:8000/

else
	# Linux settings
	LIBRARIES			+= -lGL -ldl
	TARGET				:= $(BUILD_DIR)/GamesFleadh.bin
	WEB_APP				:= xdg-open http://localhost:8000/
endif


# ----------------------------------------
# Source files
# ----------------------------------------
SRC						:= $(wildcard $(SRC_DIR)/*.c)
OBJ						:= $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/$(OBJECTS_DIR)/%.o)

# ----------------------------------------
# Targets
# ----------------------------------------
# Default target
.PHONY: all

# Compile object files
$(BUILD_DIR)/$(OBJECTS_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)/$(OBJECTS_DIR)
    # Copy icon files and spritesheet(s) png
    # Just PNGs copied at the moment
	mkdir -p $(BUILD_DIR)/$(RESOURCE_DIR)
	cp $(RESOURCE_DIR)/*.png $(BUILD_DIR)/$(RESOURCE_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build target
.PHONY: build
build: BUILD_TYPE := build\
	$(call INFO_MSG,$(MSG_BUILD_START))\
	$(MAKE) $(OBJ)\
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS) $(LIBRARIES)\
	$(call SUCCESS_MSG,$(MSG_BUILD_END))\

# Run target
.PHONY: run
run:
	$(call INFO_MSG,$(MSG_RUN_BINARY))
	./$(TARGET)



# Stop Web Server target
stop_web_server:
	$(MAKE) -C $(RAYLIB_DIR) stop_web_server

# Start Web Server target
start_web_server:
	$(call INFO_MSG,$(WEB_DIR))
	$(info WEB_DIR: $(WEB_DIR))

	python3 -m http.server --directory $(WEB_DIR) &
	sleep 3

# Run Web target
.PHONY: run_web
run_web: build_web stop_web_server start_web_server
	$(WEB_APP)

# Clean target
.PHONY: clean
clean:
	$(call INFO_MSG,"Cleaning...")
	rm -f $(TARGET_DEBUG) $(TARGET_RELEASE)
	rm -rf $(DEBUG_DIR) $(RELEASE_DIR) ${WEB_DIR}
	$(call SUCCESS_MSG,"Clean complete")

# Clean target
.PHONY: clean_toolchain
clean_toolchain: clean
	$(call INFO_MSG,"Cleaning toolchain...")
	$(MAKE) -C $(RAYLIB_DIR) clean_toolchain
	$(call SUCCESS_MSG,"Clean toolchain complete")