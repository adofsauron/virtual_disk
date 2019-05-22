# CMake最低版本号要求
cmake_minimum_required(VERSION 3.13)

# 项目信息
project (virtual_disk)

SET(CMAKE_CXX_FLAGS_DEBUG "-g  -o0 -ggdb -Wall -DDEBUG_LOG")
SET(CMAKE_CXX_FLAGS_RELEASE "-g -O3")
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--hash-style=sysv")

#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_BUILD_TYPE Release)

add_subdirectory(base)
add_subdirectory(client)
add_subdirectory(handle)
add_subdirectory(tools)
add_subdirectory(start)
add_subdirectory(cmd)


# 指定生成目标
add_executable (disk  start/start.cpp)
target_link_libraries(disk base client handle tools start cmd)

