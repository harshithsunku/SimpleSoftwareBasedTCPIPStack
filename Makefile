# Makefile for building the project
# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -pedantic
DEBUG_FLAGS := -g
RELEASE_FLAGS := -O2
LDFLAGS := -lpthread
INCLUDES := -I GenericEmbeddableDoublyLinkedList/ -I ThreadSafeCLogger/ -I infra/ -I common/ -I l3/ -I l2/
TARGET := main
OBJ_DIR := obj
DEP_DIR := deps

# Color definitions for print statements
RED := \033[0;31m
GREEN := \033[0;32m
NC := \033[0m # No Color

# Gather all source files from specified directories
SRCS := $(wildcard *.c) \
        $(wildcard GenericEmbeddableDoublyLinkedList/*.c) \
        $(wildcard ThreadSafeCLogger/*.c) \
        $(wildcard infra/*.c) \
        $(wildcard common/*.c) \
		$(wildcard l3/*.c) \
		$(wildcard l2/*.c) 

# Define patterns to exclude
EXCLUDE_PATTERNS := GenericEmbeddableDoublyLinkedList/main.c ThreadSafeCLogger/main.c

# Filter out the excluded files
FILTERED_SRCS := $(filter-out $(EXCLUDE_PATTERNS), $(SRCS))

# Create object and dependency files list for debug and release
DEBUG_OBJS := $(patsubst %.c,$(OBJ_DIR)/debug/%.o,$(notdir $(FILTERED_SRCS)))
RELEASE_OBJS := $(patsubst %.c,$(OBJ_DIR)/release/%.o,$(notdir $(FILTERED_SRCS)))
DEPS := $(patsubst %.o,$(DEP_DIR)/%.d,$(DEBUG_OBJS) $(RELEASE_OBJS))

# Specify directories to look for source files
VPATH := GenericEmbeddableDoublyLinkedList:ThreadSafeCLogger:infra:common:l3:l2

# Default build both debug and release
all: debug release

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(OBJ_DIR)/debug/$(TARGET)

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: $(OBJ_DIR)/release/$(TARGET)

# Target to build the executable for debug
$(OBJ_DIR)/debug/$(TARGET): $(DEBUG_OBJS)
	@echo "${GREEN}Building debug target: $@${NC}"
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "${GREEN}Debug build complete. Creating symlink to debug executable.${NC}\n"
	@cp $(OBJ_DIR)/debug/$(TARGET) $(TARGET)-debug

# Target to build the executable for release
$(OBJ_DIR)/release/$(TARGET): $(RELEASE_OBJS)
	@echo "${GREEN}Building release target: $@${NC}"
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "${GREEN}Release build complete. Creating symlink to release executable.${NC}\n"
	@cp $(OBJ_DIR)/release/$(TARGET) $(TARGET)-release

# Include the dependency files
-include $(DEPS)

# Rule to compile source files into object files for debug and release
$(OBJ_DIR)/debug/%.o: %.c
	@mkdir -p $(OBJ_DIR)/debug $(DEP_DIR)
	@echo "${GREEN}Compiling $< for debug...${NC}"
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -MF $(DEP_DIR)/$*.d -c $< -o $@
	@echo "${GREEN}Compiled $@ successfully.${NC}\n"

$(OBJ_DIR)/release/%.o: %.c
	@mkdir -p $(OBJ_DIR)/release $(DEP_DIR)
	@echo "${GREEN}Compiling $< for release...${NC}"
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -MF $(DEP_DIR)/$*.d -c $< -o $@
	@echo "${GREEN}Compiled $@ successfully.${NC}\n"

# Clean target
.PHONY: clean
clean:
	@echo "${RED}Cleaning up...${NC}"
	rm -rf $(OBJ_DIR) $(DEP_DIR)
	rm -f $(TARGET)-debug $(TARGET)-release
	@echo "${RED}Clean complete.${NC}\n"

# Help target
.PHONY: help
help:
	@echo "${GREEN}Usage:${NC}"
	@echo "  make                Build both debug and release targets"
	@echo "  make debug          Build the debug target $(TARGET)"
	@echo "  make release        Build the release target $(TARGET)"
	@echo "  make clean          Clean the build artifacts"
	@echo "  make help           Show this help message\n"

# Guide for adding new files or modules
#
# Adding a New Source File:
#   - Place your new .c file in the appropriate directory (e.g., infra/).
#   - If your new file is in one of the directories already listed in the SRCS variable, 
#     it will be automatically included in the build.
#
# Adding a New Module/Directory:
#   - Create a new directory for your module (e.g., newModule/).
#   - Add source files to your new directory.
#   - Update the SRCS variable to include your new directory:
#       SRCS := $(wildcard *.c) \
#               ... \
#               $(wildcard newModule/*.c)
#   - Update the VPATH variable to include your new directory:
#       VPATH := GenericEmbeddableDoublyLinkedList:ThreadSafeCLogger:infra:common:newModule
#   - Ensure that INCLUDES is updated if your new module contains new header files.
#
#   - After these steps, the new files or modules will be included in your build process.
