cmake_minimum_required(VERSION 3.10)
project(SynthesizerBackend)

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

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/components
    ${PROJECT_SOURCE_DIR}/include/components/effects
    ${PROJECT_SOURCE_DIR}/include/utils
    ${PROJECT_SOURCE_DIR}/lib/RtAudio
    ${PROJECT_SOURCE_DIR}/lib/RtAudio/include
)

file(GLOB COMPONENT_SOURCES
    ${PROJECT_SOURCE_DIR}/src/components/*.cpp
)

file(GLOB UTIL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/components/effects/*.cpp
)

file(GLOB EFFECTS_SOURCES
    ${PROJECT_SOURCE_DIR}/src/utils/*.cpp
)

file(GLOB ASIO_SOURCES
    ${PROJECT_SOURCE_DIR}/lib/RtAudio/include/*.cpp
)


set(SOURCES
    ${COMPONENT_SOURCES}
    ${UTIL_SOURCES}
    ${EFFECTS_SOURCES}
    ${ASIO_SOURCES}
    ${PROJECT_SOURCE_DIR}/src/Synthesizer.cpp
    ${PROJECT_SOURCE_DIR}/lib/RtAudio/RtAudio.cpp
    ${PROJECT_SOURCE_DIR}/main.cpp
)

add_executable(synth_test ${SOURCES})

if (WIN32)
    add_definitions(-D__WINDOWS_DS__)
    target_link_libraries(synth_test winmm dsound -lwinpthread)
elseif (APPLE)
    find_library(COREAUDIO_LIBRARY CoreAudio)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(AUDIOUNIT_LIBRARY AudioUnit)
    find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox)
    target_link_libraries(synth_test
        ${COREAUDIO_LIBRARY}
        ${COREFOUNDATION_LIBRARY}
        ${AUDIOUNIT_LIBRARY}
        ${AUDIOTOOLBOX_LIBRARY}
    )
elseif (UNIX AND NOT APPLE)
    target_link_libraries(synth_test asound pthread)
endif()
