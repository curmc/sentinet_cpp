cmake_minimum_required(VERSION 3.5)
project(core)

set(CMAKE_INSTALL_PREFIX "/opt/sentinet_cpp/${PROJECT_NAME}")
set(ARCHITECTURE "x86_64")

find_package(OpenCV REQUIRED )

# Used for language analysis (and clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Third party sentinet message package
set(MSG_PKG "${PROJECT_SOURCE_DIR}/third_party/sentinet_message_pkg")

# Include cpp options
include("${PROJECT_SOURCE_DIR}/.cmake/cpp.cmake")
include_directories("${OpenCV_INCLUDE_DIRS}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ARCHITECTURE}/${PROJECT_NAME}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ARCHITECTURE}/${PROJECT_NAME}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ARCHITECTURE}/${PROJECT_NAME}/bin")

# Find the necessary libraries
find_library(PTHREAD_LIB pthread)
find_library(ZMQ_LIB zmq)
find_library(APRIL_TAG apriltag)

# Include some files
include_directories(include)
include_directories("${MSG_PKG}/c/include")

# add_subdirectory(third_party)
add_subdirectory(src)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

