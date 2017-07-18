cmake_minimum_required(VERSION 3.0)
project(meevax VERSION 0.2.4 LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall -Wextra -O3 -pipe")
set(CMAKE_BUILD_TYPE Debug)

find_package(Boost 1.58.0 REQUIRED COMPONENTS)

file(GLOB ${PROJECT_NAME}_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

configure_file(
  ${PROJECT_SOURCE_DIR}/configure/version.hpp.cmake
  ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/version.hpp
)

configure_file(
  ${PROJECT_SOURCE_DIR}/configure/readme.md.cmake
  ${PROJECT_SOURCE_DIR}/readme.md
)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
)

link_directories(
  /usr/X11R6/lib
)

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  X11
  cairo
)

