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
CMAKE_SOURCE_DIR = /home/krzys/Pulpit/zajecia/lab2/cwiczenie4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build

# Include any dependencies generated for this target.
include CMakeFiles/copy.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/copy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/copy.dir/flags.make

CMakeFiles/copy.dir/copy3.c.o: CMakeFiles/copy.dir/flags.make
CMakeFiles/copy.dir/copy3.c.o: ../copy3.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/copy.dir/copy3.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/copy.dir/copy3.c.o   -c /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/copy3.c

CMakeFiles/copy.dir/copy3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/copy.dir/copy3.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/copy3.c > CMakeFiles/copy.dir/copy3.c.i

CMakeFiles/copy.dir/copy3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/copy.dir/copy3.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/copy3.c -o CMakeFiles/copy.dir/copy3.c.s

# Object files for target copy
copy_OBJECTS = \
"CMakeFiles/copy.dir/copy3.c.o"

# External object files for target copy
copy_EXTERNAL_OBJECTS =

copy: CMakeFiles/copy.dir/copy3.c.o
copy: CMakeFiles/copy.dir/build.make
copy: time/libTime.a
copy: CMakeFiles/copy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable copy"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/copy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/copy.dir/build: copy

.PHONY : CMakeFiles/copy.dir/build

CMakeFiles/copy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/copy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/copy.dir/clean

CMakeFiles/copy.dir/depend:
	cd /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/krzys/Pulpit/zajecia/lab2/cwiczenie4 /home/krzys/Pulpit/zajecia/lab2/cwiczenie4 /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/CMakeFiles/copy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/copy.dir/depend

