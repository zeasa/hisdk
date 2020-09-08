#include "hirt.h"
#include "hirt_model.h"
#include "hirt_mm.h"
#include "hirt_device.h"
#include "hirt_cqueue.h"
#include "hirt_kernel.h"
#include "hirt_scheduler.h"
#include "hirt_event.h"
#include "hirt_utils.h"
#include "hisdk_log.h"
#include "hmodel.h"

using namespace hisdk;

hirtModelCtx::hirtModelCtx()
{
    
}

hirtModelCtx::~hirtModelCtx()
{
    
}

hisdkRet_t hirtModelCtx::loadModel(const char* fname)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    hisdkFile_t *file = NULL;
    hisdkStatType_t fileinfo;
    size_t filesize;
    u8_t *buf = NULL;
    size_t actually_read = 0;
    
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


    if(!m_hModel.deserializeFrom(buf))
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM, "model deserialize failed.");
    }

    m_memory_entries  = m_hModel.getMemoryListEntries();
    m_address_entries = m_hModel.getAddressListEntries();
    m_task_entries    = m_hModel.getTaskListEntries();

    /*
	if(m_memory_entries.size() > HIRT_MODEL_MEMINDEX_MAX)
	{
	    HISDK_ERR_RPTFAIL(HISDK_RET_ERR_BADPARAMETER, "model memindex %d > %d.", 
	        m_memory_entries.size(), HIRT_MODEL_MEMINDEX_MAX);
	}
	*/

    m_memory.resize(m_memory_entries.size());
    for ( size_t mi = 0, MI = m_memory_entries.size(); mi != MI; ++mi ) 
    {
        m_memory[mi] = Memory(m_memory_entries[mi]);
    }
    
	loadMemory();
	
    m_address.resize(m_address_entries.size());
    for ( size_t ai = 0, AI = m_address_entries.size(); ai != AI; ++ai ) 
    {
        m_address[ai] = Address(m_address_entries[ai]);
    }

    m_task.resize(m_task_entries.size());
    for ( size_t ti = 0, TI = m_task_entries.size(); ti != TI; ++ti )
    {
        m_task[ti] = Task(m_task_entries[ti]);
        /*
        if ( m_task[ti].interface() == ILoadable::Interface_DLA1 )
        {
            if ( m_task[ti].instance() != ILoadable::TaskListEntry::instance_ANY() )
            {
                gLogWarning << "the loadable specified dla instance " <<
                    m_task[ti].instance() << " which will be ignored.";
            }
            m_numDLATasks++;
        }
        */
    }

    if(buf != NULL)
    {
        hisdkFree(buf);
    }
    HISDK_LOG_INFO(LOG_DEBUG, "himlLoadModel done.");
    return e;
    
fail:
    if(buf != NULL)
    {
        hisdkFree(buf);
    }
    HISDK_LOG_INFO(LOG_DEBUG, "himlLoadModel failed.");
    return e;
}

hisdkRet_t hirtModelCtx::loadMemory()
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    size_t mi, MI;
    bool ok = false;

    HISDK_LOG_INFO(LOG_SYSTEM, "loadModelMemory()=%lu", m_memory.size());
    for(mi = 0, MI = m_memory.size(); mi != MI; ++mi)
    {
        Memory &mle = m_memory[mi];
        
        if( !(mle.flags() & MemoryListEntry::flags_alloc()) )
        {
            //HISDK_LOG_INFO(LOG_SYSTEM, "!flags_alloc()");
            continue;
        }

        //HISDK_LOG_INFO(LOG_SYSTEM, "mle.domain=%u", mle.domain);
        if( mle.domain() == MemoryListEntry::domain_gmem() )
        {
            //do the gmem alloc for the memory area
            hirtGMemAddress_t gmem_addr;
            u8_t *pbuf;

            //HISDK_LOG_INFO(LOG_SYSTEM, "MemoryListEntry::domain_gmem()");
            
            pbuf = (u8_t*)hisdkAlloc(mle.size());
            if(pbuf == NULL)
            {
                HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM,
                                         "failed to alloc buffer for memorylistentry");
            }
            HISDK_ERR_FCALLFAIL( hirtGpuMalloc(&gmem_addr, mle.size()) );
            mle.setHandle(gmem_addr);
            
            if(mle.flags() & MemoryListEntry::flags_set())
            {
                //HISDK_LOG_INFO(LOG_SYSTEM, "flags_set()");
                //load the memory with blob contents
                const std::vector<std::string> contents = mle.contents();
                const std::vector<u64_t>       offsets  = mle.offsets();
                HISDK_LOG_INFO(LOG_SYSTEM, "contents.size=%lu", mle.contents().size());
                
                size_t ci, CI;
                for ( ci = 0, CI = contents.size(); ci != CI; ++ci )
                {
                    Blob content_blob;
                    u8_t *data;
                    
                    const std::string &content_symbol = contents[ci];

                    ok = m_hModel.getSymbolContent(content_symbol, content_blob, data);

                    HISDK_LOG_INFO(LOG_SYSTEM, "content_symbol=%s, size=%lu", content_symbol.c_str(), content_blob.size);
                    
                    if ( !ok ) {
                        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_BADPARAMETER,
                                             "failed to find buffer content symbol %s",
                                             content_symbol.c_str());
                    }

                    if ( mle.size() >= (u64_t)(offsets[ci] + content_blob.size) )
                    {
                        u8_t *src = data;
                        u8_t *dst = pbuf + offsets[ci];

                        for ( size_t byte = 0; byte < content_blob.size; byte++ ) 
                        {
                            dst[byte] = src[byte];
                        }
                    }
                    else {
                        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_BADPARAMETER, "content blob too large for pool size");
                    }
                }
            }
            //transfor the memory contents to gmem
            HISDK_ERR_FCALLFAIL( hirtMemcpy((void*)gmem_addr, pbuf, mle.size(), HIRT_MEM_TRANS_DIR_HOST2GPU) );
        }
    }

fail:
    return e;
}

void hirtModelCtx::unloadModel()
{
    unloadMemory();

    m_task_entries.clear();
    m_memory_entries.clear();
    m_address_entries.clear();

    m_task.clear();
    m_memory.clear();
    m_address.clear();
}


hisdkRet_t submit()
{
    //for each task in modelctx,choose the right 
    //kernel and invoke kernel to do the inference
    hisdkRet_t e = HISDK_RET_SUCCESS;
    
    HISDK_LOG_INFO(LOG_SYSTEM, "example_hirt program start...");
    
    hirtInit(0);
    hirtDev_t dev = 0;
    hirtGetDeviceHandle(&dev, 0);
    hirtSetCurrentDevice(dev);
    
    hirtCmdQueue_t *pCmdQueue = NULL;
    hirtCmdQueueCreate(&pCmdQueue);
    
    hirtScheduler_t *pScheduler = NULL;
    hirtSchedulerCreate(&pScheduler, pCmdQueue);
    
    hirtEventHandler_t *pEventHandler = NULL;
    hirtEventHandlerCreate(&pEventHandler, pScheduler);
    
#if 0
    int *k_a, *k_b;
    hirtGpuMalloc((void **)&k_a, sizeof(int));
    hirtGpuMalloc((void **)&k_b, sizeof(int));
    hirtMemcpy(k_a, &a, sizeof(int), HIRT_MEM_TRANS_DIR_HOST2GPU);
    hirtMemcpy(k_b, &b, sizeof(int), HIRT_MEM_TRANS_DIR_HOST2GPU);
#endif
    
    
    hirtKernelParamsBuffer_t *pParams = NULL;
    hirtKernelParamsBufferCreate(&pParams);
    //hirtKernelParamsBufferAddParam(pParams, &k_a, sizeof(int *));
    hirtKernelBinBuffer_t *pkrnlBin = NULL;
    hirtInvokeKernel("kernel.o", pParams, &pkrnlBin, 1, pCmdQueue);
    hirtCmdQueueSync(pCmdQueue);
    
#if 0    
    hirtInvokeKernel(&kernel1, 3, pParams, queue);
    hirtInvokeKernel(&kernel2, 3, pParams, queue);
    hirtMemcpy(&out, k_a, sizeof(int), HIRT_MEM_TRANS_DIR_GPU2HOST);
    hirtGpuFree(k_a);
    hirtGpuFree(k_b);
    hirtDestroyKernelParamsBuffer(pParams);
    hirtDestroyQueue(queue);
    hirtDestroy();
#endif
    
    hirtEventHandlerDestroy(pEventHandler);
    hirtSchedulerDestroy(pScheduler);
    hirtCmdQueueDestroy(pCmdQueue);
}


