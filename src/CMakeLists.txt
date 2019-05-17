# CMake最低版本号要求
cmake_minimum_required(VERSION 3.13)

# 项目信息
project (virtual_disk)

SET(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -Wall -Werror -Wno-unknown-pragmas")
SET(CMAKE_CXX_FLAGS_RELEASE "-g -O0")
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--hash-style=sysv")
#add_definitions(-DENABLE_MONGODB)
add_definitions(-DDEBUG_LOG)


add_subdirectory(base)
add_subdirectory(client)
add_subdirectory(handle)
add_subdirectory(tools)
add_subdirectory(start)
add_subdirectory(cmd)


# 指定生成目标
add_executable (disk  start/start.cpp)
target_link_libraries(disk base client handle tools start cmd)

