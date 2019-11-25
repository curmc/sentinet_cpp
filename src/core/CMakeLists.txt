project(control)

file(GLOB CONTROL_SOURCE "${PROJECT_SOURCE_DIR}/*.cpp")

target_sources(sentinet
  PRIVATE
    ${CONTROL_SOURCE})
# if(BUILD_SHARED)
#   add_library(control SHARED ${CONTROL_SOURCE})
# else()
#   add_library(control STATIC ${CONTROL_SOURCE})
# endif()
#
# target_link_libraries(control messages zmq pthread)
