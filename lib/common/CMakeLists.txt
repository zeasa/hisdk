########################################
# put all sources in to {DIR_LIBCOMMON_SRCS}
aux_source_directory(. DIR_LIBCOMMON_SRCS)
aux_source_directory(./argtable3 DIR_LIBCOMMON_ARGTABLE3_SRCS)

########################################
# generate library
add_library(common ${DIR_LIBCOMMON_SRCS} ${DIR_LIBCOMMON_ARGTABLE3_SRCS})

