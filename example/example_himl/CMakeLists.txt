include_directories(${HISDK_INCLUDE_DIRS})
add_executable(example_himl example_himl.cpp)
target_link_libraries(example_himl himl hirt common pthread)

########################################
# install
install(TARGETS example_himl
	RUNTIME DESTINATION ${BIN_INSTALL_PATH}
)
