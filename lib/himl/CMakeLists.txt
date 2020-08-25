########################################
# put all sources in to {DIR_LIBHIML_SRCS}
aux_source_directory(. DIR_LIBHIML_SRCS)

########################################
# generate library
add_library(himl ${DIR_LIBHIML_SRCS})

########################################
# install
install(TARGETS himl
    ARCHIVE DESTINATION install/lib
	RUNTIME DESTINATION install/bin
)