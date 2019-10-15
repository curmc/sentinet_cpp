project(libs)

# Order matters
add_subdirectory("${PROJECT_SOURCE_DIR}/messages")
add_subdirectory("${PROJECT_SOURCE_DIR}/control")
add_subdirectory("${PROJECT_SOURCE_DIR}/kernel")
add_subdirectory("${CNTLR_PKG}" controller)
add_subdirectory("${PROJECT_SOURCE_DIR}/impl")

