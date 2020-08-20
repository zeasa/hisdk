include_directories(${HISDK_INCLUDE_DIRS})
add_executable(example_hisdk_log example_hisdk_log.c)
target_link_libraries(example_hisdk_log common)