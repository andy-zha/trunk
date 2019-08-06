# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12.0)

if(CMAKE_BUILD_RPATH)
    message("CMAKE_BUILD_RPATH: ${CMAKE_BUILD_RPATH}")
endif()

# 项目信息
project (ml_engine)
set (ML_MAJOR_VERSION 1)
set (ML_MINOR_VERSION 0)
set (ML_PATCH_VERSION 0)

# 设置编译选项

if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES DEBUG)) 
	add_definitions(-D_DEBUG_VERSION_ -D_MEMCHECK_)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g -rdynamic") 
endif()	

if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES RELEASE))
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
endif()

if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES GTEST))
	add_definitions(-D_GTEST_)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g -fprofile-arcs -ftest-coverage") 
endif()

# 目录下的所有源文件
aux_source_directory(src ML_ENGINE_SRC_FILES)
aux_source_directory(httpparser ML_ENGINE_HTTPPARSER_SRC_FILES)

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
	aux_source_directory(tools/memcheck ML_ENGINE_MEMCHECK_SRC_FILES)
endif()

if (CMAKE_BUILD_TYPE MATCHES GTEST)
	aux_source_directory(gtest ML_ENGINE_TEST_FILES)
endif()	

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/hdr)
include_directories(${PROJECT_SOURCE_DIR}/httpparser)
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
	include_directories(${PROJECT_SOURCE_DIR}/tools/memcheck)
endif()

# 库文件目录
include_directories(${CMAKE_BUILD_RPATH}/include)
link_directories(${CMAKE_BUILD_RPATH}/lib)

# 指定生成目标
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
	add_executable(ml_engine ${ML_ENGINE_SRC_FILES} 
					${ML_ENGINE_MEMCHECK_SRC_FILES}
					${ML_ENGINE_HTTPPARSER_SRC_FILES}
					${ML_ENGINE_CONSOLE_SRC_FILES})
	target_link_libraries(ml_engine PRIVATE mysqlclient)
	target_link_libraries(ml_engine PRIVATE yajl)
endif()

if (CMAKE_BUILD_TYPE MATCHES RELEASE)
	add_executable(ml_engine ${ML_ENGINE_SRC_FILES}
					${ML_ENGINE_HTTPPARSER_SRC_FILES})
	target_link_libraries(ml_engine PRIVATE mysqlclient)
	target_link_libraries(ml_engine PRIVATE yajl)
endif()

if (CMAKE_BUILD_TYPE MATCHES GTEST)
	#添加编译选项
	add_executable(ml_gtest ${ML_ENGINE_SRC_FILES} 
					${ML_ENGINE_HTTPPARSER_SRC_FILES}
					${ML_ENGINE_TEST_FILES})
	target_link_libraries(ml_gtest PRIVATE gtest)
	target_link_libraries(ml_gtest PRIVATE mysqlclient)
	target_link_libraries(ml_gtest PRIVATE yajl)
	target_link_libraries(ml_gtest PRIVATE gcov)

	#执行程序
	add_custom_target(test
	COMMAND ./ml_gtest
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
	COMMENT "run test suit.")

	add_custom_target(gen_coverage
	COMMAND lcov -d CMakeFiles/ml_gtest.dir/src -t ml_gtest -o 'ml_gtest.cov.info' -b . -c
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
	COMMENT "genarate coverage info")

	add_custom_target(gen_coverage_html
	COMMAND genhtml -o cov_result ml_gtest.cov.info
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
	COMMENT "genarate coverage info to ./cov_result")
	
	add_dependencies(gen_coverage test)
	add_dependencies(gen_coverage ml_gtest)
	add_dependencies(gen_coverage_html gen_coverage)
endif()
