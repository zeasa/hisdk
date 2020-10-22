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
    
	loadMemories();
	
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

hisdkRet_t hirtModelCtx::loadMemories()
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
            HISDK_LOG_INFO(LOG_SYSTEM, "!flags_alloc()");
            continue;
        }

        //HISDK_LOG_INFO(LOG_SYSTEM, "mle.domain=%u", mle.domain);
        if( mle.domain() == MemoryListEntry::domain_gmem() )
        {
            //do the gmem alloc for the memory area
            hirtGMemAddress_t gmem_addr;

            //HISDK_LOG_INFO(LOG_SYSTEM, "MemoryListEntry::domain_gmem()");
            HISDK_ERR_FCALLFAIL( hirtGpuMalloc(&gmem_addr, mle.size()) );
            mle.setHandle(gmem_addr);

            /*bind input and output memory entry*/
            if(mle.flags() & MemoryListEntry::flags_input())
            {
                m_memory_input = mle;
                HISDK_LOG_INFO(LOG_SYSTEM, "m_memory_input()=%lu", mle.size());
            }
            if(mle.flags() & MemoryListEntry::flags_output())
            {
                m_memory_output = mle;
                HISDK_LOG_INFO(LOG_SYSTEM, "m_memory_output()=%lu", mle.size());
            }
            
            if(mle.flags() & MemoryListEntry::flags_set())
            {
                u8_t *pbuf;
                pbuf = (u8_t*)hisdkAlloc(mle.size());
                if(pbuf == NULL)
                {
                    HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM,
                                             "failed to alloc buffer for memorylistentry");
                }
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

                //HISDK_ERR_FCALLFAIL( hirtMemcpy((void*)gmem_addr, pbuf, mle.size(), HIRT_MEM_TRANS_DIR_HOST2GPU) );
                HISDK_LOG_INFO(LOG_SYSTEM, "hirtMemcpy->0x%lx,size=%lu", gmem_addr, mle.size());
                hisdkFree(pbuf);
            }
            //transfor the memory contents to gmem
            
        }
    }

fail:
    return e;
}

void hirtModelCtx::unloadModel()
{
    unloadMemories();

    m_task_entries.clear();
    m_memory_entries.clear();
    m_address_entries.clear();

    m_task.clear();
    m_memory.clear();
    m_address.clear();
}

#include <iostream>
hisdkRet_t hirtModelCtx::submit(const unsigned char* pBufInput, unsigned char* pBufOutput, hirtCmdQueue_t *pCmdQueue)
{
    //for each task in modelctx,choose the right 
    //kernel and invoke kernel to do the inference
    hisdkRet_t e = HISDK_RET_SUCCESS;
    hirtGMemAddress_t gmemaddr;
    
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtModelCtx::submit...");

    /*prepare the input&output buffer for the network*/
    gmemaddr = m_memory_input.getHandle();
    hirtMemcpy(&gmemaddr, pBufInput, m_memory_input.size(), HIRT_MEM_TRANS_DIR_HOST2GPU);
    
    /*prepare kernel for all layers of the network*/
    for(int i=0; i<m_task_entries.size(); i++)
    {
        debugTaskEntrys(m_task_entries[i]);

        hirtKernelParamsBuffer_t *pParams = NULL;
        hirtKernelParamMemory_t  paramMem;
        hirtKernelParamsBufferCreate(&pParams);
        hirtKernelParamsBufferAddPlaceHolder(pParams, sizeof(hirtKernelParamParallel_t));
        paramMem.mem_param_num   = m_task_entries[i].pr_addr_list.size();
        paramMem.mem_input_num   = m_task_entries[i].in_addr_list.size();
        paramMem.mem_output_num  = m_task_entries[i].ou_addr_list.size();
        paramMem.mem_weight_num  = m_task_entries[i].wt_addr_list.size();
        paramMem.mem_bias_num    = m_task_entries[i].bs_addr_list.size();
        paramMem.mem_feature_num = m_task_entries[i].fm_addr_list.size();
        paramMem.mem_lut_num     = m_task_entries[i].lu_addr_list.size();
        hirtKernelParamsBufferAddParam(pParams, &paramMem, sizeof(hirtKernelParamMemory_t));
        
        for(int j=0; j<m_task_entries[i].pr_addr_list.size(); j++)
        {
            uint32_t addr = m_task_entries[i].pr_addr_list[j];
            hirtGMemAddress_t gmemaddr = m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset();
            hirtKernelParamsBufferAddParam(pParams, &gmemaddr, sizeof(hirtGMemAddress_t));
        }
        for(int j=0; j<m_task_entries[i].in_addr_list.size(); j++)
        {
            uint32_t addr = m_task_entries[i].in_addr_list[j];
            hirtGMemAddress_t gmemaddr = m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset();
            hirtKernelParamsBufferAddParam(pParams, &gmemaddr, sizeof(hirtGMemAddress_t));
        }
        for(int j=0; j<m_task_entries[i].ou_addr_list.size(); j++)
        {
            uint32_t addr = m_task_entries[i].ou_addr_list[j];
            hirtGMemAddress_t gmemaddr = m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset();
            hirtKernelParamsBufferAddParam(pParams, &gmemaddr, sizeof(hirtGMemAddress_t));
        }
        for(int j=0; j<m_task_entries[i].wt_addr_list.size(); j++)
        {
            uint32_t addr = m_task_entries[i].wt_addr_list[j];
            hirtGMemAddress_t gmemaddr = m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset();
            hirtKernelParamsBufferAddParam(pParams, &gmemaddr, sizeof(hirtGMemAddress_t));
        }
        for(int j=0; j<m_task_entries[i].bs_addr_list.size(); j++)
        {
            uint32_t addr = m_task_entries[i].bs_addr_list[j];
            hirtGMemAddress_t gmemaddr = m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset();
            hirtKernelParamsBufferAddParam(pParams, &gmemaddr, sizeof(hirtGMemAddress_t));
        }
        for(int j=0; j<m_task_entries[i].fm_addr_list.size(); j++)
        {
            uint32_t addr = m_task_entries[i].fm_addr_list[j];
            hirtGMemAddress_t gmemaddr = m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset();
            hirtKernelParamsBufferAddParam(pParams, &gmemaddr, sizeof(hirtGMemAddress_t));
        }
        for(int j=0; j<m_task_entries[i].lu_addr_list.size(); j++)
        {
            uint32_t addr = m_task_entries[i].lu_addr_list[j];
            hirtGMemAddress_t gmemaddr = m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset();
            hirtKernelParamsBufferAddParam(pParams, &gmemaddr, sizeof(hirtGMemAddress_t));
        }

        hirtKernelBinBuffer_t *pkrnlBin = NULL;
        hirtInvokeKernel(m_task_entries[i].type.c_str(), pParams, &pkrnlBin, 1, pCmdQueue);

#if 0
        hirtCmdQueueSync(pCmdQueue);
#endif
        hirtKernelBinBufferDestroy(pkrnlBin);
    }

    /*copy out the result from the gpu output buffer gmem to the host memory*/
    gmemaddr = m_memory_output.getHandle();
    hirtMemcpy(pBufOutput, &gmemaddr, m_memory_output.size(), HIRT_MEM_TRANS_DIR_GPU2HOST);
}

u64_t hirtModelCtx::getInputTensorBufferSize(void)
{
    return m_memory_input.size();
}

u64_t hirtModelCtx::getOutputTensorBufferSize(void)
{
    return m_memory_output.size();
}

void hirtModelCtx::debugTaskEntrys(TaskListEntry &task)
{
    std::string op_name = task.name;
    std::string op_type = task.type;
    std::cout << "op[" << op_name.c_str() << "]<" << op_type.c_str() << ">==========" <<  std::endl;

    std::cout << "  pr_addr_list:" << std::endl;
    for(int j=0; j<task.pr_addr_list.size(); j++)
    {
        uint32_t addr = task.pr_addr_list[j];
        std::cout << "    [" << j << "] = " << addr
                  << ", 0x" << std::hex << m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset()
                  << ", 0x" << m_address[addr].size()
                  << std::dec << std::endl;
    }
    
    std::cout << "  in_addr_list:" << std::endl;
    for(int j=0; j<task.in_addr_list.size(); j++)
    {
        uint32_t addr = task.in_addr_list[j];
        std::cout << "    [" << j << "] = " << task.in_addr_list[j] 
                  << ", 0x" << std::hex << m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset()
                  << ", 0x" << m_address[addr].size()
                  << std::dec << std::endl;
    }
    
    std::cout << "  ou_addr_list:" << std::endl;
    for(int j=0; j<task.ou_addr_list.size(); j++)
    {
        uint32_t addr = task.ou_addr_list[j];
        std::cout << "    [" << j << "] = " << task.ou_addr_list[j] 
                  << ", 0x" << std::hex << m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset()
                  << ", 0x" << m_address[addr].size()
                  << std::dec << std::endl;
    }
    
    std::cout << "  wt_addr_list:" << std::endl;
    for(int j=0; j<task.wt_addr_list.size(); j++)
    {
        uint32_t addr = task.wt_addr_list[j];
        std::cout << "    [" << j << "] = " << task.wt_addr_list[j] 
                  << ", 0x" << std::hex << m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset()
                  << ", 0x" << m_address[addr].size()
                  << std::dec << std::endl;
    }
    
    std::cout << "  bs_addr_list:" << std::endl;
    for(int j=0; j<task.bs_addr_list.size(); j++)
    {
        uint32_t addr = task.bs_addr_list[j];
        std::cout << "    [" << j << "] = " << task.bs_addr_list[j] 
                  << ", 0x" << std::hex << m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset()
                  << ", 0x" << m_address[addr].size()
                  << std::dec << std::endl;
    }
    
    std::cout << "  fm_addr_list:" << std::endl;
    for(int j=0; j<task.fm_addr_list.size(); j++)
    {
        uint32_t addr = task.fm_addr_list[j];
        std::cout << "    [" << j << "] = " << task.fm_addr_list[j] 
                  << ", 0x" << std::hex << m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset()
                  << ", 0x" << m_address[addr].size()
                  << std::dec << std::endl;
    }
    
    std::cout << "  lu_addr_list:" << std::endl;
    for(int j=0; j<task.lu_addr_list.size(); j++)
    {
        uint32_t addr = task.lu_addr_list[j];
        std::cout << "    [" << j << "] = " << task.lu_addr_list[j] 
                  << ", 0x" << std::hex << m_memory[m_address[addr].mem_id()].getHandle() + m_address[addr].offset()
                  << ", 0x" << m_address[addr].size()
                  << std::dec << std::endl;
    }
}