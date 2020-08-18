
#include "hikl.h"
#include "libhikl_op.h"

const hikl_api_entrytable_t hikl_api_entrytable[] =
{
    {HIKL_API_CONV1, libhikl_op_conv1},
    {HIKL_API_CONV2, libhikl_op_conv2},
    {HIKL_API_CONV3, libhikl_op_conv3}
};

int libhikl_entry(int apicode)
{
    if(apicode >= sizeof(hikl_api_entrytable)/sizeof(hikl_api_entrytable_t))
        return -1;

    return (*hikl_api_entrytable[apicode].func_ptr)(0);
}