include_directories(${HISDK_INCLUDE_DIRS})
add_executable(example_hmodel example_hmodel.cpp)
target_link_libraries(example_hmodel himl common m)

########################################
# install
install(TARGETS example_hmodel
	RUNTIME DESTINATION ${BIN_INSTALL_PATH}
)