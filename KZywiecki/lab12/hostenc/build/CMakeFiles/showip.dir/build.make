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
CMAKE_SOURCE_DIR = /home/krzys/Pulpit/zajecia/lab12/hostenc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/krzys/Pulpit/zajecia/lab12/hostenc/build

# Include any dependencies generated for this target.
include CMakeFiles/showip.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/showip.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/showip.dir/flags.make

CMakeFiles/showip.dir/showip.c.o: CMakeFiles/showip.dir/flags.make
CMakeFiles/showip.dir/showip.c.o: ../showip.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/krzys/Pulpit/zajecia/lab12/hostenc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/showip.dir/showip.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/showip.dir/showip.c.o   -c /home/krzys/Pulpit/zajecia/lab12/hostenc/showip.c

CMakeFiles/showip.dir/showip.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/showip.dir/showip.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/krzys/Pulpit/zajecia/lab12/hostenc/showip.c > CMakeFiles/showip.dir/showip.c.i

CMakeFiles/showip.dir/showip.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/showip.dir/showip.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/krzys/Pulpit/zajecia/lab12/hostenc/showip.c -o CMakeFiles/showip.dir/showip.c.s

# Object files for target showip
showip_OBJECTS = \
"CMakeFiles/showip.dir/showip.c.o"

# External object files for target showip
showip_EXTERNAL_OBJECTS =

showip: CMakeFiles/showip.dir/showip.c.o
showip: CMakeFiles/showip.dir/build.make
showip: CMakeFiles/showip.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/krzys/Pulpit/zajecia/lab12/hostenc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable showip"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/showip.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/showip.dir/build: showip

.PHONY : CMakeFiles/showip.dir/build

CMakeFiles/showip.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/showip.dir/cmake_clean.cmake
.PHONY : CMakeFiles/showip.dir/clean

CMakeFiles/showip.dir/depend:
	cd /home/krzys/Pulpit/zajecia/lab12/hostenc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/krzys/Pulpit/zajecia/lab12/hostenc /home/krzys/Pulpit/zajecia/lab12/hostenc /home/krzys/Pulpit/zajecia/lab12/hostenc/build /home/krzys/Pulpit/zajecia/lab12/hostenc/build /home/krzys/Pulpit/zajecia/lab12/hostenc/build/CMakeFiles/showip.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/showip.dir/depend

