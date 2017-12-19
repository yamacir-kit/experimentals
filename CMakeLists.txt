cmake_minimum_required(VERSION 3.5)

project(meevax VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-variable -std=c++17")

# set(CXX_STANDARD 17)
# set(CXX_STANDARD_REQUIRED ON)

file(GLOB ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Boost REQUIRED COMPONENTS system thread)

string(TIMESTAMP ${PROJECT_NAME}_TIMESTAMP "%Y/%m/%d %H:%M:%S")
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/configure/version.hpp.cmake)
  file(MAKE_DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/configure)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/configure/version.hpp.cmake
    ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/configure/version.hpp
  )
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/configure/README.md.cmake)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/configure/README.md.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
  )
endif()

include_directories(
  ${${PROJECT_NAME}_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
)

link_directories(
  ${Boost_LIBRARY_DIRS}
)

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  cairo
  xcb
  stdc++fs
)

install(
  DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/
  DESTINATION include
)

