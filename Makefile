# The MIT License (MIT)
#
# Copyright © 2023 β
#
# Permission is hereby granted, free of charge, to any person obtaining a copy 
# of this software and associated documentation files (the “Software”), to deal 
# in the Software without restriction, including without limitation the rights 
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
# copies of the Software, and to permit persons to whom the Software is 
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in 
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, 
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
# OR OTHER DEALINGS IN THE SOFTWARE.


# MasterMaker
# Author: β
#
# This Makefile is designed to simplify the compilation and build process for any 
# C/C++ project of small or medium size. 
# It supports both debug and release modes, handles 
# automatic dependency generation, and provides various targets for building, 
# running, cleaning, and even analyzing the program using Valgrind. It will accept
# both c and cpp files and it can handle diferent project file structures.
# For best use, make sure your project has this structure:
#
# your_project_name/
# | src/
# | | main.cpp
# | | module_name/
# | | | module_name.cpp/
# | | | submodule_name_1.cpp/
# | | | submodule_name_2.c/
# | include/
# | | module_name/
# | | | module_name.hpp/
# | | | submodule_name_1.h/
# | | | submodule_name_2.hpp/
# | Makefile
#
# You don't need to care about paths when importing your headers. In the example
# above, writing '#include "module_name.hpp"' will sufice.
#
# Be aware this Makefile has the capability to change itself. So, it might be 
# cofusing to tinker with it.




#==============================================================================/
#======================| CONFIGURATIONS |======================================/
#==============================================================================/
# NOTE: Edit this configurations according to your needs.

# Compiler and compiler flags
PROGRAM_NAME := myapp
CUDAC := nvcc
CC := g++
STD_MODE := debug

CUDACFLAGS := 
CCFLAGS := -Wall -Wextra
LDFLAGS := -lm -lncursesw
VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all



#==============================================================================/
#======================| INNER LOGIC |=========================================/
#==============================================================================/
# NOTE: DO NOT EDIT THE LINES BELLOW UNLESS YOU KNOW WHAT YOU ARE DOING.

# Definitions
MKDIR := mkdir -p
MK := make
RM := rm -fr
CLEAR := clear
ECHO := echo

# Determine build mode (debug or release)
MODE := $(STD_MODE)
TARGET_ROOT_DIR := target

DEBUG_CCFLAGS := -O0 -g -ggdb3 -DDEBUG
RELEASE_CCFLAGS := -O3 -DNDEBUG -pedantic -Werror -DRELEASE

DEBUG_CUDACFLAGS := -O0 -g -DDEBUG
RELEASE_CUDACFLAGS := -O3 -DNDEBUG -DRELEASE 

ifneq ($(wildcard bundle-force-release.txt),)
	RELEASE := true
endif

ifeq ($(RELEASE),)
    MODE := debug
    CCFLAGS += $(DEBUG_CCFLAGS)
	CUDACFLAGS += $(DEBUG_CUDACFLAGS)
else
    MODE := release
    CCFLAGS += $(RELEASE_CCFLAGS)
	CUDACFLAGS += $(RELEASE_CUDACFLAGS)
endif

TARGET_DIR := $(TARGET_ROOT_DIR)/$(MODE)

# Directory structure
SRC_DIR := src

INC_DIR := include
OBJ_DIR := $(TARGET_DIR)/obj
BIN_DIR := $(TARGET_DIR)/bin
ANALYSIS_DIR := $(TARGET_DIR)/analysis
EXECUTABLE := $(BIN_DIR)/$(PROGRAM_NAME)

ifeq ($(wildcard $(SRC_DIR)),)
    SRC_DIR := .
    INC_DIR := .
endif

# Include directories
INCLUDE := $(shell find $(INC_DIR)/ -type d -printf "-I%p ") $(shell find $(SRC_DIR)/ -type d -printf "-I%p ")

# Collect source files
CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
C_FILES := $(shell find $(SRC_DIR) -name '*.c')
CU_FILES := $(shell find $(SRC_DIR) -name '*.cu')

# Generate corresponding object file names
CPP_OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_FILES))
C_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_FILES))
CU_OBJ_FILES := $(patsubst $(SRC_DIR)/%.cu,$(OBJ_DIR)/%.o,$(CU_FILES))

# Combine all object files
OBJ_FILES := $(CPP_OBJ_FILES) $(C_OBJ_FILES) $(CU_OBJ_FILES)

# Generate dependency files
CPP_DEP_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(CPP_FILES))
C_DEP_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(C_FILES))
CU_DEP_FILES := $(patsubst $(SRC_DIR)/%.cu,$(OBJ_DIR)/%.d,$(CU_FILES))

# Combine all dependency files
DEP_FILES := $(CPP_DEP_FILES) $(C_DEP_FILES) $(CU_DEP_FILES)




# Bundle file setup
BUNDLE_DIR := target/bundle
BUNDLE_NAME := bundle.zip








#==============================================================================/
#======================| MAIN TARGETS |========================================/
#==============================================================================/

# Make all the default in runcodes bundle
ifneq ($(wildcard bundle-force-release.txt),)

all:
	@$(MK) build -s > /dev/null

run:
	@$(EXECUTABLE)

endif

# Build target
build: $(EXECUTABLE)
ifneq ($(wildcard $(SRC_DIR).),)
	@$(ECHO) " "
	@$(ECHO) "[$(MODE):build]\t WARNING"
	@$(ECHO) ":\t'src' directory was not detected in the current location."
	@$(ECHO) ":\tThis is not a good look :\\"
	@$(ECHO) ":\tI will do my best, but I strongly recommend that,"
	@$(ECHO) ":\twhen it comes to project organization: GET GOOD."
	@$(ECHO) ":\tI will assume '$(dir $(abspath $(lastword $(MAKEFILE_LIST))))'"
	@$(ECHO) ":\tis your source directory for now."
	@$(ECHO) " "
	@$(ECHO) "[$(MODE):build]\t •\`_´• Build done."
else
	@$(ECHO) "[$(MODE):build]\t (ﾉ◕ヮ◕)ﾉ*:・ﾟ✧ Build done!"
endif


# Run target
ifeq ($(wildcard bundle-force-release.txt),)

run: build
	@$(ECHO) "[$(MODE):run]\t Starting program... :)"
	@$(EXECUTABLE)

all:
	@$(MK) build -s > /dev/null

endif






#==============================================================================/
#======================| RULES |===============================================/
#==============================================================================/

# Compile source files and link to executable
$(EXECUTABLE): $(OBJ_FILES)
	@$(ECHO) "[$(MODE):rule:link]\t Linking executable '$(PROGRAM_NAME)'..."
ifeq ($(VERBOSE),)
	@$(MKDIR) "$(@D)"
	@$(CUDAC) $(CUDACFLAGS) $(INCLUDE) $^ -o "$@" $(LDFLAGS)
else
	$(MKDIR) "$(@D)"
	$(CUDAC) $(CUDACFLAGS) $(INCLUDE) $^ -o "$@" $(LDFLAGS)
endif


# Compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(ECHO) "[$(MODE):rule:compile]\t Compiling $(subst $(OBJ_DIR)/,,$@)..."
ifeq ($(VERBOSE),)
	@$(MKDIR) "$(@D)"
	@$(CC) $(CCFLAGS) $(INCLUDE) -MMD -MP -c "$<" -o "$@"
else
	$(MKDIR) "$(@D)"
	$(CC) $(CCFLAGS) $(INCLUDE) -MMD -MP -c "$<" -o "$@"
endif


# Compile C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(ECHO) "[$(MODE):rule:compile]\t Compiling $(subst $(OBJ_DIR)/,,$@)..."
ifeq ($(VERBOSE),)
	@$(MKDIR) $(@D)
	@$(CC) $(CCFLAGS) $(INCLUDE) -MMD -MP -c "$<" -o "$@"
else
	$(MKDIR) $(@D)
	$(CC) $(CCFLAGS) $(INCLUDE) -MMD -MP -c "$<" -o "$@"
endif


# Compile Cuda source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cu
	@$(ECHO) "[$(MODE):rule:compile]\t Compiling $(subst $(OBJ_DIR)/,,$@)..."
ifeq ($(VERBOSE),)
	@$(MKDIR) $(@D)
	@$(CUDAC) $(CUDACFLAGS) $(INCLUDE) -MMD -MP -c "$<" -o "$@"
else
	$(MKDIR) $(@D)
	$(CUDAC) $(CUDACFLAGS) $(INCLUDE) -MMD -MP -c "$<" -o "$@"
endif



# Include dependency files
-include $(DEP_FILES)






#==============================================================================/
#======================| TARGETS |=============================================/
#==============================================================================/

.PHONY: clean NUKE help bundle

# Clean build artifacts
clean:
	@$(ECHO) "ʕノ•ᴥ•ʔノ ︵ ┻━┻"
	@$(ECHO) "[$(MODE):clean]\t Cleaning this mess..."
	@$(RM) "$(TARGET_DIR)"

# Clean and clear the console
clear: clean
	$(CLEAR)

# Clean and rebuild
fresh: clean build
	@$(ECHO) "[$(MODE):fresh]\t Fresh from the oven :)"

# Clean and rebuild
fresher: clear build
	@$(ECHO) "[$(MODE):fresher]\t Super fresh from the oven :)"

# Clean and rebuild
quick: 
	$(CLEAR)
	@$(MK) run --no-print-directory


# Run Valgrind memory analysis
analysis: build
	@$(ECHO) "[$(MODE):analysis]\t Starting analysis..."
	@$(MKDIR) "$(ANALYSIS_DIR)"
	@$(eval ANALYSIS_COUNT := $(shell ls -1 $(ANALYSIS_DIR) | grep -oE 'valgrind_analysis_[0-9]+' | sort -rn | head -n 1 | grep -oE '[0-9]+'))
	@$(eval NEXT_ANALYSIS_COUNT := $(shell echo $$(($(ANALYSIS_COUNT) + 1))))
	@$(eval ANALYSIS_FILENAME := valgrind_analysis_$(shell printf "%03d" $(NEXT_ANALYSIS_COUNT)).txt)
	valgrind $(VALGRIND_FLAGS) "$(EXECUTABLE)" > "$(ANALYSIS_DIR)/$(ANALYSIS_FILENAME)" 2>&1

# Prompt for user confirmation before nuking the target folder
NUKE:
	@$(ECHO) " "
	@$(ECHO) "	( 0 _ 0 )"
	@read -p "Are you sure you want to NUKE the target folder? This will delete all build artifacts. (yes/no): " confirmation; \
	if [ "$$confirmation" = "yes" ]; then \
		$(RM) "$(TARGET_ROOT_DIR)"; \
		$(ECHO) "ヽ(｀Д´)⊃━☆ﾟ. * ･ ｡ﾟ, Target folder nuked."; \
		$(ECHO) " "; \
	else \
		echo "Aborted."; \
	fi


# Create a .gitignore file with common build artifacts
gitignore:
	@$(ECHO) "Creating .gitignore file..."
	@$(ECHO) "# Common build artifacts" > .gitignore
	@$(ECHO) "$(TARGET_ROOT_DIR)/" >> .gitignore
	@$(ECHO) "*.o" >> .gitignore
	@$(ECHO) "*.d" >> .gitignore
	@$(ECHO) "*.exe" >> .gitignore
	@$(ECHO) "*.out" >> .gitignore
	@$(ECHO) "*.log" >> .gitignore
	@$(ECHO) "analysis/" >> .gitignore
	@$(ECHO) "*.stackdump" >> .gitignore
	@$(ECHO) ".vscode/" >> .gitignore
	@$(ECHO) ".idea/" >> .gitignore
	@$(ECHO) ".clangd/" >> .gitignore
	@$(ECHO) ".DS_Store" >> .gitignore
	@$(ECHO) "*.swp" >> .gitignore
	@$(ECHO) "*.swo" >> .gitignore
	@$(ECHO) ".gitignore" >> .gitignore
	@$(ECHO) "Gitignore file created."


hello:
	@$(ECHO) "( * ^ *) ノシ"
	@$(ECHO) "Hello, World!"

# Help target to display available targets and their descriptions
help:
	@echo ""
	@echo "Welcome to MasterMaker - Makefile Helper"
	@echo "======================================="
	@echo ""
	@echo "NOTE: Try clean or NUKE if errors arise."
	@echo ""
	@echo "Available targets:"
	@echo ""
	@echo "  build       - Build the program"
	@echo "  run         - Build and run the program"
	@echo "  clean       - Clean build artifacts"
	@echo "  clear       - Clean and clear the console"
	@echo "  fresh       - Clean, build, and run"
	@echo "  analysis    - Run Valgrind memory analysis"
	@echo "  gitignore   - Create a .gitignore file for common build artifacts"
	@echo "  hello       - Print a friendly greeting"
	@echo ""
	@echo "Advanced targets:"
	@echo ""
	@echo "  quick       - Quick clean, build, and run"
	@echo "  bundle      - Create a bundle of your project (excluding build artifacts)"
	@echo "  NUKE        - Delete the target folder (use with caution)"
	@echo ""
	@echo "Optional Configuration:"
	@echo ""
	@echo "  RELEASE=true - Build in release mode (optimized)"
	@echo "                 Use this option to enable optimization flags and build in release mode."
	@echo "                 Example: 'make build RELEASE=true'"
	@echo ""
	@echo "For more details on each target, run 'make <target_name>'"
	@echo ""



h: 
	@$(MK) help --no-print-directory

HELP: 
	@$(ECHO) "Hey, calm down, I can help! ;)"
	@$(MK) help --no-print-directory

r:
	@$(MK) run --no-print-directory

b:
	@$(MK) build --no-print-directory

# Show a warning message for misspelled targets
%: ; @$(if $(filter-out %.d,$@),$($(info ¯\_(ツ)_/¯ Unknown target '$@'. Use 'make help' to see available targets.)))


bundle:
	@$(MKDIR) "$(TARGET_ROOT_DIR)/bundle"
	@$(MKDIR) "$(TARGET_ROOT_DIR)/config"
	@$(ECHO) '--FORCE-RELEASE' >> bundle-force-release.txt
	@echo "Creating bundle..."
	@zip -r $(BUNDLE_DIR)/$(BUNDLE_NAME) ./ -x "target/*" ".git/*" ".vscode"
	@echo "Bundle created: $(BUNDLE_NAME)"
	@$(RM) bundle-force-release.txt