cmake_minimum_required(VERSION "3.17")

project(assignment-two)

set(OpenCV_DIR ~/opencv/build)

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(CCL CCL.cpp)

add_executable(gray2binary gray2binary.cpp)

target_link_libraries(CCL ${OpenCV_LIBS} )

target_link_libraries(gray2binary ${OpenCV_LIBS} )