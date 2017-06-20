cmake_minimum_required(VERSION 3.0)
project(meevax VERSION 0.2.1 LANGUAGES CXX)

add_definitions("-std=c++14 -Wall -O3")

# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER clang++)

file(GLOB ${PROJECT_NAME}_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

set(${PROJECT_NAME}_HEADER_DIR
  ${PROJECT_SOURCE_DIR}/include
)

configure_file(
  ${PROJECT_SOURCE_DIR}/configure/version.hpp.cmake
  ${${PROJECT_NAME}_HEADER_DIR}/${PROJECT_NAME}/version.hpp
)

configure_file(
  ${PROJECT_SOURCE_DIR}/configure/readme.md.cmake
  ${PROJECT_SOURCE_DIR}/readme.md
)

include_directories(
  ${${PROJECT_NAME}_HEADER_DIR}
)

link_directories(
  /usr/X11R6/lib
)

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  X11
  cairo
)

# set(CMAKE_INSTALL_PREFIX /usr/local)
#
# set_target_properties(${PROJECT_NAME} PROPERTIES
#   VERSION   ${PROJECT_VERSION}
#   SOVERSION ${PROJECT_VERSION_MAJOR}
# )
#
# install(TARGETS ${PROJECT_NAME}
#   LIBRARY DESTINATION lib
# )
#
# install(DIRECTORY ${${PROJECT_NAME}_HEADERS}/
#   DESTINATION include
# )
