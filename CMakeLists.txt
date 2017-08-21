cmake_minimum_required(VERSION 3.0)
project(meevax VERSION 0.2.4 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++1z -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -O3")
set(CMAKE_CXX_COMPILER clang++) # XXX LOW PORTABILITY
set(CMAKE_INSTALL_PREFIX /usr/local)
set(CMAKE_BUILD_TYPE Debug)

find_package(Boost 1.58.0 REQUIRED COMPONENTS)

file(GLOB ${PROJECT_NAME}_SOURCE_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)
set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)


configure_file(
  ${PROJECT_SOURCE_DIR}/configure/version.hpp.cmake
  ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/configure/version.hpp
)

configure_file(
  ${PROJECT_SOURCE_DIR}/configure/readme.md.cmake
  ${PROJECT_SOURCE_DIR}/README.md
)


include_directories(
  ${${PROJECT_NAME}_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
)

link_directories(
  ${Boost_LIBRARY_DIRS}
)

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCE_LIST}
)

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  cairo
  xcb
  xcb-keysyms
)


install(
  DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/
  DESTINATION include
)

