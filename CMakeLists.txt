cmake_minimum_required(VERSION 3.5)
project(cpp)

# Used for language analysis (and clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include some cmake files I found on github
include("${PROJECT_SOURCE_DIR}/.cmake/cpp.cmake")
include("${PROJECT_SOURCE_DIR}/.cmake/GitUtils.cmake")

option(CLONE_MSGS "Clone messages into message folder" OFF)

# Clones sentinet_message_pkg into smp
if(CLONE_MSGS)
  git_clone(
    PROJECT_NAME  sentinet_msgs
    GIT_URL       https://github.com/curmc/sentinet_message_pkg.git
    GIT_BRANCH    release
    DIRECTORY     "${PROJECT_SOURCE_DIR}/third_party"
  )
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Find the necessary libraries
find_library(PTHREAD_LIB pthread)
find_library(ZMQ_LIB zmq)



# Include some files
include_directories(include)
include_directories("${PROJECT_SOURCE_DIR}/third_party/sentinet_message_pkg/c/include")



# Build our main libraries

# MESSAGE LIBRARY - Pipes and serialization
file(GLOB_RECURSE MESSAGE_SRC "${PROJECT_SOURCE_DIR}/src/messages/*.cpp")
set(MESSAGE_SRC "${MESSAGE_SRC};${PROJECT_SOURCE_DIR}/third_party/sentinet_message_pkg/c/src/messages.c")
set(MESSAGE_LIB messages)
add_library(${MESSAGE_LIB} STATIC ${MESSAGE_SRC})


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

  add_executable(all_in_one_two "${PROJECT_SOURCE_DIR}/src/impl/examples/all_in_one_type_two.cpp")
  target_link_libraries(all_in_one_two ${KERNEL_LIB} ${CONTROL_LIB} ${YAML_CPP})

  add_executable(proxy "${PROJECT_SOURCE_DIR}/src/impl/examples/proxy.cpp")
  target_link_libraries(proxy ${KERNEL_LIB} ${CONTROL_LIB} ${YAML_CPP})
endif()
