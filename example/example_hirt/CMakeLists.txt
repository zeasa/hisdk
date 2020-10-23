include_directories(${HISDK_INCLUDE_DIRS})

add_executable(example_hirt_vadd example_hirt_vadd.cpp)
target_link_libraries(example_hirt_vadd hirt hidv common pthread m)

add_executable(example_hirt_model example_hirt_model.cpp)
target_link_libraries(example_hirt_model hirt hidv common pthread)

################################################################################
# install
install(TARGETS example_hirt_vadd
	RUNTIME DESTINATION ${BIN_INSTALL_PATH}
)

install(TARGETS example_hirt_model
	RUNTIME DESTINATION ${BIN_INSTALL_PATH}
)