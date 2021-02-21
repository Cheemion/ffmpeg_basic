cmake_minimum_required(VERSION 3.18)
#project name
PROJECT(ffmpeg_basic)
#head file path
INCLUDE_DIRECTORIES(include)
link_directories(lib)
link_libraries(
    avcodec
    avformat
    avfilter
    avdevice
    swresample
    swscale
    avutil
)
ADD_EXECUTABLE(ffmpeg_basic 0_helloword.cpp) 
