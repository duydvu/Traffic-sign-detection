cmake_minimum_required(VERSION 2.8)
project (sign-recognition)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

include_directories(./tiny-dnn)
include(cotire)

add_executable(main main.cpp)

find_package(OpenCV REQUIRED)
find_package(Threads)
find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

target_link_libraries( main ${OpenCV_LIBS} )
target_link_libraries ( main ${CMAKE_THREAD_LIBS_INIT})

cmake_policy(SET CMP0015 NEW)

include_directories(... ${Boost_INCLUDE_DIRS})

link_directories(... ${Boost_LIBRARY_DIRS})

target_link_libraries(main ${Boost_LIBRARIES})

cotire(main)

