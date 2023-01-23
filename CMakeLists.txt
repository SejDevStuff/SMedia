cmake_minimum_required(VERSION 2.8)
project( SMedia )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( test test.cpp s_img.cpp s_vid.cpp)
target_link_libraries( test ${OpenCV_LIBS} )

add_executable( SImgView SImgView.cpp s_img.cpp)
target_link_libraries( SImgView ${OpenCV_LIBS} SDL2)

add_executable( SVidPlay SVidPlay.cpp s_vid.cpp s_img.cpp)
target_link_libraries( SVidPlay ${OpenCV_LIBS} SDL2)