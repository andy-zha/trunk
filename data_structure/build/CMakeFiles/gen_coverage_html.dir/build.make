# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andy/trunk/data_structure

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andy/trunk/data_structure/build

# Utility rule file for gen_coverage_html.

# Include the progress variables for this target.
include CMakeFiles/gen_coverage_html.dir/progress.make

CMakeFiles/gen_coverage_html:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/andy/trunk/data_structure/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "genarate coverage info to ./cov_result"
	genhtml -o cov_result ds_gtest.cov.info

gen_coverage_html: CMakeFiles/gen_coverage_html
gen_coverage_html: CMakeFiles/gen_coverage_html.dir/build.make

.PHONY : gen_coverage_html

# Rule to build all files generated by this target.
CMakeFiles/gen_coverage_html.dir/build: gen_coverage_html

.PHONY : CMakeFiles/gen_coverage_html.dir/build

CMakeFiles/gen_coverage_html.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gen_coverage_html.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gen_coverage_html.dir/clean

CMakeFiles/gen_coverage_html.dir/depend:
	cd /home/andy/trunk/data_structure/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andy/trunk/data_structure /home/andy/trunk/data_structure /home/andy/trunk/data_structure/build /home/andy/trunk/data_structure/build /home/andy/trunk/data_structure/build/CMakeFiles/gen_coverage_html.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gen_coverage_html.dir/depend

