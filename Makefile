# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Applications/CMake 2.8-9.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-9.app/Contents/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-9.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/czhang/Desktop/Codes/mia

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/czhang/Desktop/Codes/mia

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	"/Applications/CMake 2.8-9.app/Contents/bin/ccmake" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	"/Applications/CMake 2.8-9.app/Contents/bin/cmake" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/czhang/Desktop/Codes/mia/CMakeFiles /Users/czhang/Desktop/Codes/mia/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/czhang/Desktop/Codes/mia/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named bin/elementary

# Build rule for target.
bin/elementary: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 bin/elementary
.PHONY : bin/elementary

# fast build rule for target.
bin/elementary/fast:
	$(MAKE) -f CMakeFiles/bin/elementary.dir/build.make CMakeFiles/bin/elementary.dir/build
.PHONY : bin/elementary/fast

src/elly/elly/main.o: src/elly/elly/main.cpp.o
.PHONY : src/elly/elly/main.o

# target to build an object file
src/elly/elly/main.cpp.o:
	$(MAKE) -f CMakeFiles/bin/elementary.dir/build.make CMakeFiles/bin/elementary.dir/src/elly/elly/main.cpp.o
.PHONY : src/elly/elly/main.cpp.o

src/elly/elly/main.i: src/elly/elly/main.cpp.i
.PHONY : src/elly/elly/main.i

# target to preprocess a source file
src/elly/elly/main.cpp.i:
	$(MAKE) -f CMakeFiles/bin/elementary.dir/build.make CMakeFiles/bin/elementary.dir/src/elly/elly/main.cpp.i
.PHONY : src/elly/elly/main.cpp.i

src/elly/elly/main.s: src/elly/elly/main.cpp.s
.PHONY : src/elly/elly/main.s

# target to generate assembly for a file
src/elly/elly/main.cpp.s:
	$(MAKE) -f CMakeFiles/bin/elementary.dir/build.make CMakeFiles/bin/elementary.dir/src/elly/elly/main.cpp.s
.PHONY : src/elly/elly/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... bin/elementary"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... src/elly/elly/main.o"
	@echo "... src/elly/elly/main.i"
	@echo "... src/elly/elly/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

