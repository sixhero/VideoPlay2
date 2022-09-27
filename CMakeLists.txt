cmake_minimum_required(VERSION 3.22.0)

project(VideoPlay CXX C)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#第三方提供的工具包
add_subdirectory(module)
#视频解码包
add_subdirectory(VideoDes)
#主程序
add_subdirectory(VideoPlay)
#OpenGL的视频展示器
add_subdirectory(VideoGL)