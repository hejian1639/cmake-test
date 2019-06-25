cmake_minimum_required(VERSION 3.4.1)
project(server)
add_custom_target(server
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

