cmake_minimum_required(VERSION 3.5)
project(sentinet)

set(CMAKE_INSTALL_PREFIX "/opt/${PROJECT_NAME}_cpp")
set(ARCHITECTURE "x86_64")

# Used for language analysis (and clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Third party sentinet message package
set(MSG_PKG "${PROJECT_SOURCE_DIR}/third_party/sentinet_message_pkg")

# Include cpp options
include("${PROJECT_SOURCE_DIR}/.cmake/cpp.cmake")

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ARCHITECTURE}/${PROJECT_NAME}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ARCHITECTURE}/${PROJECT_NAME}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ARCHITECTURE}/${PROJECT_NAME}/bin")

# Find the necessary libraries
find_library(PTHREAD_LIB pthread)
find_library(ZMQ_LIB zmq)

# Include some files
include_directories(include)
include_directories("${PROJECT_SOURCE_DIR}/third_party/sentinet_message_pkg/c/include")



# Build our main libraries

# MESSAGE LIBRARY - Pipes and serialization
file(GLOB_RECURSE MESSAGE_SRC "${PROJECT_SOURCE_DIR}/src/messages/*.cpp")
set(MESSAGE_SRC "${MESSAGE_SRC};${PROJECT_SOURCE_DIR}/third_party/sentinet_message_pkg/c/src/messages.cpp")
set(MESSAGE_LIB messages)
add_library(${MESSAGE_LIB} STATIC ${MESSAGE_SRC})
target_link_libraries(${MESSAGE_LIB})


# CONTROL SOURCE - Control Client
file(GLOB_RECURSE CONTROL_SRC "${PROJECT_SOURCE_DIR}/src/control/*.cpp")
set(CONTROL_LIB control)
add_library(${CONTROL_LIB} STATIC ${CONTROL_SRC})
target_link_libraries(${CONTROL_LIB} ${MESSAGE_LIB} ${ZMQ_LIB} ${PTHREAD_LIB})


# IMPLIMENTATION SOURCE - Kermit and kernel stuff
file(GLOB_RECURSE KERNEL_SRC "${PROJECT_SOURCE_DIR}/src/kernel/*.cpp")
set(KERNEL_LIB kernel)
add_library(${KERNEL_LIB} STATIC ${KERNEL_SRC})
target_link_libraries(${KERNEL_LIB})


if(BUILD_EXECUTABLE)
  add_executable(serial_example "${PROJECT_SOURCE_DIR}/src/impl/kernel_examples/serial_example.cpp")
  target_link_libraries(serial_example ${KERNEL_LIB})

  add_executable(kermit "${PROJECT_SOURCE_DIR}/src/impl/examples/kermit.cpp")
  target_link_libraries(kermit ${KERNEL_LIB} ${CONTROL_LIB} ${YAML_CPP})

  add_executable(all_in_one "${PROJECT_SOURCE_DIR}/src/impl/examples/all_in_one.cpp")
  target_link_libraries(all_in_one ${KERNEL_LIB} ${CONTROL_LIB} ${YAML_CPP})

include_directories("${MSG_PKG}/c/include")

add_subdirectory(third_party)
add_subdirectory(src)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})
