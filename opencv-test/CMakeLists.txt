
project (opencv-test)

set (CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)


AUX_SOURCE_DIRECTORY(. src)

add_executable(opencv-test ${src})


target_link_libraries(opencv-test ${OpenCV_LIBS})
