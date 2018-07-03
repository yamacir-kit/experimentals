cmake_minimum_required(VERSION 3.5.1)

project(meevax VERSION 0.3.1 LANGUAGES CXX)

set(${PROJECT_NAME}_CONFIGURE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/configure)
set(${PROJECT_NAME}_DOXYGEN_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/doxygen)
set(${PROJECT_NAME}_INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_BUILD_TYPE release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wdelete-non-virtual-dtor -Winline -std=c++17 -O2")

find_package(Boost REQUIRED)
# find_package(Cairo REQUIRED)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  if(EXISTS ${${PROJECT_NAME}_CONFIGURE_DIR}/doxyfile)
    configure_file(${${PROJECT_NAME}_CONFIGURE_DIR}/doxyfile ${${PROJECT_NAME}_DOXYGEN_DIR}/doxyfile)
    add_custom_target(doxygen
      COMMAND ${DOXYGEN_EXECUTABLE} "${${PROJECT_NAME}_DOXYGEN_DIR}/doxyfile"
      WORKING_DIRECTORY "${${PROJECT_NAME}_DOXYGEN_DIR}"
    )
  endif()
endif()

if(EXISTS ${${PROJECT_NAME}_CONFIGURE_DIR}/version.hpp)
  file(MAKE_DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/configure)
  string(TIMESTAMP ${PROJECT_NAME}_TIMESTAMP "%Y/%m/%d %H:%M:%S")
  configure_file(
    ${${PROJECT_NAME}_CONFIGURE_DIR}/configure.hpp
    ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/configure/configure.hpp
  )
endif()

if(EXISTS ${${PROJECT_NAME}_CONFIGURE_DIR}/README.md)
  configure_file(${${PROJECT_NAME}_CONFIGURE_DIR}/README.md ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
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
  dl
  xcb
  stdc++fs
)

install(
  DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

