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
include CMakeFiles/talker.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/talker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/talker.dir/flags.make

CMakeFiles/talker.dir/talker.c.o: CMakeFiles/talker.dir/flags.make
CMakeFiles/talker.dir/talker.c.o: ../talker.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/krzys/Pulpit/zajecia/lab11/listener/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/talker.dir/talker.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/talker.dir/talker.c.o   -c /home/krzys/Pulpit/zajecia/lab11/listener/talker.c

CMakeFiles/talker.dir/talker.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/talker.dir/talker.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/krzys/Pulpit/zajecia/lab11/listener/talker.c > CMakeFiles/talker.dir/talker.c.i

CMakeFiles/talker.dir/talker.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/talker.dir/talker.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/krzys/Pulpit/zajecia/lab11/listener/talker.c -o CMakeFiles/talker.dir/talker.c.s

# Object files for target talker
talker_OBJECTS = \
"CMakeFiles/talker.dir/talker.c.o"

# External object files for target talker
talker_EXTERNAL_OBJECTS =

talker: CMakeFiles/talker.dir/talker.c.o
talker: CMakeFiles/talker.dir/build.make
talker: CMakeFiles/talker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/krzys/Pulpit/zajecia/lab11/listener/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable talker"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/talker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/talker.dir/build: talker

.PHONY : CMakeFiles/talker.dir/build

CMakeFiles/talker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/talker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/talker.dir/clean

CMakeFiles/talker.dir/depend:
	cd /home/krzys/Pulpit/zajecia/lab11/listener/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/krzys/Pulpit/zajecia/lab11/listener /home/krzys/Pulpit/zajecia/lab11/listener /home/krzys/Pulpit/zajecia/lab11/listener/build /home/krzys/Pulpit/zajecia/lab11/listener/build /home/krzys/Pulpit/zajecia/lab11/listener/build/CMakeFiles/talker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/talker.dir/depend

