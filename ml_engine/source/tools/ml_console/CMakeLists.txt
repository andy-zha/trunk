# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12.0)

if(CMAKE_BUILD_RPATH)
    message("CMAKE_BUILD_RPATH: ${CMAKE_BUILD_RPATH}")
endif()

# 项目信息
project (ml_console)
set (CONSOLE_MAJOR_VERSION 1)
set (CONSOLE_MINOR_VERSION 0)
set (CONSOLE_PATCH_VERSION 0)

# 设置编译选项
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g -rdynamic") 

# 目录下的所有源文件
aux_source_directory(src ML_CONSOLE_SRC_FILES)

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/hdr)

# 库文件目录
include_directories(${CMAKE_BUILD_RPATH}/include)
link_directories(${CMAKE_BUILD_RPATH}/lib)

add_executable(ml_console ${ML_CONSOLE_SRC_FILES})
target_link_libraries(ml_console PRIVATE mysqlclient)
