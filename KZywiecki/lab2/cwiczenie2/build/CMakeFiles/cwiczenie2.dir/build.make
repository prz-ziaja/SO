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
CMAKE_SOURCE_DIR = /home/krzys/Pulpit/zajecia/lab2/cwiczenie2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/krzys/Pulpit/zajecia/lab2/cwiczenie2/build

# Include any dependencies generated for this target.
include CMakeFiles/cwiczenie2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cwiczenie2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cwiczenie2.dir/flags.make

CMakeFiles/cwiczenie2.dir/writeall.c.o: CMakeFiles/cwiczenie2.dir/flags.make
CMakeFiles/cwiczenie2.dir/writeall.c.o: ../writeall.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/krzys/Pulpit/zajecia/lab2/cwiczenie2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/cwiczenie2.dir/writeall.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cwiczenie2.dir/writeall.c.o   -c /home/krzys/Pulpit/zajecia/lab2/cwiczenie2/writeall.c

CMakeFiles/cwiczenie2.dir/writeall.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cwiczenie2.dir/writeall.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/krzys/Pulpit/zajecia/lab2/cwiczenie2/writeall.c > CMakeFiles/cwiczenie2.dir/writeall.c.i

CMakeFiles/cwiczenie2.dir/writeall.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cwiczenie2.dir/writeall.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/krzys/Pulpit/zajecia/lab2/cwiczenie2/writeall.c -o CMakeFiles/cwiczenie2.dir/writeall.c.s

# Object files for target cwiczenie2
cwiczenie2_OBJECTS = \
"CMakeFiles/cwiczenie2.dir/writeall.c.o"

# External object files for target cwiczenie2
cwiczenie2_EXTERNAL_OBJECTS =

cwiczenie2: CMakeFiles/cwiczenie2.dir/writeall.c.o
cwiczenie2: CMakeFiles/cwiczenie2.dir/build.make
cwiczenie2: CMakeFiles/cwiczenie2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/krzys/Pulpit/zajecia/lab2/cwiczenie2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cwiczenie2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cwiczenie2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cwiczenie2.dir/build: cwiczenie2

.PHONY : CMakeFiles/cwiczenie2.dir/build

CMakeFiles/cwiczenie2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cwiczenie2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cwiczenie2.dir/clean

CMakeFiles/cwiczenie2.dir/depend:
	cd /home/krzys/Pulpit/zajecia/lab2/cwiczenie2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/krzys/Pulpit/zajecia/lab2/cwiczenie2 /home/krzys/Pulpit/zajecia/lab2/cwiczenie2 /home/krzys/Pulpit/zajecia/lab2/cwiczenie2/build /home/krzys/Pulpit/zajecia/lab2/cwiczenie2/build /home/krzys/Pulpit/zajecia/lab2/cwiczenie2/build/CMakeFiles/cwiczenie2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cwiczenie2.dir/depend

