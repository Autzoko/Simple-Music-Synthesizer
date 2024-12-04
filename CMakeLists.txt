cmake_minimum_required(VERSION 3.10)
project(Synthesizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加子目录
add_subdirectory(backend)
add_subdirectory(frontend)
