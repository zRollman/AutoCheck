# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /workspace/Benchmarks/checkpoint/himeno

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspace/Benchmarks/checkpoint/himeno/build

# Include any dependencies generated for this target.
include CMakeFiles/himeno.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/himeno.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/himeno.dir/flags.make

CMakeFiles/himeno.dir/src/himenoBMT.c.o: CMakeFiles/himeno.dir/flags.make
CMakeFiles/himeno.dir/src/himenoBMT.c.o: ../src/himenoBMT.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/Benchmarks/checkpoint/himeno/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/himeno.dir/src/himenoBMT.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/himeno.dir/src/himenoBMT.c.o   -c /workspace/Benchmarks/checkpoint/himeno/src/himenoBMT.c

CMakeFiles/himeno.dir/src/himenoBMT.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/himeno.dir/src/himenoBMT.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /workspace/Benchmarks/checkpoint/himeno/src/himenoBMT.c > CMakeFiles/himeno.dir/src/himenoBMT.c.i

CMakeFiles/himeno.dir/src/himenoBMT.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/himeno.dir/src/himenoBMT.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /workspace/Benchmarks/checkpoint/himeno/src/himenoBMT.c -o CMakeFiles/himeno.dir/src/himenoBMT.c.s

CMakeFiles/himeno.dir/src/himenoBMT.c.o.requires:

.PHONY : CMakeFiles/himeno.dir/src/himenoBMT.c.o.requires

CMakeFiles/himeno.dir/src/himenoBMT.c.o.provides: CMakeFiles/himeno.dir/src/himenoBMT.c.o.requires
	$(MAKE) -f CMakeFiles/himeno.dir/build.make CMakeFiles/himeno.dir/src/himenoBMT.c.o.provides.build
.PHONY : CMakeFiles/himeno.dir/src/himenoBMT.c.o.provides

CMakeFiles/himeno.dir/src/himenoBMT.c.o.provides.build: CMakeFiles/himeno.dir/src/himenoBMT.c.o


CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o: CMakeFiles/himeno.dir/flags.make
CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o: /workspace/Benchmarks/libcheckpoint/checkpoint.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/Benchmarks/checkpoint/himeno/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o   -c /workspace/Benchmarks/libcheckpoint/checkpoint.c

CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /workspace/Benchmarks/libcheckpoint/checkpoint.c > CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.i

CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /workspace/Benchmarks/libcheckpoint/checkpoint.c -o CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.s

CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o.requires:

.PHONY : CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o.requires

CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o.provides: CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o.requires
	$(MAKE) -f CMakeFiles/himeno.dir/build.make CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o.provides.build
.PHONY : CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o.provides

CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o.provides.build: CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o


# Object files for target himeno
himeno_OBJECTS = \
"CMakeFiles/himeno.dir/src/himenoBMT.c.o" \
"CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o"

# External object files for target himeno
himeno_EXTERNAL_OBJECTS =

himeno: CMakeFiles/himeno.dir/src/himenoBMT.c.o
himeno: CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o
himeno: CMakeFiles/himeno.dir/build.make
himeno: CMakeFiles/himeno.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/workspace/Benchmarks/checkpoint/himeno/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable himeno"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/himeno.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/himeno.dir/build: himeno

.PHONY : CMakeFiles/himeno.dir/build

CMakeFiles/himeno.dir/requires: CMakeFiles/himeno.dir/src/himenoBMT.c.o.requires
CMakeFiles/himeno.dir/requires: CMakeFiles/himeno.dir/workspace/Benchmarks/libcheckpoint/checkpoint.c.o.requires

.PHONY : CMakeFiles/himeno.dir/requires

CMakeFiles/himeno.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/himeno.dir/cmake_clean.cmake
.PHONY : CMakeFiles/himeno.dir/clean

CMakeFiles/himeno.dir/depend:
	cd /workspace/Benchmarks/checkpoint/himeno/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspace/Benchmarks/checkpoint/himeno /workspace/Benchmarks/checkpoint/himeno /workspace/Benchmarks/checkpoint/himeno/build /workspace/Benchmarks/checkpoint/himeno/build /workspace/Benchmarks/checkpoint/himeno/build/CMakeFiles/himeno.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/himeno.dir/depend

