#ifndef _HISDK_HMODEL_H__
#define _HISDK_HMODEL_H__

#include <map>
#include <vector>
#include "hmodel_generated.h"
#include "hisdk_type.h"
#include "hisdk_config.h"
#include "hisdk_port.h"
#include "hisdk_log.h"
#include "hisdk_error.h"


#define HISDK_HMODEL_INTERFACE_NONE 0U
#define HISDK_HMODEL_INTERFACE_GPU 1U
#define HISDK_HMODEL_INTERFACE_CPU 2U

#define HISDK_HMODEL_SUB_INTERFACE_DLA1_NONE  0U
#define HISDK_HMODEL_SUB_INTERFACE_DLA1_ADDR0 1U
#define HISDK_HMODEL_SUB_INTERFACE_DLA1_DEPS  2U
#define HISDK_HMODEL_SUB_INTERFACE_DLA1_OPS   3U
#define HISDK_HMODEL_SUB_INTERFACE_DLA1_SURFS 4U
#define HISDK_HMODEL_SUB_INTERFACE_DLA1_LUTS  5U

#define HISDK_HMODEL_SUB_INTERFACE_EMU1_NONE  0U
#define HISDK_HMODEL_SUB_INTERFACE_EMU1_ADDR0 1U
/* #define HISDK_HMODEL_SUB_INTERFACE_EMU1_DEPS  2U */
#define HISDK_HMODEL_SUB_INTERFACE_EMU1_OPS   3U
#define HISDK_HMODEL_SUB_INTERFACE_EMU1_SURFS 4U
/* #define HISDK_HMODEL_SUB_INTERFACE_EMU1_LUTS  5U */


#define HISDK_HMODEL_MEMORY_DOMAIN_GMEM 0U
#define HISDK_HMODEL_MEMORY_DOMAIN_MRAB 1U

#define HISDK_HMODEL_MEMORY_FLAGS_NONE    0U
#define HISDK_HMODEL_MEMORY_FLAGS_ALLOC   1U
#define HISDK_HMODEL_MEMORY_FLAGS_SET     2U
#define HISDK_HMODEL_MEMORY_FLAGS_INPUT   4U
#define HISDK_HMODEL_MEMORY_FLAGS_OUTPUT  8U
#define HISDK_HMODEL_MEMORY_FLAGS_DEBUG  16U

#define HISDK_HMODEL_EVENT_OP_WAIT   0U
#define HISDK_HMODEL_EVENT_OP_SIGNAL 1U

#define HISDK_HMODEL_TENSOR_DESC_NUM_STRIDES 8U    /* a little room to grow */


namespace hisdk
{

enum Interface {
    Interface_NONE = HISDK_HMODEL_INTERFACE_NONE,
    Interface_GPU  = HISDK_HMODEL_INTERFACE_GPU,
    Interface_CPU  = HISDK_HMODEL_INTERFACE_CPU,
};

enum MemoryDomain {
    MemoryDomain_GMEM   = HISDK_HMODEL_MEMORY_DOMAIN_GMEM,
    MemoryDomain_MRAB   = HISDK_HMODEL_MEMORY_DOMAIN_MRAB,
};

enum MemoryFlags {
    MemoryFlags_NONE   = HISDK_HMODEL_MEMORY_FLAGS_NONE,
    MemoryFlags_ALLOC  = HISDK_HMODEL_MEMORY_FLAGS_ALLOC,
    MemoryFlags_SET    = HISDK_HMODEL_MEMORY_FLAGS_SET,
    MemoryFlags_INPUT  = HISDK_HMODEL_MEMORY_FLAGS_INPUT,
    MemoryFlags_OUTPUT = HISDK_HMODEL_MEMORY_FLAGS_OUTPUT,
    MemoryFlags_DEBUG  = HISDK_HMODEL_MEMORY_FLAGS_DEBUG
};

struct Version
{
    u8_t main;
    u8_t sub;
    u8_t sub_minor;
    Version(u8_t maj, u8_t min, u8_t sub) : main(maj), sub(min), sub_minor(sub) { }
    Version() : main(0), sub(0), sub_minor(0) { }
};

struct MemoryListEntry
{
    u32_t id;
    u64_t size;
    u32_t alignment; // 0 for n/a, otherwise byte alignment
    u32_t  domain;
    static inline u32_t domain_gmem() { return MemoryDomain_GMEM; }
    static inline u32_t domain_mrab() { return MemoryDomain_MRAB; }
    u32_t  flags; // alloc or alloc_content or is-input or is-output
    static inline u32_t  flags_alloc()  { return MemoryFlags_ALLOC;  }
    static inline u32_t  flags_set()    { return MemoryFlags_SET;    }
    static inline u32_t  flags_input()  { return MemoryFlags_INPUT;  }
    static inline u32_t  flags_output() { return MemoryFlags_OUTPUT; }
    static inline u32_t  flags_debug()  { return MemoryFlags_DEBUG;  }
    std::vector<std::string> contents;  // symbolic reference to content blob
    std::vector<uint64_t>    offsets;   // associated offset for contents

    MemoryListEntry() : id(0), size(0), alignment(0), domain(0), flags(0),
                        contents(), offsets() { }
    MemoryListEntry(const MemoryListEntry &o) : id(o.id), size(o.size), alignment(o.alignment), domain(o.domain), flags(o.flags),
                                                contents(o.contents),
                                                offsets(o.offsets) { }
    MemoryListEntry(u32_t i, u64_t s, u32_t a, u32_t d, u32_t f, std::string sym = std::string(), u64_t o = 0) :
        id(i), size(s), alignment(a), domain(d), flags(f)
    {
        if ( sym.size() )
        {
            contents.push_back(sym);
            offsets.push_back(o);
        }
    }

};

struct TaskListEntry
{
    u32_t id;
    u32_t interface; // DLA interface id
    static inline u32_t interface_NONE() { return Interface_NONE; }
    static inline u32_t interface_GPU()  { return Interface_GPU;  }
    static inline u32_t interface_CPU()  { return Interface_CPU;  }

    std::vector<u32_t> address_list; // [addr list id]...[addr list id]
    TaskListEntry(const TaskListEntry &o) :
        id(o.id),
        interface(o.interface),
        address_list(o.address_list) { }

    TaskListEntry() : id(0),
                      interface(Interface_NONE),
                      address_list() { }
    TaskListEntry(u32_t i) : id(i),
                      interface(Interface_NONE),
                      address_list() { }

};

struct AddressListEntry
{
    u32_t id;     // all possible address list entries are given an id
    u32_t mem_id; // determines hRm (+offset from below)
    u64_t size;   // assert size <= memory[mem_id].size
    u64_t offset; // assert (offset + size) <= memory[mem_id].size
    AddressListEntry() : id(0), mem_id(0), size(0), offset(0) { }
    AddressListEntry(u32_t i, u32_t m, u64_t s, u64_t o = 0) : id(i), mem_id(m), size(s), offset(o) { }
    AddressListEntry(const AddressListEntry &o) : id(o.id), mem_id(o.mem_id), size(o.size), offset(o.offset) { }
};

struct Blob
{
    std::string name;
    u64_t size;
    Interface interface;
    u32_t type;
    Version version;

    Blob() :
        size(0),
        interface(Interface_NONE),
        type(0) { }

    Blob(const std::string &n, u64_t s, Interface i, u32_t si, Version v) :
        name(n),
        size(s),
        interface(i),
        type(si),
        version(v) { }
};

struct Symbol 
{
    std::string name;
    Interface interface;
    u32_t type;
    Version version;
    u64_t size;
    u8_t *data;
    Symbol() { }
};

class hModel
{
public: // externally facing

    virtual std::string getName() const;

    virtual int             getNumMemoryListEntries() const;
    virtual MemoryListEntry getMemoryListEntry(u32_t pool_id) const;

    virtual int           getNumTaskListEntries() const;
    virtual TaskListEntry getTaskListEntry(u32_t task_id) const;

    virtual int              getNumAddressListEntries() const;
    virtual AddressListEntry getAddressListEntry(u32_t address_list_index) const;

public: // internally facing
    int setSymbolContent(std::string name, const Blob &b, u8_t *data);
    bool getSymbolContent(std::string name, Blob &b, u8_t *&data);

    void setMemoryListEntries (const std::vector<MemoryListEntry>  &);
    void setTaskListEntries (const std::vector<TaskListEntry>    &);
    void setAddressListEntries (const std::vector<AddressListEntry> &);

    const std::vector<TaskListEntry> & getTaskListEntries()   const;
    const std::vector<MemoryListEntry> & getMemoryListEntries() const;
    const std::vector<AddressListEntry> & getAddressListEntries() const;

    hModel();
    virtual ~hModel();

    virtual bool serialize();
    virtual hisdkRet_t getSerializedData(u8_t *buffer);
    virtual hisdkRet_t getSerializedDataSize(u64_t *size);
    virtual bool deserializeFrom(u8_t *);

protected:
    std::map<std::string, Symbol> mSymbols;
    std::vector<MemoryListEntry>  mMemoryListEntries;
    std::vector<TaskListEntry>    mTaskListEntries;
    std::vector<AddressListEntry> mAddressListEntries;
    std::string mName;

private:
    flatbuffers::FlatBufferBuilder mFbb;
};

} // nvdla

#endif
