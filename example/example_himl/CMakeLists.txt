include_directories(${HISDK_INCLUDE_DIRS})
add_executable(example_himl example_himl.c)
target_link_libraries(example_himl common himl hirt)