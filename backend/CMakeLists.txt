cmake_minimum_required(VERSION 3.10)

#set(CMAKE_C_COMPILER "C:/Program Files/mingw-w64/mingw64/bin/gcc.exe")
#set(CMAKE_CXX_COMPILER "C:/Program Files/mingw-w64/mingw64/bin/g++.exe")

set(CMAKE_C_COMPILER "X:/Softwares/Tools/mingw1310_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "X:/Softwares/Tools/mingw1310_64/bin/g++.exe")

project(backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
message("Debug mode enabled")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -DDEBUG")
else()
message("Release mode enabled")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/include)

file(GLOB SYNTH_BACKEND_HEADERS 
${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
${CMAKE_CURRENT_SOURCE_DIR}/include/components/*.h
${CMAKE_CURRENT_SOURCE_DIR}/include/components/effects/*.h
${CMAKE_CURRENT_SOURCE_DIR}/include/utils/*.h
)
file(GLOB_RECURSE SYNTH_BACKEND_SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/components/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/components/effects/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp
)

file(GLOB PORTAUDIO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/src/common/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/src/os/win/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/src/hostapi/dsound/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/src/hostapi/wasapi/*.c
)

file(GLOB RTMIDI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/RtMidi/*.cpp
)

add_definitions(-DPA_USE_DS=1 -DPA_USE_WASAPI=1 -DPA_USE_ASIO=0 -DPA_USE_ALSA=0 -DPA_USE_OSS=0)

add_library(
    backend STATIC
    ${SYNTH_BACKEND_HEADERS}
    ${SYNTH_BACKEND_SOURCES}
    ${PORTAUDIO_SOURCES}
    ${RTMIDI_SOURCES}
)

target_include_directories(
    backend PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/components
    ${CMAKE_CURRENT_SOURCE_DIR}/include/components/effects
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/src/os/win
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/RtMidi
)

target_compile_features(backend PUBLIC cxx_std_20)



if (WIN32)
    target_link_libraries(backend winmm dsound -lwinpthread)
    add_definitions(-D__WINDOWS_MM__)
elseif (APPLE)
    find_library(COREAUDIO_LIBRARY CoreAudio)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(AUDIOUNIT_LIBRARY AudioUnit)
    find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox)
    target_link_libraries(backend
        ${COREAUDIO_LIBRARY}
        ${COREFOUNDATION_LIBRARY}
        ${AUDIOUNIT_LIBRARY}
        ${AUDIOTOOLBOX_LIBRARY}
    )
elseif (UNIX AND NOT APPLE)
    target_link_libraries(backend asound pthread)
endif()

