cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "X:/Softwares/Tools/mingw1310_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "X:/Softwares/Tools/mingw1310_64/bin/g++.exe")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


project(frontend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
set(QT6_DIR "X:/Softwares/6.8.1/mingw_64/lib/cmake/Qt6")

# 设置前端头文件和源文件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# 自动查找所有的 .h 和 .cpp 文件
file(GLOB_RECURSE SYNTHESIZER_FRONTEND_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SYNTHESIZER_FRONTEND_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# 添加可执行文件
add_executable(frontend
    ${SYNTHESIZER_FRONTEND_HEADERS}
    ${SYNTHESIZER_FRONTEND_SOURCES}
)

# 链接后端库和 Qt6 库
target_link_libraries(frontend PRIVATE backend Qt6::Core Qt6::Widgets)

# 包含头文件目录
target_include_directories(frontend PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/backend/include
)
