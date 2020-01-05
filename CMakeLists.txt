cmake_minimum_required(VERSION 2.8.6)

project (cmake-test)

#SET(CMAKE_C_COMPILER "gcc-8")
#SET(CMAKE_CXX_COMPILER "g++-8")

add_subdirectory(connection-test)
add_subdirectory(server)
add_subdirectory(opencv-test)
add_subdirectory(atomic)
add_subdirectory(lock)
