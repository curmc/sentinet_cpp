project(libs)

if(BUILD_SHARED)
  add_library(sentinet SHARED "")
else()
  add_library(sentinet STATIC "")
endif()

# Order matters
add_subdirectory("${PROJECT_SOURCE_DIR}/core")
add_subdirectory("${PROJECT_SOURCE_DIR}/messages")
add_subdirectory("${PROJECT_SOURCE_DIR}/control")
add_subdirectory("${PROJECT_SOURCE_DIR}/kernel")
add_subdirectory("${PROJECT_SOURCE_DIR}/apriltag")
# add_subdirectory("${PROJECT_SOURCE_DIR}/controllers")

target_link_libraries(sentinet zmq pthread smessages ${OpenCV_LIBS} apriltag)

install(TARGETS sentinet LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

add_subdirectory("${PROJECT_SOURCE_DIR}/impl")
