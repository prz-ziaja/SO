# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/krzys/Pulpit/zajecia/lab2/cwiczenie3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/krzys/Pulpit/zajecia/lab2/cwiczenie3/build

# Include any dependencies generated for this target.
include CMakeFiles/testOffset.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testOffset.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testOffset.dir/flags.make

CMakeFiles/testOffset.dir/testOffset.c.o: CMakeFiles/testOffset.dir/flags.make
CMakeFiles/testOffset.dir/testOffset.c.o: ../testOffset.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/krzys/Pulpit/zajecia/lab2/cwiczenie3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/testOffset.dir/testOffset.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testOffset.dir/testOffset.c.o   -c /home/krzys/Pulpit/zajecia/lab2/cwiczenie3/testOffset.c

CMakeFiles/testOffset.dir/testOffset.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testOffset.dir/testOffset.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/krzys/Pulpit/zajecia/lab2/cwiczenie3/testOffset.c > CMakeFiles/testOffset.dir/testOffset.c.i

CMakeFiles/testOffset.dir/testOffset.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testOffset.dir/testOffset.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/krzys/Pulpit/zajecia/lab2/cwiczenie3/testOffset.c -o CMakeFiles/testOffset.dir/testOffset.c.s

# Object files for target testOffset
testOffset_OBJECTS = \
"CMakeFiles/testOffset.dir/testOffset.c.o"

# External object files for target testOffset
testOffset_EXTERNAL_OBJECTS =

testOffset: CMakeFiles/testOffset.dir/testOffset.c.o
testOffset: CMakeFiles/testOffset.dir/build.make
testOffset: CMakeFiles/testOffset.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/krzys/Pulpit/zajecia/lab2/cwiczenie3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable testOffset"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testOffset.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testOffset.dir/build: testOffset

.PHONY : CMakeFiles/testOffset.dir/build

CMakeFiles/testOffset.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testOffset.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testOffset.dir/clean

CMakeFiles/testOffset.dir/depend:
	cd /home/krzys/Pulpit/zajecia/lab2/cwiczenie3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/krzys/Pulpit/zajecia/lab2/cwiczenie3 /home/krzys/Pulpit/zajecia/lab2/cwiczenie3 /home/krzys/Pulpit/zajecia/lab2/cwiczenie3/build /home/krzys/Pulpit/zajecia/lab2/cwiczenie3/build /home/krzys/Pulpit/zajecia/lab2/cwiczenie3/build/CMakeFiles/testOffset.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testOffset.dir/depend
