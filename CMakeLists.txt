cmake_minimum_required(VERSION 3.5.1)

project(meevax VERSION 0.3.1 LANGUAGES CXX)

set(${PROJECT_NAME}_CONFIGURE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/configure)
set(${PROJECT_NAME}_INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-variable -std=c++17")

find_package(Boost REQUIRED COMPONENTS system thread)
# find_package(Cairo REQUIRED)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  if(EXISTS ${${PROJECT_NAME}_CONFIGURE_DIR}/doxyfile.cmake)
    configure_file(
      ${${PROJECT_NAME}_CONFIGURE_DIR}/doxyfile.cmake
      ${${PROJECT_NAME}_CONFIGURE_DIR}/doxyfile
    )
    add_custom_target(doxygen
      ${DOXYGEN_EXECUTABLE} "${${PROJECT_NAME}_CONFIGURE_DIR}/doxyfile"
    )
  endif()
endif()

if(EXISTS ${${PROJECT_NAME}_CONFIGURE_DIR}/version.hpp.cmake)
  file(MAKE_DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/configure)
  string(TIMESTAMP ${PROJECT_NAME}_TIMESTAMP "%Y/%m/%d %H:%M:%S")
  configure_file(
    ${${PROJECT_NAME}_CONFIGURE_DIR}/version.hpp.cmake
    ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/configure/version.hpp
  )
endif()

if(EXISTS ${${PROJECT_NAME}_CONFIGURE_DIR}/README.md.cmake)
  configure_file(
    ${${PROJECT_NAME}_CONFIGURE_DIR}/README.md.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
  )
endif()

include_directories(
  ${${PROJECT_NAME}_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  # ${Cairo_INCLUDE_DIRS}
)

link_directories(
  ${Boost_LIBRARY_DIRS}
  # ${Cairo_LIBRARY_DIRS}
)

file(GLOB ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  # ${Cairo_LIBRARIES}
  # cairo
  xcb
  stdc++fs
)

install(
  DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

