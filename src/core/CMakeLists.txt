project(control)

file(GLOB CONTROL_SOURCE "${PROJECT_SOURCE_DIR}/*.cpp")

target_sources(sentinet
  PRIVATE
    ${CONTROL_SOURCE})
