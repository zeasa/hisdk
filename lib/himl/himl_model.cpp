#include "himl.h"
#include "hmodel.h"

using namespace hisdk;

hisdkRet_t himlLoadModel(himlModel_t **ppModel, const char* fname)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    himlModel_t *pModel = NULL;
    hisdkFile_t *file = NULL;
    hisdkStatType_t fileinfo;
    size_t filesize;
    u8_t *buf = NULL;
    size_t actually_read = 0;
    hModel *phModel = new hModel();

    const std::vector<AddressListEntry> * addresslist;
    const std::vector<MemoryListEntry>  * memorylist;
    const std::vector<TaskListEntry>    * tasklist;
   
    pModel = (himlModel_t *)hisdkAlloc(sizeof(himlModel_t));
    if(pModel == NULL)
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM, "himlModel_t alloc failed!");
    }
    hisdkMemset(pModel, 0, sizeof(himlModel_t));
    
    HISDK_ERR_FCALLFAIL( hisdkPortOsFopen(fname, HISDK_OPEN_READ, &file) );
    HISDK_ERR_FCALLFAIL( hisdkPortOsFstat(file, &fileinfo) );
    filesize = hisdkPortOsStatGetSize(&fileinfo);
    if(filesize == 0)
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_FILEREADFAILED, "model filesize==0.");
    }
    buf = (u8_t *)hisdkAlloc(filesize);
    if(buf == NULL)
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM, "model buffer alloc failed.");
    }
    HISDK_ERR_FCALLFAIL( hisdkPortOsFseek(file, 0, HisdkSeek_Set) );

    HISDK_ERR_FCALLFAIL( hisdkPortOsFread(file, buf, filesize, &actually_read) );
    if(actually_read != filesize)
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM, "model read wrong size for buffer?");
    }
    hisdkPortOsFclose(file);


    if(!phModel->deserializeFrom(buf))
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM, "model deserialize failed.");
    }
    
    if(buf != NULL)
    {
        hisdkFree(buf);
    }
    HISDK_LOG_INFO(LOG_DEBUG, "himlLoadModel done.");
    pModel->phModel = phModel;
    *ppModel = pModel;
    return e;
    
fail:
    if(buf != NULL)
    {
        hisdkFree(buf);
    }
    if(phModel != NULL)
    {
        delete phModel;
    }
    HISDK_LOG_INFO(LOG_DEBUG, "himlLoadModel failed.");
    *ppModel = NULL;
    return e;
}