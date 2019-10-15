project(kernel)

file(GLOB KERNEL_SOURCE "${PROJECT_SOURCE_DIR}/*.cpp")

if(BUILD_SHARED)
  add_library(kernel SHARED ${KERNEL_SOURCE})
else()
  add_library(kernel STATIC ${KERNEL_SOURCE})
endif()

target_link_libraries(kernel pthread control messages)

install(TARGETS kernel LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
                       ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

