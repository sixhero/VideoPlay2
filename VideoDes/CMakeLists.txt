add_library(VideoDes SHARED)

set(CMAKE_CXX_STANDARD 11)

target_sources( VideoDes
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/VideoDes.cpp
    VideoDes.h
    VideoExport.cpp
    VideoExport.h
)
if(WIN32)
    target_include_directories(VideoDes
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/win/include
    )

    target_link_libraries(VideoDes 
    PRIVATE 
    #日志模块
        spdlog::spdlog_header_only
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/win/lib/avcodec.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/win/lib/avdevice.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/win/lib/avfilter.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/win/lib/avformat.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/win/lib/avutil.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/win/lib/swresample.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/win/lib/swscale.lib
    )
elseif(UNIX)
    target_include_directories(VideoDes
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/include
    )

    target_link_libraries(VideoDes 
    PRIVATE 
    #日志模块
        spdlog::spdlog_header_only
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/lib/libavcodec.so
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/lib/libavdevice.so
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/lib/libavfilter.so
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/lib/libavformat.so
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/lib/libavutil.so
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/lib/libswresample.so
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/lib/libswscale.so
    )
endif(WIN32)

#设置FFMpeg的共享库路径(*.so)
set(CMAKE_BUILD_RPATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)