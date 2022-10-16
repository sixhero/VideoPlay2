add_executable(TestAudio)

target_sources( TestAudio
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/TestAudio.cpp 
    VideoDes.h
    VideoDes.cpp
)

set(ffmpeg_path ${CMAKE_SOURCE_DIR}/VideoDes/win CACHE INTERNAL "ffmpeg库的路径")

message(INFO,"${ffmpeg_path}/win/include")

target_include_directories(TestAudio
PRIVATE
    ${ffmpeg_path}/include
)

target_link_libraries(TestAudio 
PRIVATE 
#日志模块
    spdlog::spdlog_header_only
PRIVATE
    ${ffmpeg_path}/lib/avcodec.lib
    ${ffmpeg_path}/lib/avdevice.lib
    ${ffmpeg_path}/lib/avfilter.lib
    ${ffmpeg_path}/lib/avformat.lib
    ${ffmpeg_path}/lib/avutil.lib
    ${ffmpeg_path}/lib/swresample.lib
    ${ffmpeg_path}/lib/swscale.lib
)
