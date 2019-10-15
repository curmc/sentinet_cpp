project(msg_pkg)

file(GLOB MSG_PKG_SRC "${PROJECT_SOURCE_DIR}/sentinet_message_pkg/c/src/*.c")

if(BUILD_SHARED)
  add_library(smessages SHARED ${MSG_PKG_SRC})
else()
  add_library(smessages STATIC ${MSG_PKG_SRC})
endif()
