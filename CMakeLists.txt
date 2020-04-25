cmake_minimum_required(VERSION 3.10)

project(virtmgr)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE sourcefiles
    "*.h"
    "*.cc"
)

add_executable(virtmgr ${sourcefiles})