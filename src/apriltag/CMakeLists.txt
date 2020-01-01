project(apriltag)

file(GLOB APRIL_SRC "${PROJECT_SOURCE_DIR}/*.cpp")

target_sources(sentinet
  PUBLIC
  ${APRIL_SRC})
