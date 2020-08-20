include_directories(${HISDK_INCLUDE_DIRS})
add_executable(example_hirt example_hirt.c)
target_link_libraries(example_hirt common hirt)