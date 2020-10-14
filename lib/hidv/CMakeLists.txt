########################################
# put all sources in to {DIR_LIBHIDV_SRCS}
aux_source_directory(. DIR_LIBHIDV_SRCS)

########################################
# generate library
add_library(hidv ${DIR_LIBHIDV_SRCS})

########################################
# install
install(TARGETS hidv
    ARCHIVE DESTINATION install/lib
	RUNTIME DESTINATION install/bin
)