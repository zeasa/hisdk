#ifndef LIBHIRT_MODEL_H__
#define LIBHIRT_MODEL_H__

#include "hirt.h"

//#ifdef __cplusplus
//extern "C" {
//#endif // __cplusplus

#define HIRT_MODEL_MEMINDEX_PARAM           (0)
#define HIRT_MODEL_MEMINDEX_INPUT           (1)
#define HIRT_MODEL_MEMINDEX_OUTPUT          (2)
#define HIRT_MODEL_MEMINDEX_WEIGHT          (3)
#define HIRT_MODEL_MEMINDEX_BIAS            (4)
#define HIRT_MODEL_MEMINDEX_FEATURE         (5)
#define HIRT_MODEL_MEMINDEX_LUT             (6)
#define HIRT_MODEL_MEMINDEX_MAX             (7)

using namespace hisdk;

class hirtModelCtx
{
public:
    class Task  {
    public:
        Task() { }
        Task(const TaskListEntry &e) : mEntry(e) { }
        Task(const Task &o) : mEntry(o.mEntry) { }
        u16_t id() const { return mEntry.id; }
        u32_t interface() const { return mEntry.interface; }
        //i32_t instance()  const { return mEntry.instance; }
        std::vector<u32_t> &address_list() { return mEntry.address_list; }
    protected:
        TaskListEntry mEntry;

    };

    class Memory {
    public:
        Memory() : hMem(0), pVirtAddr(0) { }
        Memory(const MemoryListEntry &e) : hMem(0), pVirtAddr(0), mEntry(e) { }
        Memory(const Memory &o) : hMem(o.hMem), pVirtAddr(0), mEntry(o.mEntry) { }
        inline u16_t id() { return mEntry.id; }
        inline u64_t size() { return mEntry.size; }
        inline u32_t alignment() { return mEntry.alignment; }
        inline u32_t domain() { return mEntry.domain; }
        inline bool  bound() { return hMem != 0; }
        inline u32_t flags() { return mEntry.flags; }
        inline void setHandle(void *h) { hMem = h; }
        inline void *getHandle() const { return hMem; }
        inline void setVirtAddr(void *addr) { pVirtAddr = addr; }
        inline void *getVirtAddr() const { return pVirtAddr; }
        inline std::vector<std::string> & contents() { return mEntry.contents; }
        inline std::vector<u64_t> & offsets() { return mEntry.offsets; }
   #if 0
        inline int inputBindId() const {
            if ( mEntry.flags & mEntry.flags_input() ) {
                return (int) mEntry.bind_id;
            }
            return -1;
        }
        inline int outputBindId() const {
            if ( mEntry.flags & mEntry.flags_output() ) {
                return (int) mEntry.bind_id;
            }
            return -1;
        }
        inline bool bindable() const {
            return !!(mEntry.flags & (mEntry.flags_input() | mEntry.flags_output()));
        }
        inline int bindId(IOD &which) const
        {
            // there should be only one valid.  but if not take in order of
            // input, output
            if ( mEntry.flags & mEntry.flags_input() ) {
                which = IOD_Input;
                return (int) mEntry.bind_id;
            } else if ( mEntry.flags & mEntry.flags_output() ) {
                which = IOD_Output;
                return (int) mEntry.bind_id;
            }
            which = IOD_Max;
            return -1;
        };
        inline int tensorDescId() const {
            if ( mEntry.flags & ( mEntry.flags_input() | mEntry.flags_output() ) ) {
                return (int) mEntry.tensor_desc_id;
            }
            return -1;
        }
    #endif
    protected:
        void *hMem;
        void *pVirtAddr;
        MemoryListEntry mEntry;
    };

    class Address {
    public:
        Address() { }
        Address(const AddressListEntry &e) : mEntry(e) { }
        Address(const Address &o) : mEntry(o.mEntry) { }
        u16_t id() const { return mEntry.id; }
        u16_t mem_id() const { return mEntry.mem_id; }
        u64_t offset() const { return mEntry.offset; }
    public:
        AddressListEntry mEntry;
    };

public:
    hirtModelCtx();
    ~hirtModelCtx();

    hisdkRet_t loadModel(const char* fname);
    hisdkRet_t unloadModel();
    hisdkRet_t loadModelMemory();

    hisdkRet_t runModel();
private:
    hModel m_hModel;

};

//#ifdef __cplusplus
//}
//#endif
#endif /*LIBHIRT_MODEL_H__*/

