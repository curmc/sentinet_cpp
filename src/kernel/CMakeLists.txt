project(kernel)

file(GLOB KERNEL_SOURCE_CPP "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB KERNEL_SOURCE_C "${PROJECT_SOURCE_DIR}/*.c")

set(KERNEL_SOURCE "${KERNEL_SOURCE_CPP};${KERNEL_SOURCE_C}")

message(${KERNEL_SOURCE})
target_sources(sentinet
  PUBLIC 
  ${KERNEL_SOURCE})

# if(BUILD_SHARED)
#   add_library(kernel SHARED ${KERNEL_SOURCE})
# else()
#   add_library(kernel STATIC ${KERNEL_SOURCE})
# endif()
#
# target_link_libraries(kernel pthread control messages smessages)
#
# install(TARGETS kernel LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
#
