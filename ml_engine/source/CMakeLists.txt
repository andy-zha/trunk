# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12.0)

if(CMAKE_BUILD_RPATH)
    message("CMAKE_BUILD_RPATH: ${CMAKE_BUILD_RPATH}")
endif()

# 项目信息
project (ml_engine)
set (WAF_MAJOR_VERSION 1)
set (WAF_MINOR_VERSION 0)
set (WAF_PATCH_VERSION 0)

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
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g") 
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
	target_link_libraries(ml_engine PRIVATE pthread)
	target_link_libraries(ml_engine PRIVATE mysqlclient)
	target_link_libraries(ml_engine PRIVATE http_parser)
endif()

if (CMAKE_BUILD_TYPE MATCHES RELEASE)
	add_executable(ml_engine ${ML_ENGINE_SRC_FILES}
					${ML_ENGINE_HTTPPARSER_SRC_FILES})
	target_link_libraries(ml_engine PRIVATE pthread)
	target_link_libraries(ml_engine PRIVATE mysqlclient)
	target_link_libraries(ml_engine PRIVATE http_parser)
endif()

if (CMAKE_BUILD_TYPE MATCHES GTEST)
	add_executable(ml_gtest ${ML_ENGINE_SRC_FILES} ${ML_ENGINE_TEST_FILES})
	target_link_libraries(ml_gtest PRIVATE gtest)
	target_link_libraries(ml_gtest PRIVATE mysqlclient)
endif()
