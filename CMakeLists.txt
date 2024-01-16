cmake_minimum_required(VERSION 3.10)
project(Vino VERSION 1.0 LANGUAGES CXX)

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")

file(GLOB_RECURSE SOURCES
	src/*.cpp
	lib/*.cpp)

add_executable(vino_example ${SOURCES})
target_include_directories(vino_example PRIVATE include)
