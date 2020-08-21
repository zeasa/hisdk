#ifndef __HISDK_CONFIG_H__
#define __HISDK_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define HISDK_DEBUG
//undef HISDK_DEBUG

#define LOGFILE_NAME                    "logfile"
#define LOGFILE_SIZEMAX                 (1*1024*1024)
#define LOGFILE_MEDIA                   (HISDK_LOG_TO_FILE)
#define LOGFILE_DEBUG_EN                (HISDK_LOG_DEBUG_ENABLE)

#define HISDK_UTILS_ERROR_TAG           "hisdk"

#define HIRT_CMDQUEUE_SIZMAX            (1024)
#define HIRT_PARAMBUF_MAXSIZE           (1024)
#define HIRT_HIPU200_CORENUMMAX         (13)
#define HIRT_HIPU200_MEM_CH_NUM         (2)
#define HIRT_HIPU200_MEM_CH_SIZ         (4096LL*1024*1024)
#define HIRT_HIPU200_MEM_BLK_SIZ        (64*1024*1024)
#define HIRT_HIPU200_MEM_BLK_NUM        (HIRT_HIPU200_MEM_CH_SIZ/HIRT_HIPU200_MEM_BLK_SIZ)
#define HIRT_HIPU200_MEM_CH0_NOCADDR    (0)
#define HIRT_HIPU200_MEM_CH1_NOCADDR    (1)


#ifdef __cplusplus
}
#endif
#endif /*__HISDK_CONFIG_H__*/

