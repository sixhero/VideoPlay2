add_library(VideoGL SHARED)

set(CMAKE_CXX_STANDARD 11)

target_sources( VideoGL
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/VideoGL.cpp
    VideoGL.h
    VideoGLExport.cpp

PUBLIC
    # 向外暴露使用的头文件
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VideoGL/VideoGLExport.h

PRIVATE
    # glad.c库文件需要使用源码参与编译
    ${CMAKE_CURRENT_SOURCE_DIR}/glad.c

)
if(WIN32)
    target_include_directories(VideoGL
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
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
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
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