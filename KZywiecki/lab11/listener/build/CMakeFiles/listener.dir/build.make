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
CMAKE_SOURCE_DIR = /home/krzys/Pulpit/zajecia/lab11/listener

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/krzys/Pulpit/zajecia/lab11/listener/build

# Include any dependencies generated for this target.
include CMakeFiles/listener.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/listener.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/listener.dir/flags.make

CMakeFiles/listener.dir/listener.c.o: CMakeFiles/listener.dir/flags.make
CMakeFiles/listener.dir/listener.c.o: ../listener.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/krzys/Pulpit/zajecia/lab11/listener/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/listener.dir/listener.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/listener.dir/listener.c.o   -c /home/krzys/Pulpit/zajecia/lab11/listener/listener.c

CMakeFiles/listener.dir/listener.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/listener.dir/listener.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/krzys/Pulpit/zajecia/lab11/listener/listener.c > CMakeFiles/listener.dir/listener.c.i

CMakeFiles/listener.dir/listener.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/listener.dir/listener.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/krzys/Pulpit/zajecia/lab11/listener/listener.c -o CMakeFiles/listener.dir/listener.c.s

# Object files for target listener
listener_OBJECTS = \
"CMakeFiles/listener.dir/listener.c.o"

# External object files for target listener
listener_EXTERNAL_OBJECTS =

listener: CMakeFiles/listener.dir/listener.c.o
listener: CMakeFiles/listener.dir/build.make
listener: CMakeFiles/listener.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/krzys/Pulpit/zajecia/lab11/listener/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable listener"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/listener.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/listener.dir/build: listener

.PHONY : CMakeFiles/listener.dir/build

CMakeFiles/listener.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/listener.dir/cmake_clean.cmake
.PHONY : CMakeFiles/listener.dir/clean

CMakeFiles/listener.dir/depend:
	cd /home/krzys/Pulpit/zajecia/lab11/listener/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/krzys/Pulpit/zajecia/lab11/listener /home/krzys/Pulpit/zajecia/lab11/listener /home/krzys/Pulpit/zajecia/lab11/listener/build /home/krzys/Pulpit/zajecia/lab11/listener/build /home/krzys/Pulpit/zajecia/lab11/listener/build/CMakeFiles/listener.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/listener.dir/depend

