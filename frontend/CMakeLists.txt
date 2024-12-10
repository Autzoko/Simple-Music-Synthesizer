cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "X:/Softwares/Tools/mingw1310_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "X:/Softwares/Tools/mingw1310_64/bin/g++.exe")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_PREFIX_PATH "X:/Softwares/6.8.1/mingw_64/lib")
set(QT6_DIR "X:/Softwares/6.8.1/mingw_64/lib/cmake/Qt6")

project(frontend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(Qt6 REQUIRED COMPONENTS Gui Core Widgets)

FILE(GLOB SOURCE_FILES "./src/*.cpp")
FILE(GLOB HEADER_FILES "./include/*.h")
FILE(GLOB UI_FILES "./resources/*.ui")

qt_wrap_cpp(MOC_FILES ${HEADER_FILES})
qt_wrap_ui(UIC_FILES ${UI_FILES})

add_executable(
    frontend
    ${MOC_FILES}
    ${UIC_FILES}
    ${SOURCE_FILES}
)

target_link_libraries(frontend PRIVATE backend Qt6::Gui Qt6::Core Qt6::Widgets winmm dsound)

target_include_directories(frontend PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/backend/include
)