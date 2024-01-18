cmake_minimum_required(VERSION 3.10)
project(Vino VERSION 1.0 LANGUAGES CXX)

add_definitions(-DUNICODE -D_UNICODE)

set(DIRECTX_INCLUDE_DIR "C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/um")
set(DIRECTX_LIBRARY_DIR "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x64")


set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")


file(GLOB_RECURSE SOURCES
	src/*.cpp
	lib/*.cpp)

add_executable(vino_example ${SOURCES})
target_include_directories(vino_example PRIVATE include)

include_directories(${DIRECTX_INCLUDE_DIR})
target_link_libraries(vino_example
    "${DIRECTX_LIBRARY_DIR}/d3d11.lib"
    "${DIRECTX_LIBRARY_DIR}/dxgi.lib"
    "${DIRECTX_LIBRARY_DIR}/d3dcompiler.lib"
    "${DIRECTX_LIBRARY_DIR}/dwrite.lib"
    "${DIRECTX_LIBRARY_DIR}/d2d1.lib"
    # Add other DirectX libraries as needed
)
