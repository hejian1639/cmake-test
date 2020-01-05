
project (lock-test)

set (CMAKE_CXX_STANDARD 11)

AUX_SOURCE_DIRECTORY(. src)

add_executable(lock ${src})
