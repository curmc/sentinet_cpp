project(tests)

add_executable(run_unit_tests "${PROJECT_SOURCE_DIR}/main.cpp"
                              "${PROJECT_SOURCE_DIR}/control/NetworkPatterns.cpp"
                              "${PROJECT_SOURCE_DIR}/control/ZMQControlClient.cpp"
                              "${PROJECT_SOURCE_DIR}/messages/data.cpp"
                              "${PROJECT_SOURCE_DIR}/messages/cmd_vel.cpp")

target_link_libraries(run_unit_tests sentinet gtest smessages)
