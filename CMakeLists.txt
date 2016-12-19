cmake_minimum_required(VERSION 2.8.12)
project(meevax CXX)

add_definitions("-std=c++11 -Wall")

# set(${PROJECT_NAME}_MAJOR_VERSION 0)
# set(${PROJECT_NAME}_MINOR_VERSION 0)
# set(${PROJECT_NAME}_PATCH_VERSION 0)
#
# set(${PROJECT_NAME}_VERSION
#   ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION}
# )

file(GLOB ${PROJECT_NAME}_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

set(${PROJECT_NAME}_HEADERS
  ${PROJECT_SOURCE_DIR}/include
)

include_directories(
  ${${PROJECT_NAME}_HEADERS}
)

link_directories(
)

add_executable(${PROJECT_NAME}
# add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
)

# set(CMAKE_INSTALL_PREFIX /usr/local)
#
# set_target_properties(${PROJECT_NAME} PROPERTIES
#   VERSION   ${${PROJECT_NAME}_VERSION}
#   SOVERSION ${${PROJECT_NAME}_MAJOR_VERSION}
# )
#
# install(TARGETS ${PROJECT_NAME}
#   LIBRARY DESTINATION lib
# )
#
# install(DIRECTORY ${${PROJECT_NAME}_HEADERS}/
#   DESTINATION include
# )