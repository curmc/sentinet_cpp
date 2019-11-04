project(controllers)

file(GLOB CONTROL_SOURCE "${PROJECT_SOURCE_DIR}/*.cpp")

if(BUILD_SHARED)
  add_library(controllers SHARED ${CONTROL_SOURCE})
else()
  add_library(controllers STATIC ${CONTROL_SOURCE})
endif()

target_link_libraries(controllers pthread wiiuse control)

install(TARGETS controllers LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
