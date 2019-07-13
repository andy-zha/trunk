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

# 目录下的所有源文件
aux_source_directory(src ML_ENGINE_SRC_FILES)
aux_source_directory(gtest ML_ENGINE_TEST_FILES)

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/hdr)

# 库文件目录
include_directories(${CMAKE_BUILD_RPATH}/include)
link_directories(${CMAKE_BUILD_RPATH}/lib)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# 指定生成目标
add_executable(ml_engine ${ML_ENGINE_SRC_FILES})
#target_link_libraries(ml_engine PRIVATE gtest)
