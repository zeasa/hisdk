include_directories(${HISDK_INCLUDE_DIRS})

add_executable(example_hidv example_hidv.cpp)
target_link_libraries(example_hidv himl hidv common)

########################################
# install
install(TARGETS example_hidv
	RUNTIME DESTINATION ${BIN_INSTALL_PATH}
)

