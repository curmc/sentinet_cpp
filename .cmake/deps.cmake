
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(MSG_PKG_VERSION "release")
set(MSG_PKG_SUFFIX "v1.0.0")
set(MSG_DIR "${CMAKE_SOURCE_DIR}/third_party/msg_pkg-${MSG_PKG_SUFFIX}")
set(MSG_PKG_INC_DIRS "${MSG_DIR}/c/include")

ExternalProject_Add(
  sentinet_message_pkg 
  GIT_REPOSITORY "https://github.com/curmc/sentinet_message_pkg"
  GIT_TAG "${MSG_PKG_VERSION}"

  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${MSG_DIR}

  SOURCE_DIR "${MSG_DIR}"
  INSTALL_DIR = "${CMAKE_BINARY_DIR}/messages"
  BINARY_DIR "${CMAKE_BINARY_DIR}/messages"
  STEP_TARGETS build

  EXCLUDE_FROM_ALL TRUE
)

include_directories(${MSG_PKG_INC_DIRS})
