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
include time/CMakeFiles/Time.dir/depend.make

# Include the progress variables for this target.
include time/CMakeFiles/Time.dir/progress.make

# Include the compile flags for this target's objects.
include time/CMakeFiles/Time.dir/flags.make

time/CMakeFiles/Time.dir/t.c.o: time/CMakeFiles/Time.dir/flags.make
time/CMakeFiles/Time.dir/t.c.o: ../time/t.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object time/CMakeFiles/Time.dir/t.c.o"
	cd /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/time && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Time.dir/t.c.o   -c /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/time/t.c

time/CMakeFiles/Time.dir/t.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Time.dir/t.c.i"
	cd /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/time && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/time/t.c > CMakeFiles/Time.dir/t.c.i

time/CMakeFiles/Time.dir/t.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Time.dir/t.c.s"
	cd /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/time && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/time/t.c -o CMakeFiles/Time.dir/t.c.s

# Object files for target Time
Time_OBJECTS = \
"CMakeFiles/Time.dir/t.c.o"

# External object files for target Time
Time_EXTERNAL_OBJECTS =

time/libTime.a: time/CMakeFiles/Time.dir/t.c.o
time/libTime.a: time/CMakeFiles/Time.dir/build.make
time/libTime.a: time/CMakeFiles/Time.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libTime.a"
	cd /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/time && $(CMAKE_COMMAND) -P CMakeFiles/Time.dir/cmake_clean_target.cmake
	cd /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/time && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Time.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
time/CMakeFiles/Time.dir/build: time/libTime.a

.PHONY : time/CMakeFiles/Time.dir/build

time/CMakeFiles/Time.dir/clean:
	cd /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/time && $(CMAKE_COMMAND) -P CMakeFiles/Time.dir/cmake_clean.cmake
.PHONY : time/CMakeFiles/Time.dir/clean

time/CMakeFiles/Time.dir/depend:
	cd /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/krzys/Pulpit/zajecia/lab2/cwiczenie4 /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/time /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/time /home/krzys/Pulpit/zajecia/lab2/cwiczenie4/build/time/CMakeFiles/Time.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : time/CMakeFiles/Time.dir/depend

