########################################
# put all sources in to {DIR_LIBHIRT_SRCS}
aux_source_directory(. DIR_LIBHIRT_SRCS)

########################################
# generate library
add_library(hirt ${DIR_LIBHIRT_SRCS})