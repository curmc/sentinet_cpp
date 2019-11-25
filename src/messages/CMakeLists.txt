project(messages)

file(GLOB_RECURSE MESSAGES_SOURCE "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB SMESSAGES_SOURCE "${PROJECT_SOURCE_DIR}/*.c")

target_sources(sentinet
  PUBLIC 
  ${MESSAGES_SOURCE})

if(BUILD_SHARED)
  add_library(smessages SHARED ${SMESSAGES_SOURCE})
else()
  add_library(smessages STATIC ${SMESSAGES_SOURCE})
endif()

# message(${MESSAGES_SOURCE})
# if(BUILD_SHARED)
#   add_library(messages SHARED ${MESSAGES_SOURCE})
# else()
#   add_library(messages STATIC ${MESSAGES_SOURCE})
# endif()
#
#
# target_link_libraries(messages smessages)
#
# install(TARGETS messages LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
