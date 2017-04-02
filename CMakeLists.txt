cmake_minimum_required(VERSION 3.0)
project(meevax VERSION 0.0.0 LANGUAGES CXX)

add_definitions("-std=c++14 -Wall")

# find_package(Qt5Widgets REQUIRED)

file(GLOB ${PROJECT_NAME}_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

set(${PROJECT_NAME}_HEADER_DIR
  ${PROJECT_SOURCE_DIR}/include
)

configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/cmake_config.hpp.cmake
  ${${PROJECT_NAME}_HEADER_DIR}/${PROJECT_NAME}/cmake_config.hpp
)

include_directories(
  ${${PROJECT_NAME}_HEADER_DIR}
)

link_directories(
)

add_executable(${PROJECT_NAME}
# add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  # Qt5::Widgets
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
