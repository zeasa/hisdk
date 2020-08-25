include_directories(${HISDK_INCLUDE_DIRS})
add_executable(example_hirt example_hirt.c)
target_link_libraries(example_hirt hirt common pthread m)

########################################
# install
install(TARGETS example_hirt
    ARCHIVE DESTINATION install/lib
	RUNTIME DESTINATION install/bin
)