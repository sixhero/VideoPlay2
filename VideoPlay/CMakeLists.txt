add_executable(VideoPlay)

target_sources( VideoPlay
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
    # stb_image.h
)

target_link_libraries(VideoPlay 
PRIVATE
    VideoDes
    VideoGL
)