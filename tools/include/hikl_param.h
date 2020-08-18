#ifndef __LIBHIKL_LIBHIKL_PARAM_H__
#define __LIBHIKL_LIBHIKL_PARAM_H__

typedef unsigned char   uint8;
typedef unsigned int    uint32;

typedef struct{
    uint8 x_pos;
    uint8 y_pos;
    uint32 lcaddr;
}hikl_addr_t;

typedef struct{
    uint32 ifm_h;
    uint32 ifm_w;
    uint32 ifm_c;
    uint32 ofm_c;
    uint32 k_h;
    uint32 k_w;
}conv_shape_t;

typedef struct{
    uint8 top;
    uint8 bottom;
    uint8 side;
}pad_shape_t;

#endif /* __LIBHIKL_LIBHIKL_PARAM_H__ */