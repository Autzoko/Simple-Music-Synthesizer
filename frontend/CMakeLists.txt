find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# 设置前端头文件和源文件
set(SYNTHESIZER_FRONTEND_HEADERS
    include/SynthesizerUI.h
)

set(SYNTHESIZER_FRONTEND_SOURCES
    src/SynthesizerUI.cpp
    src/main.cpp
)

add_executable(frontend ${SYNTHESIZER_FRONTEND_HEADERS} ${SYNTHESIZER_FRONTEND_SOURCES})

# 链接 Qt6 和后端库
target_link_libraries(frontend PRIVATE backend Qt6::Core Qt6::Widgets)

# 包含头文件目录
target_include_directories(frontend PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/backend/include
)
