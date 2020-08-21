########################################
cmake_minimum_required(VERSION 2.6)

########################################
# project
project(hisdk)
set(HISDK_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR})

########################################
# language
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g3"          CACHE STRING "Flags used by the CXX compiler during DEBUG builds.")
set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG"     CACHE STRING "Flags used by the CXX compiler during RELEASE builds.")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -DNDEBUG -g3" CACHE STRING "Flags used by the CXX compiler during RELWITHDEBINFO builds.")
option(ENABLE_WERROR "Compile with -Werror enabled (default is OFF)" OFF)
if (ENABLE_WERROR)
    add_compile_options(-Werror)
endif()

########################################
# include directory
set(HISDK_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/lib/common
	${CMAKE_CURRENT_SOURCE_DIR}/lib/hirt
	${CMAKE_CURRENT_SOURCE_DIR}/lib/common/argtable3
)
include_directories(${HISDK_INCLUDE_DIRS})

########################################
# subdirectories
add_subdirectory(lib)
add_subdirectory(example)
add_subdirectory(tools)
