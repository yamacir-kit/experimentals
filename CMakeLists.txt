cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)

project(meevax VERSION 0.3.0 LANGUAGES CXX)


# ==============================================================
#  Compile Configuration
# ==============================================================
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-variable -O3")

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)


# if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_COMPILER_IS_GNUCXX}")
#   include(CheckCXXCompilerFlag)
#
#   check_cxx_compiler_flag(-std=c++17 SUPPORTS_STD_CXX17)
#   check_cxx_compiler_flag(-std=c++1z SUPPORTS_STD_CXX1Z)
#   check_cxx_compiler_flag(-std=c++14 SUPPORTS_STD_CXX14)
#   check_cxx_compiler_flag(-std=c++1y SUPPORTS_STD_CXX1Y)
#   check_cxx_compiler_flag(-std=c++11 SUPPORTS_STD_CXX11)
#   check_cxx_compiler_flag(-std=c++0x SUPPORTS_STD_CXX0X)
#
#   if(SUPPORTS_STD_CXX17)
#     message(STATUS "activated compiler flag -std=c++17")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#     set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c++17")
#   elseif(SUPPORTS_STD_CXX1Z)
#     message(STATUS "activated compiler flag -std=c++1z")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
#     set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c++1z")
#   elseif(SUPPORTS_STD_CXX14)
#     message(STATUS "activated compiler flag -std=c++14")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
#     set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c++14")
#   elseif(SUPPORTS_STD_CXX1Y)
#     message(STATUS "activated compiler flag -std=c++1y")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
#     set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c++1y")
#   elseif(SUPPORTS_STD_CXX11)
#     message(STATUS "activated compiler flag -std=c++11")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#     set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c++11")
#   elseif(SUPPORTS_STD_CXX0X)
#     message(STATUS "activated compiler flag -std=c++0x")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#     set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c++0x")
#   else()
#     message(FATAL_ERROR "your compiler is too old")
#   endif()
# else()
#   message(FATAL_ERROR "this program guarantees compilability only with GCC or Clang")
# endif()

file(GLOB ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
message(STATUS "source files: " ${${PROJECT_NAME}_SOURCES})

set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
message(STATUS "include directory: " ${${PROJECT_NAME}_INCLUDE_DIR})


# ==============================================================
#  Package Dependencies
# ==============================================================
find_package(Boost 1.58.0 REQUIRED COMPONENTS)


# ==============================================================
#  File Configuration
# ==============================================================
string(TIMESTAMP ${PROJECT_NAME}_TIMESTAMP "%Y/%m/%d %H:%M:%S")

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/configure/version.hpp.cmake
  ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/configure/version.hpp
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/configure/README.md.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/README.md
)


# ==============================================================
#  Main
# ==============================================================
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
)

install(
  DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/
  DESTINATION include
)

