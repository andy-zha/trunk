# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12.2)

if(CMAKE_BUILD_RPATH)
    message("CMAKE_BUILD_RPATH: ${CMAKE_BUILD_RPATH}")
endif()

# 项目信息
project (Calculation)
set (WAF_MAJOR_VERSION 1)
set (WAF_MINOR_VERSION 0)
set (WAF_PATCH_VERSION 0)

# 目录下的所有源文件
aux_source_directory(src CALCULATION_SRC_FILES)
aux_source_directory(gtest CALCULATION_TEST_FILES)

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/hdr)

# 库文件目录
include_directories(${CMAKE_BUILD_RPATH}/include)
link_directories(${CMAKE_BUILD_RPATH}/lib)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -fprofile-arcs -ftest-coverage")

# 指定生成目标
add_executable(Calculation ${CALCULATION_SRC_FILES} ${CALCULATION_TEST_FILES})
target_link_libraries(Calculation PRIVATE gtest)
target_link_libraries(Calculation PRIVATE gcov)
#执行程序
add_custom_target(test
	COMMAND ./Calculation
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
	COMMENT "run test suit.")

add_custom_target(gen_coverage
	COMMAND lcov -d CMakeFiles/Calculation.dir/src -t Calculation -o 'Calculation.cov.info' -b . -c
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
	COMMENT "genarate coverage info")

add_custom_target(gen_coverage_html
	COMMAND genhtml -o cov_result Calculation.cov.info
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
	COMMENT "genarate coverage info to ./cov_result")

add_dependencies(gen_coverage test)
add_dependencies(gen_coverage Calculation)
add_dependencies(gen_coverage_html gen_coverage)
