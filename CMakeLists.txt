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
ADD_EXECUTABLE(0_helloword 0_helloword.cpp ) 
ADD_EXECUTABLE(2_remuxing  "2_remuxing.cpp") 
ADD_EXECUTABLE(3_log  "3_log.c") 
ADD_EXECUTABLE(4_deleteAndrename  "4_deleteAndrename.c") 
ADD_EXECUTABLE(5_meta  "5_meta.c") 
ADD_EXECUTABLE(6_audioData  "6_audioData.c") 
ADD_EXECUTABLE(7_H264Data "7_H264Data.c") 







