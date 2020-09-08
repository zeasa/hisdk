#include "hmodel.h"
#include "hmodel_generated.h"

using namespace std;

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

MemoryListEntry hModel::getMemoryListEntry(uint32_t mem_id) const
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

TaskListEntry hModel::getTaskListEntry(uint32_t task_id) const
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

AddressListEntry hModel::getAddressListEntry(uint32_t address_list_index) const
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

int  hModel::setSymbolContent(std::string name, const Blob &b, uint8_t *data)
{
    mSymbols[name].name = b.name;
    mSymbols[name].interface = b.interface;
    mSymbols[name].type = b.type;
    mSymbols[name].version   = b.version;
    mSymbols[name].size = b.size;
    mSymbols[name].data = data;

    return 0;
}

bool hModel::getSymbolContent(std::string name, Blob &b, uint8_t * &data)
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
        flatbuffers::Offset<flatbuffers::Vector<uint32_t>> pr_addr_list = mFbb.CreateVector<uint32_t>(tle.pr_addr_list);
        flatbuffers::Offset<flatbuffers::Vector<uint32_t>> in_addr_list = mFbb.CreateVector<uint32_t>(tle.in_addr_list);
        flatbuffers::Offset<flatbuffers::Vector<uint32_t>> ou_addr_list = mFbb.CreateVector<uint32_t>(tle.ou_addr_list);
        flatbuffers::Offset<flatbuffers::Vector<uint32_t>> wt_addr_list = mFbb.CreateVector<uint32_t>(tle.wt_addr_list);
        flatbuffers::Offset<flatbuffers::Vector<uint32_t>> bs_addr_list = mFbb.CreateVector<uint32_t>(tle.bs_addr_list);
        flatbuffers::Offset<flatbuffers::Vector<uint32_t>> fm_addr_list = mFbb.CreateVector<uint32_t>(tle.fm_addr_list);
        flatbuffers::Offset<flatbuffers::Vector<uint32_t>> lu_addr_list = mFbb.CreateVector<uint32_t>(tle.lu_addr_list);
        flatbuffers::Offset<flatbuffers::String> name = mFbb.CreateString(tle.name.c_str());
        flatbuffers::Offset<flatbuffers::String> type = mFbb.CreateString(tle.name.c_str());
        
        hisdk::hmodel::TaskListEntryBuilder tleb(mFbb);
        tleb.add_pr_addr_list(pr_addr_list);
        tleb.add_in_addr_list(in_addr_list);
        tleb.add_ou_addr_list(ou_addr_list);
        tleb.add_wt_addr_list(wt_addr_list);
        tleb.add_bs_addr_list(bs_addr_list);
        tleb.add_fm_addr_list(fm_addr_list);
        tleb.add_lu_addr_list(lu_addr_list);
        tleb.add_id(tle.id);
        hisdk::hmodel::Interface if_id;
        if ( tle.interface == TaskListEntry::interface_GPU() ) {
            if_id = hisdk::hmodel::Interface_GPU;
        } else if ( tle.interface == TaskListEntry::interface_CPU() ) {
            if_id = hisdk::hmodel::Interface_CPU;
        } else {
            if_id = hisdk::hmodel::Interface_NONE;
        }
        tleb.add_interface(if_id);
        tleb.add_name(name);
        tleb.add_type(type);
        task_list.push_back(tleb.Finish());
    }

    for ( size_t mi = 0, MI = mMemoryListEntries.size(); mi != MI; ++mi) 
    {
        const MemoryListEntry & mle = mMemoryListEntries[mi];
        flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> contents = mFbb.CreateVectorOfStrings(mle.contents);
        flatbuffers::Offset<flatbuffers::Vector<uint64_t>> offsets = mFbb.CreateVector<uint64_t>(mle.offsets);

        hisdk::hmodel::MemoryListEntryBuilder mleb(mFbb);
        mleb.add_size(mle.size);
        mleb.add_alignment(mle.alignment);
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

bool hModel::getSerializedData(uint8_t *buffer) const
{
    uint8_t *tmp = NULL;
    uint64_t size = 0;

    if (buffer == NULL)
        return false;

    size = mFbb.GetSize();
    tmp = mFbb.GetBufferPointer();
    memcpy(buffer, tmp, size);

    return true;
}

bool hModel::getSerializedDataSize(uint64_t *size) const
{
    if (size == NULL)
        return false;

    *size = mFbb.GetSize();

    return true;
}

bool hModel::deserializeFrom(uint8_t *flatbuf)
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
        std::string tle_name = tli->name()->str();
        std::string tle_type = tli->type()->str();
        
        tle.id = tli->id();
        tle.name = tle_name;
        tle.type = tle_type;
        tle.interface = tli->interface();

        if ( tli->pr_addr_list() ) {
            flatbuffers::Vector<unsigned int>::const_iterator ali = tli->pr_addr_list()->begin();
            for (; ali != tli->pr_addr_list()->end(); ++ali) {
                tle.pr_addr_list.push_back(*ali);
            }
        }
        if ( tli->in_addr_list() ) {
            flatbuffers::Vector<unsigned int>::const_iterator ali = tli->in_addr_list()->begin();
            for (; ali != tli->in_addr_list()->end(); ++ali) {
                tle.in_addr_list.push_back(*ali);
            }
        }
        if ( tli->ou_addr_list() ) {
            flatbuffers::Vector<unsigned int>::const_iterator ali = tli->ou_addr_list()->begin();
            for (; ali != tli->ou_addr_list()->end(); ++ali) {
                tle.ou_addr_list.push_back(*ali);
            }
        }
        if ( tli->wt_addr_list() ) {
            flatbuffers::Vector<unsigned int>::const_iterator ali = tli->wt_addr_list()->begin();
            for (; ali != tli->wt_addr_list()->end(); ++ali) {
                tle.wt_addr_list.push_back(*ali);
            }
        }
        if ( tli->bs_addr_list() ) {
            flatbuffers::Vector<unsigned int>::const_iterator ali = tli->bs_addr_list()->begin();
            for (; ali != tli->bs_addr_list()->end(); ++ali) {
                tle.bs_addr_list.push_back(*ali);
            }
        }
        if ( tli->fm_addr_list() ) {
            flatbuffers::Vector<unsigned int>::const_iterator ali = tli->fm_addr_list()->begin();
            for (; ali != tli->fm_addr_list()->end(); ++ali) {
                tle.fm_addr_list.push_back(*ali);
            }
        }
        if ( tli->lu_addr_list() ) {
            flatbuffers::Vector<unsigned int>::const_iterator ali = tli->lu_addr_list()->begin();
            for (; ali != tli->lu_addr_list()->end(); ++ali) {
                tle.lu_addr_list.push_back(*ali);
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
        
        if ( li->contents() ) {
            flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String> >::const_iterator mli = li->contents()->begin();
            for (; mli != li->contents()->end(); ++mli) {
                mle.contents.push_back(mli->str());
            }
        }

        if ( li->offsets() ) {
            flatbuffers::Vector<uint64_t>::const_iterator mli = li->offsets()->begin();
            for (; mli != li->offsets()->end(); ++mli) {
                mle.offsets.push_back(*mli);
            }
        }

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

        uint8_t *blob_data = new uint8_t[mSymbols[blob_name].size];
        memset(blob_data, 0, mSymbols[blob_name].size);

        uint8_t *binblob = (uint8_t *)bi->data()->Data();
        memcpy((void*)blob_data, (void *)binblob, mSymbols[blob_name].size);

        mSymbols[blob_name].data = blob_data;

    }

    return true;
}

} // hisdk
