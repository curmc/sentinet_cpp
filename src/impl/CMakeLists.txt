project(impl)

if(RMT_CORE)
  add_executable(kermit_kernel "${PROJECT_SOURCE_DIR}/kermit_kernel.cpp")
  add_executable(kermit_proxy "${PROJECT_SOURCE_DIR}/kermit_proxy.cpp")
  # add_executable(serial "${PROJECT_SOURCE_DIR}/serial_test.c")
  add_executable(april "${PROJECT_SOURCE_DIR}/april.cpp")
  # add_executable(kermit_control "${PROJECT_SOURCE_DIR}/kermit_control.cpp")
  #
  target_link_libraries(kermit_kernel sentinet)
  target_link_libraries(kermit_proxy sentinet)
  # target_link_libraries(serial sentinet)
  target_link_libraries(april sentinet)
  # target_link_libraries(kermit_control kernel control messages)

  install(TARGETS kermit_proxy kermit_kernel RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
  # install(TARGETS kermit_kernel RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()

if(EXAMPLES)
  add_executable(example_with_proxy "${PROJECT_SOURCE_DIR}/examples/all_in_one_type_two.cpp")
  add_executable(proxy "${PROJECT_SOURCE_DIR}/examples/proxy.cpp")
  add_executable(pipe "${PROJECT_SOURCE_DIR}/examples/pipe2.cpp")
  # add_executable(wiicontroller "${PROJECT_SOURCE_DIR}/controller_examples/wiitest.cpp")


  target_link_libraries(example_with_proxy kernel control messages)
  target_link_libraries(proxy kernel control messages)
  target_link_libraries(pipe kernel control messages)
  # target_link_libraries(wiicontroller controllers kernel control wiiuse)

  install(TARGETS pipe example_with_proxy proxy RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()
