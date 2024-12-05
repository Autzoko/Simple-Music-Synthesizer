cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "C:/Program Files/mingw-w64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/mingw-w64/mingw64/bin/g++.exe")

project(Synthesizer)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加子目录
add_subdirectory(backend)
add_subdirectory(frontend)
