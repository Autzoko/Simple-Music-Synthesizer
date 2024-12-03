cmake_minimum_required(VERSION 3.10)
project(SynthesizerBackend)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/components
    ${PROJECT_SOURCE_DIR}/include/utils
    ${PROJECT_SOURCE_DIR}/lib/RtAudio
)

file(GLOB COMPONENT_SOURCES
    ${PROJECT_SOURCE_DIR}/src/components/*.cpp
)

file(GLOB UTIL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/utils/*.cpp
)

set(SOURCES
    ${COMPONENT_SOURCES}
    ${UTIL_SOURCES}
    ${PROJECT_SOURCE_DIR}/src/Synthesizer.cpp
    ${PROJECT_SOURCE_DIR}/lib/RtAudio/RtAudio.cpp
    ${PROJECT_SOURCE_DIR}/main.cpp 
)

add_executable(synth_test ${SOURCES})


# Windows
if (WIN32)
    target_link_libraries(synth_test winmm)
endif()

# MacOS
if (APPLE)
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
endif()

# Linux
if (UNIX AND NOT APPLE)
    target_link_libraries(synth_test asound)
endif()
