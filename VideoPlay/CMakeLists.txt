add_executable(VideoPlay)

target_sources( VideoPlay
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
)

target_link_libraries(VideoPlay 
PRIVATE
    VideoDes
    VideoGL
    spdlog::spdlog_header_only


)