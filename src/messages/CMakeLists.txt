project(messages)

file(GLOB_RECURSE MESSAGES_SOURCE "${PROJECT_SOURCE_DIR}/*.cpp")

if(BUILD_SHARED)
  add_library(messages SHARED ${MESSAGES_SOURCE})
else()
  add_library(messages STATIC ${MESSAGES_SOURCE})
endif()

target_link_libraries(messages smessages)
