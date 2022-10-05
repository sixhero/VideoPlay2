add_library(VideoGL SHARED)

set(CMAKE_CXX_STANDARD 11)

target_sources( VideoGL
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/VideoGL.cpp
    VideoGL.h
    VideoGLExport.cpp
    VideoGLExport.h

PRIVATE
    # glad.c库文件需要使用源码参与编译
    ${CMAKE_CURRENT_SOURCE_DIR}/glad.c

)
if(WIN32)
    target_include_directories(VideoGL
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(VideoGL 
    PRIVATE 
    #日志模块
        spdlog::spdlog_header_only
        glfw
    PRIVATE
        # opengl32.lib
    )
elseif(UNIX)
    target_include_directories(VideoGL
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(VideoGL 
    PRIVATE 
    #日志模块
        spdlog::spdlog_header_only
        glfw

    PRIVATE

    )
endif(WIN32)