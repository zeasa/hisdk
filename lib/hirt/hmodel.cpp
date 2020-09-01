#include "hmodel.h"
#include "hmodel_generated.h"

using std::endl;


namespace hisdk
{

hModel::hModel()
{

}

hModel::~hModel()
{
    mMemoryListEntries.clear();
    mTaskListEntries.clear();
    mAddressListEntries.clear();
}

std::string hModel::getName() const
{
    return mName;
}

int hModel::getNumMemoryListEntries() const
{
    return (int)mMemoryListEntries.size();
}

MemoryListEntry hModel::getMemoryListEntry(u32_t mem_id) const
{
    return mMemoryListEntries[mem_id];
}
const std::vector<MemoryListEntry> &hModel::getMemoryListEntries() const
{
    return mMemoryListEntries;
}


int hModel::getNumTaskListEntries() const
{
    return mTaskListEntries.size();
}

TaskListEntry hModel::getTaskListEntry(u32_t task_id) const
{
    return mTaskListEntries[task_id];
}
const std::vector<TaskListEntry> &hModel::getTaskListEntries() const
{
    return mTaskListEntries;
}

int hModel::getNumAddressListEntries() const
{
    return mAddressListEntries.size();
}

AddressListEntry hModel::getAddressListEntry(u32_t address_list_index) const
{
    return mAddressListEntries[address_list_index];
}
const std::vector<AddressListEntry> &hModel::getAddressListEntries() const
{
    return mAddressListEntries;
}

//
// internally facing
//
void hModel::setMemoryListEntries(const std::vector<MemoryListEntry> &m)
{
    mMemoryListEntries = m;
}
void hModel::setTaskListEntries(const std::vector<TaskListEntry> &m)
{
    mTaskListEntries = m;
}
void hModel::setAddressListEntries(const std::vector<AddressListEntry> &e)
{
    mAddressListEntries = e;
}

int  hModel::setSymbolContent(std::string name, const Blob &b, u8_t *data)
{
    mSymbols[name].name = b.name;
    mSymbols[name].interface = b.interface;
    mSymbols[name].type = b.type;
    mSymbols[name].version   = b.version;
    mSymbols[name].size = b.size;
    mSymbols[name].data = data;

    return 0;
}

bool hModel::getSymbolContent(std::string name, Blob &b, u8_t * &data)
{
    std::map<std::string, Symbol>::iterator f = mSymbols.find(name);

    if ( f == mSymbols.end() ) {
        return false;
    }

    b.name = f->second.name;
    b.size = f->second.size;
    b.version = f->second.version;
    b.interface = f->second.interface;
    b.type = f->second.type;
    data = f->second.data;
    
    return true;
}



bool hModel::serialize()
{
    // TODO: Check if mFbb already contains serialized data.

    std::vector<flatbuffers::Offset<hisdk::hmodel::TaskListEntry>>    task_list;
    std::vector<flatbuffers::Offset<hisdk::hmodel::MemoryListEntry>>  memory_list;
    std::vector<flatbuffers::Offset<hisdk::hmodel::AddressListEntry>> address_list;
    std::vector<flatbuffers::Offset<hisdk::hmodel::Blob>>             blobs;

    hisdk::hmodel::Version hmodel_version(hisdk::hmodel::HModelVersionMain_VAL,
                                          hisdk::hmodel::HModelVersionSub_VAL,
                                          hisdk::hmodel::HModelVersionSubMinor_VAL);

    for ( size_t ti = 0, TI = mTaskListEntries.size(); ti != TI; ++ti ) 
    {
        const TaskListEntry & tle = mTaskListEntries[ti];
        flatbuffers::Offset<flatbuffers::Vector<u32_t>> address_list = mFbb.CreateVector<u32_t>(tle.address_list);

        hisdk::hmodel::TaskListEntryBuilder tleb(mFbb);
        tleb.add_address_list(address_list);
        tleb.add_id(tle.id);
        hisdk::hmodel::Interface if_id;
        if ( tle.interface == hisdk::TaskListEntry::interface_GPU() ) {
            if_id = hisdk::hmodel::Interface_GPU;
        } else if ( tle.interface == hisdk::TaskListEntry::interface_CPU() ) {
            if_id = hisdk::hmodel::Interface_CPU;
        } else {
            if_id = hisdk::hmodel::Interface_NONE;
        }
        tleb.add_interface(if_id);
        task_list.push_back(tleb.Finish());
    }

    for ( size_t mi = 0, MI = mMemoryListEntries.size(); mi != MI; ++mi) 
    {
        const MemoryListEntry & mle = mMemoryListEntries[mi];

        hisdk::hmodel::MemoryListEntryBuilder mleb(mFbb);
        mleb.add_size(mle.size);
        mleb.add_alignment(mle.alignment);
        mleb.add_blob_id(mle.blob_id);
        mleb.add_flags( (hisdk::hmodel::MemoryFlags) mle.flags);
        mleb.add_id(mle.id);
        mleb.add_domain((hisdk::hmodel::MemoryDomain) mle.domain);
        memory_list.push_back(mleb.Finish());
    }

    for ( size_t ai = 0, AI = mAddressListEntries.size(); ai != AI; ++ai) 
    {
        const AddressListEntry & ale = mAddressListEntries[ai];
        hisdk::hmodel::AddressListEntryBuilder aleb(mFbb);
        aleb.add_size(ale.size);
        aleb.add_offset(ale.offset);
        aleb.add_mem_id(ale.mem_id);
        aleb.add_id(ale.id);
        address_list.push_back(aleb.Finish());
    }
    
    for ( std::map<std::string, Symbol>::const_iterator si = mSymbols.begin(); si != mSymbols.end(); ++si) {
        const Symbol &sym = si->second;
        flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data = mFbb.CreateVector<uint8_t>(sym.data, sym.size);
        flatbuffers::Offset<flatbuffers::String> name = mFbb.CreateString(sym.name.c_str());

        hisdk::hmodel::BlobBuilder bb(mFbb);
        bb.add_data(data);
        bb.add_size(sym.size);
        hisdk::hmodel::Version v(sym.version.main, sym.version.sub, sym.version.sub_minor);
        bb.add_version(&v);
        bb.add_interface( (hisdk::hmodel::Interface) sym.interface);
        bb.add_type( sym.type);
        bb.add_name(name);
        blobs.push_back(bb.Finish());
    }

    flatbuffers::Offset<hisdk::hmodel::HModel> l =
        CreateHModelDirect(mFbb, &hmodel_version, &task_list, &address_list, &memory_list, &blobs);

    mFbb.Finish(l, "HSDK");

    return true;
}

hisdkRet_t hModel::getSerializedData(u8_t *buffer)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    u8_t *tmp = NULL;
    u64_t size = 0;

    if (buffer == NULL)
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_BADPARAMETER);

    size = mFbb.GetSize();
    tmp = mFbb.GetBufferPointer();
    memcpy(buffer, tmp, size);

fail:
    return e;
}

hisdkRet_t hModel::getSerializedDataSize(u64_t *size)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    if (size == NULL)
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_BADPARAMETER);

    *size = mFbb.GetSize();

fail:
    return e;
}

bool hModel::deserializeFrom(u8_t *flatbuf)
{
    const hisdk::hmodel::HModel *model = hisdk::hmodel::GetHModel(flatbuf);
    if(model == NULL)
    {
        return false;
    }
    
    const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::TaskListEntry>>    *task_list    = model->task_list();
    const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::MemoryListEntry>>  *memory_list  = model->memory_list();
    const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::AddressListEntry>> *address_list = model->address_list();
    const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::Blob>>             *blobs        = model->blobs();

    flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::TaskListEntry>>::const_iterator tli = task_list->begin();
    for (; tli != task_list->end(); ++tli) 
    {
        TaskListEntry tle;

        tle.id = tli->id();
        tle.interface = tli->interface();

        if ( tli->address_list() ) {
            flatbuffers::Vector<unsigned int>::const_iterator ali = tli->address_list()->begin();
            for (; ali != tli->address_list()->end(); ++ali) {
                tle.address_list.push_back(*ali);
            }
        }

        mTaskListEntries.push_back(tle);
    }

    flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::MemoryListEntry>>::const_iterator li = memory_list->begin();
    for (; li != memory_list->end(); ++li) 
    {
        MemoryListEntry mle;

        mle.id = li->id();
        mle.size = li->size();
        mle.alignment = li->alignment();
        mle.flags = li->flags();
        mle.domain = li->domain();
        mle.blob_id = li->blob_id();
        
        mMemoryListEntries.push_back(mle);
    }

    flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::AddressListEntry>>::const_iterator adli = address_list->begin();
    for (; adli != address_list->end(); ++adli ) 
    {
        AddressListEntry ale;

        ale.size = adli->size();
        ale.offset = adli->offset();
        ale.mem_id = adli->mem_id();
        ale.id = adli->id();

        mAddressListEntries.push_back(ale);
    }

    flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::Blob>>::const_iterator bi = blobs->begin();
    for (; bi != blobs->end(); ++bi ) 
    {
        std::string blob_name = bi->name()->str();

        mSymbols[blob_name].name = blob_name;
        mSymbols[blob_name].size = bi->size();
        mSymbols[blob_name].version.main = bi->version()->main();
        mSymbols[blob_name].version.sub = bi->version()->sub();
        mSymbols[blob_name].version.sub_minor = bi->version()->sub_minor();
        mSymbols[blob_name].interface = (hisdk::Interface)bi->interface();
        mSymbols[blob_name].type = bi->type();

        u8_t *blob_data = new u8_t[mSymbols[blob_name].size];
        memset(blob_data, 0, mSymbols[blob_name].size);

        u8_t *binblob = (u8_t *)bi->data()->Data();
        memcpy((void*)blob_data, (void *)binblob, mSymbols[blob_name].size);

        mSymbols[blob_name].data = blob_data;

    }

    return true;
}


} // hisdk
