project(impl)

if(RMT_CORE)
  add_executable(kermit_kernel "${PROJECT_SOURCE_DIR}/kermit_kernel.cpp")
  add_executable(kermit_proxy "${PROJECT_SOURCE_DIR}/kermit_proxy.cpp")
  add_executable(kermit_control "${PROJECT_SOURCE_DIR}/kermit_control.cpp")

  target_link_libraries(kermit_kernel kernel control messages)
  target_link_libraries(kermit_proxy kernel control messages)
  target_link_libraries(kermit_control kernel control messages)
endif()

if(EXAMPLES)
  add_executable(example_with_proxy "${PROJECT_SOURCE_DIR}/examples/all_in_one_type_two.cpp")
  add_executable(proxy "${PROJECT_SOURCE_DIR}/examples/proxy.cpp")
  add_executable(pipe "${PROJECT_SOURCE_DIR}/examples/pipe.cpp")

  target_link_libraries(example_with_proxy kernel control messages)
  target_link_libraries(proxy kernel control messages)
  target_link_libraries(pipe kernel control messages)
endif()
