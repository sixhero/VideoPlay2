add_library(AudioMini SHARED)

set(CMAKE_CXX_STANDARD 11)

target_sources(AudioMini 
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioMini.cpp
    AudioMini.h
)

target_include_directories(AudioMini
    PRIVATE
    miniaudio_path
)

# set(INCLUDE_DIRECTORIES miniaudio_path)