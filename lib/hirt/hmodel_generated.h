// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_HMODEL_HISDK_HMODEL_H_
#define FLATBUFFERS_GENERATED_HMODEL_HISDK_HMODEL_H_

#include "flatbuffers/flatbuffers.h"

namespace hisdk {
namespace hmodel {

struct Version;

struct Blob;
struct BlobBuilder;

struct MemoryListEntry;
struct MemoryListEntryBuilder;

struct TaskListEntry;
struct TaskListEntryBuilder;

struct AddressListEntry;
struct AddressListEntryBuilder;

struct HModel;
struct HModelBuilder;

enum Interface {
  Interface_NONE = 0,
  Interface_GPU = 1,
  Interface_CPU = 2,
  Interface_MIN = Interface_NONE,
  Interface_MAX = Interface_CPU
};

inline const Interface (&EnumValuesInterface())[3] {
  static const Interface values[] = {
    Interface_NONE,
    Interface_GPU,
    Interface_CPU
  };
  return values;
}

inline const char * const *EnumNamesInterface() {
  static const char * const names[4] = {
    "NONE",
    "GPU",
    "CPU",
    nullptr
  };
  return names;
}

inline const char *EnumNameInterface(Interface e) {
  if (flatbuffers::IsOutRange(e, Interface_NONE, Interface_CPU)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesInterface()[index];
}

enum HModelVersionMain {
  HModelVersionMain_VAL = 0,
  HModelVersionMain_MIN = HModelVersionMain_VAL,
  HModelVersionMain_MAX = HModelVersionMain_VAL
};

inline const HModelVersionMain (&EnumValuesHModelVersionMain())[1] {
  static const HModelVersionMain values[] = {
    HModelVersionMain_VAL
  };
  return values;
}

inline const char * const *EnumNamesHModelVersionMain() {
  static const char * const names[2] = {
    "VAL",
    nullptr
  };
  return names;
}

inline const char *EnumNameHModelVersionMain(HModelVersionMain e) {
  if (flatbuffers::IsOutRange(e, HModelVersionMain_VAL, HModelVersionMain_VAL)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesHModelVersionMain()[index];
}

enum HModelVersionSub {
  HModelVersionSub_VAL = 7,
  HModelVersionSub_MIN = HModelVersionSub_VAL,
  HModelVersionSub_MAX = HModelVersionSub_VAL
};

inline const HModelVersionSub (&EnumValuesHModelVersionSub())[1] {
  static const HModelVersionSub values[] = {
    HModelVersionSub_VAL
  };
  return values;
}

inline const char * const *EnumNamesHModelVersionSub() {
  static const char * const names[2] = {
    "VAL",
    nullptr
  };
  return names;
}

inline const char *EnumNameHModelVersionSub(HModelVersionSub e) {
  if (flatbuffers::IsOutRange(e, HModelVersionSub_VAL, HModelVersionSub_VAL)) return "";
  const size_t index = static_cast<size_t>(e) - static_cast<size_t>(HModelVersionSub_VAL);
  return EnumNamesHModelVersionSub()[index];
}

enum HModelVersionSubMinor {
  HModelVersionSubMinor_VAL = 0,
  HModelVersionSubMinor_MIN = HModelVersionSubMinor_VAL,
  HModelVersionSubMinor_MAX = HModelVersionSubMinor_VAL
};

inline const HModelVersionSubMinor (&EnumValuesHModelVersionSubMinor())[1] {
  static const HModelVersionSubMinor values[] = {
    HModelVersionSubMinor_VAL
  };
  return values;
}

inline const char * const *EnumNamesHModelVersionSubMinor() {
  static const char * const names[2] = {
    "VAL",
    nullptr
  };
  return names;
}

inline const char *EnumNameHModelVersionSubMinor(HModelVersionSubMinor e) {
  if (flatbuffers::IsOutRange(e, HModelVersionSubMinor_VAL, HModelVersionSubMinor_VAL)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesHModelVersionSubMinor()[index];
}

enum MemoryDomain {
  MemoryDomain_GMEM = 0,
  MemoryDomain_MRAB = 1,
  MemoryDomain_MIN = MemoryDomain_GMEM,
  MemoryDomain_MAX = MemoryDomain_MRAB
};

inline const MemoryDomain (&EnumValuesMemoryDomain())[2] {
  static const MemoryDomain values[] = {
    MemoryDomain_GMEM,
    MemoryDomain_MRAB
  };
  return values;
}

inline const char * const *EnumNamesMemoryDomain() {
  static const char * const names[3] = {
    "GMEM",
    "MRAB",
    nullptr
  };
  return names;
}

inline const char *EnumNameMemoryDomain(MemoryDomain e) {
  if (flatbuffers::IsOutRange(e, MemoryDomain_GMEM, MemoryDomain_MRAB)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesMemoryDomain()[index];
}

enum MemoryFlags {
  MemoryFlags_NONE = 0,
  MemoryFlags_ALLOC = 1,
  MemoryFlags_SET = 2,
  MemoryFlags_INPUT = 4,
  MemoryFlags_OUTPUT = 8,
  MemoryFlags_MIN = MemoryFlags_NONE,
  MemoryFlags_MAX = MemoryFlags_OUTPUT
};

inline const MemoryFlags (&EnumValuesMemoryFlags())[5] {
  static const MemoryFlags values[] = {
    MemoryFlags_NONE,
    MemoryFlags_ALLOC,
    MemoryFlags_SET,
    MemoryFlags_INPUT,
    MemoryFlags_OUTPUT
  };
  return values;
}

inline const char * const *EnumNamesMemoryFlags() {
  static const char * const names[10] = {
    "NONE",
    "ALLOC",
    "SET",
    "",
    "INPUT",
    "",
    "",
    "",
    "OUTPUT",
    nullptr
  };
  return names;
}

inline const char *EnumNameMemoryFlags(MemoryFlags e) {
  if (flatbuffers::IsOutRange(e, MemoryFlags_NONE, MemoryFlags_OUTPUT)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesMemoryFlags()[index];
}

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(1) Version FLATBUFFERS_FINAL_CLASS {
 private:
  uint8_t main_;
  uint8_t sub_;
  uint8_t sub_minor_;

 public:
  Version()
      : main_(0),
        sub_(0),
        sub_minor_(0) {
  }
  Version(uint8_t _main, uint8_t _sub, uint8_t _sub_minor)
      : main_(flatbuffers::EndianScalar(_main)),
        sub_(flatbuffers::EndianScalar(_sub)),
        sub_minor_(flatbuffers::EndianScalar(_sub_minor)) {
  }
  uint8_t main() const {
    return flatbuffers::EndianScalar(main_);
  }
  uint8_t sub() const {
    return flatbuffers::EndianScalar(sub_);
  }
  uint8_t sub_minor() const {
    return flatbuffers::EndianScalar(sub_minor_);
  }
};
FLATBUFFERS_STRUCT_END(Version, 3);

struct Blob FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef BlobBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_SIZE = 6,
    VT_INTERFACE = 8,
    VT_TYPE = 10,
    VT_VERSION = 12,
    VT_DATA = 14
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  uint64_t size() const {
    return GetField<uint64_t>(VT_SIZE, 0);
  }
  hisdk::hmodel::Interface interface() const {
    return static_cast<hisdk::hmodel::Interface>(GetField<uint32_t>(VT_INTERFACE, 0));
  }
  uint32_t type() const {
    return GetField<uint32_t>(VT_TYPE, 0);
  }
  const hisdk::hmodel::Version *version() const {
    return GetStruct<const hisdk::hmodel::Version *>(VT_VERSION);
  }
  const flatbuffers::Vector<uint8_t> *data() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<uint64_t>(verifier, VT_SIZE) &&
           VerifyField<uint32_t>(verifier, VT_INTERFACE) &&
           VerifyField<uint32_t>(verifier, VT_TYPE) &&
           VerifyField<hisdk::hmodel::Version>(verifier, VT_VERSION) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct BlobBuilder {
  typedef Blob Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Blob::VT_NAME, name);
  }
  void add_size(uint64_t size) {
    fbb_.AddElement<uint64_t>(Blob::VT_SIZE, size, 0);
  }
  void add_interface(hisdk::hmodel::Interface interface) {
    fbb_.AddElement<uint32_t>(Blob::VT_INTERFACE, static_cast<uint32_t>(interface), 0);
  }
  void add_type(uint32_t type) {
    fbb_.AddElement<uint32_t>(Blob::VT_TYPE, type, 0);
  }
  void add_version(const hisdk::hmodel::Version *version) {
    fbb_.AddStruct(Blob::VT_VERSION, version);
  }
  void add_data(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data) {
    fbb_.AddOffset(Blob::VT_DATA, data);
  }
  explicit BlobBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Blob> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Blob>(end);
    return o;
  }
};

inline flatbuffers::Offset<Blob> CreateBlob(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    uint64_t size = 0,
    hisdk::hmodel::Interface interface = hisdk::hmodel::Interface_NONE,
    uint32_t type = 0,
    const hisdk::hmodel::Version *version = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data = 0) {
  BlobBuilder builder_(_fbb);
  builder_.add_size(size);
  builder_.add_data(data);
  builder_.add_version(version);
  builder_.add_type(type);
  builder_.add_interface(interface);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Blob> CreateBlobDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    uint64_t size = 0,
    hisdk::hmodel::Interface interface = hisdk::hmodel::Interface_NONE,
    uint32_t type = 0,
    const hisdk::hmodel::Version *version = 0,
    const std::vector<uint8_t> *data = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto data__ = data ? _fbb.CreateVector<uint8_t>(*data) : 0;
  return hisdk::hmodel::CreateBlob(
      _fbb,
      name__,
      size,
      interface,
      type,
      version,
      data__);
}

struct MemoryListEntry FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MemoryListEntryBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_DOMAIN = 6,
    VT_FLAGS = 8,
    VT_SIZE = 10,
    VT_ALIGNMENT = 12,
    VT_CONTENTS = 14,
    VT_OFFSETS = 16
  };
  uint32_t id() const {
    return GetField<uint32_t>(VT_ID, 0);
  }
  hisdk::hmodel::MemoryDomain domain() const {
    return static_cast<hisdk::hmodel::MemoryDomain>(GetField<uint32_t>(VT_DOMAIN, 0));
  }
  hisdk::hmodel::MemoryFlags flags() const {
    return static_cast<hisdk::hmodel::MemoryFlags>(GetField<uint32_t>(VT_FLAGS, 0));
  }
  uint64_t size() const {
    return GetField<uint64_t>(VT_SIZE, 0);
  }
  uint32_t alignment() const {
    return GetField<uint32_t>(VT_ALIGNMENT, 0);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *contents() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_CONTENTS);
  }
  const flatbuffers::Vector<uint64_t> *offsets() const {
    return GetPointer<const flatbuffers::Vector<uint64_t> *>(VT_OFFSETS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_ID) &&
           VerifyField<uint32_t>(verifier, VT_DOMAIN) &&
           VerifyField<uint32_t>(verifier, VT_FLAGS) &&
           VerifyField<uint64_t>(verifier, VT_SIZE) &&
           VerifyField<uint32_t>(verifier, VT_ALIGNMENT) &&
           VerifyOffset(verifier, VT_CONTENTS) &&
           verifier.VerifyVector(contents()) &&
           verifier.VerifyVectorOfStrings(contents()) &&
           VerifyOffset(verifier, VT_OFFSETS) &&
           verifier.VerifyVector(offsets()) &&
           verifier.EndTable();
  }
};

struct MemoryListEntryBuilder {
  typedef MemoryListEntry Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) {
    fbb_.AddElement<uint32_t>(MemoryListEntry::VT_ID, id, 0);
  }
  void add_domain(hisdk::hmodel::MemoryDomain domain) {
    fbb_.AddElement<uint32_t>(MemoryListEntry::VT_DOMAIN, static_cast<uint32_t>(domain), 0);
  }
  void add_flags(hisdk::hmodel::MemoryFlags flags) {
    fbb_.AddElement<uint32_t>(MemoryListEntry::VT_FLAGS, static_cast<uint32_t>(flags), 0);
  }
  void add_size(uint64_t size) {
    fbb_.AddElement<uint64_t>(MemoryListEntry::VT_SIZE, size, 0);
  }
  void add_alignment(uint32_t alignment) {
    fbb_.AddElement<uint32_t>(MemoryListEntry::VT_ALIGNMENT, alignment, 0);
  }
  void add_contents(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> contents) {
    fbb_.AddOffset(MemoryListEntry::VT_CONTENTS, contents);
  }
  void add_offsets(flatbuffers::Offset<flatbuffers::Vector<uint64_t>> offsets) {
    fbb_.AddOffset(MemoryListEntry::VT_OFFSETS, offsets);
  }
  explicit MemoryListEntryBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<MemoryListEntry> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<MemoryListEntry>(end);
    return o;
  }
};

inline flatbuffers::Offset<MemoryListEntry> CreateMemoryListEntry(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t id = 0,
    hisdk::hmodel::MemoryDomain domain = hisdk::hmodel::MemoryDomain_GMEM,
    hisdk::hmodel::MemoryFlags flags = hisdk::hmodel::MemoryFlags_NONE,
    uint64_t size = 0,
    uint32_t alignment = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> contents = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint64_t>> offsets = 0) {
  MemoryListEntryBuilder builder_(_fbb);
  builder_.add_size(size);
  builder_.add_offsets(offsets);
  builder_.add_contents(contents);
  builder_.add_alignment(alignment);
  builder_.add_flags(flags);
  builder_.add_domain(domain);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<MemoryListEntry> CreateMemoryListEntryDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t id = 0,
    hisdk::hmodel::MemoryDomain domain = hisdk::hmodel::MemoryDomain_GMEM,
    hisdk::hmodel::MemoryFlags flags = hisdk::hmodel::MemoryFlags_NONE,
    uint64_t size = 0,
    uint32_t alignment = 0,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *contents = nullptr,
    const std::vector<uint64_t> *offsets = nullptr) {
  auto contents__ = contents ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*contents) : 0;
  auto offsets__ = offsets ? _fbb.CreateVector<uint64_t>(*offsets) : 0;
  return hisdk::hmodel::CreateMemoryListEntry(
      _fbb,
      id,
      domain,
      flags,
      size,
      alignment,
      contents__,
      offsets__);
}

struct TaskListEntry FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef TaskListEntryBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_INTERFACE = 6,
    VT_ADDRESS_LIST = 8
  };
  uint32_t id() const {
    return GetField<uint32_t>(VT_ID, 0);
  }
  hisdk::hmodel::Interface interface() const {
    return static_cast<hisdk::hmodel::Interface>(GetField<uint32_t>(VT_INTERFACE, 0));
  }
  const flatbuffers::Vector<uint32_t> *address_list() const {
    return GetPointer<const flatbuffers::Vector<uint32_t> *>(VT_ADDRESS_LIST);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_ID) &&
           VerifyField<uint32_t>(verifier, VT_INTERFACE) &&
           VerifyOffset(verifier, VT_ADDRESS_LIST) &&
           verifier.VerifyVector(address_list()) &&
           verifier.EndTable();
  }
};

struct TaskListEntryBuilder {
  typedef TaskListEntry Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) {
    fbb_.AddElement<uint32_t>(TaskListEntry::VT_ID, id, 0);
  }
  void add_interface(hisdk::hmodel::Interface interface) {
    fbb_.AddElement<uint32_t>(TaskListEntry::VT_INTERFACE, static_cast<uint32_t>(interface), 0);
  }
  void add_address_list(flatbuffers::Offset<flatbuffers::Vector<uint32_t>> address_list) {
    fbb_.AddOffset(TaskListEntry::VT_ADDRESS_LIST, address_list);
  }
  explicit TaskListEntryBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<TaskListEntry> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<TaskListEntry>(end);
    return o;
  }
};

inline flatbuffers::Offset<TaskListEntry> CreateTaskListEntry(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t id = 0,
    hisdk::hmodel::Interface interface = hisdk::hmodel::Interface_NONE,
    flatbuffers::Offset<flatbuffers::Vector<uint32_t>> address_list = 0) {
  TaskListEntryBuilder builder_(_fbb);
  builder_.add_address_list(address_list);
  builder_.add_interface(interface);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<TaskListEntry> CreateTaskListEntryDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t id = 0,
    hisdk::hmodel::Interface interface = hisdk::hmodel::Interface_NONE,
    const std::vector<uint32_t> *address_list = nullptr) {
  auto address_list__ = address_list ? _fbb.CreateVector<uint32_t>(*address_list) : 0;
  return hisdk::hmodel::CreateTaskListEntry(
      _fbb,
      id,
      interface,
      address_list__);
}

struct AddressListEntry FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AddressListEntryBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_MEM_ID = 6,
    VT_OFFSET = 8,
    VT_SIZE = 10
  };
  uint32_t id() const {
    return GetField<uint32_t>(VT_ID, 0);
  }
  uint32_t mem_id() const {
    return GetField<uint32_t>(VT_MEM_ID, 0);
  }
  uint64_t offset() const {
    return GetField<uint64_t>(VT_OFFSET, 0);
  }
  uint64_t size() const {
    return GetField<uint64_t>(VT_SIZE, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_ID) &&
           VerifyField<uint32_t>(verifier, VT_MEM_ID) &&
           VerifyField<uint64_t>(verifier, VT_OFFSET) &&
           VerifyField<uint64_t>(verifier, VT_SIZE) &&
           verifier.EndTable();
  }
};

struct AddressListEntryBuilder {
  typedef AddressListEntry Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) {
    fbb_.AddElement<uint32_t>(AddressListEntry::VT_ID, id, 0);
  }
  void add_mem_id(uint32_t mem_id) {
    fbb_.AddElement<uint32_t>(AddressListEntry::VT_MEM_ID, mem_id, 0);
  }
  void add_offset(uint64_t offset) {
    fbb_.AddElement<uint64_t>(AddressListEntry::VT_OFFSET, offset, 0);
  }
  void add_size(uint64_t size) {
    fbb_.AddElement<uint64_t>(AddressListEntry::VT_SIZE, size, 0);
  }
  explicit AddressListEntryBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<AddressListEntry> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<AddressListEntry>(end);
    return o;
  }
};

inline flatbuffers::Offset<AddressListEntry> CreateAddressListEntry(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t id = 0,
    uint32_t mem_id = 0,
    uint64_t offset = 0,
    uint64_t size = 0) {
  AddressListEntryBuilder builder_(_fbb);
  builder_.add_size(size);
  builder_.add_offset(offset);
  builder_.add_mem_id(mem_id);
  builder_.add_id(id);
  return builder_.Finish();
}

struct HModel FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef HModelBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_VERSION = 4,
    VT_TASK_LIST = 6,
    VT_ADDRESS_LIST = 8,
    VT_MEMORY_LIST = 10,
    VT_BLOBS = 12
  };
  const hisdk::hmodel::Version *version() const {
    return GetStruct<const hisdk::hmodel::Version *>(VT_VERSION);
  }
  const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::TaskListEntry>> *task_list() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::TaskListEntry>> *>(VT_TASK_LIST);
  }
  const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::AddressListEntry>> *address_list() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::AddressListEntry>> *>(VT_ADDRESS_LIST);
  }
  const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::MemoryListEntry>> *memory_list() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::MemoryListEntry>> *>(VT_MEMORY_LIST);
  }
  const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::Blob>> *blobs() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::Blob>> *>(VT_BLOBS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyFieldRequired<hisdk::hmodel::Version>(verifier, VT_VERSION) &&
           VerifyOffset(verifier, VT_TASK_LIST) &&
           verifier.VerifyVector(task_list()) &&
           verifier.VerifyVectorOfTables(task_list()) &&
           VerifyOffset(verifier, VT_ADDRESS_LIST) &&
           verifier.VerifyVector(address_list()) &&
           verifier.VerifyVectorOfTables(address_list()) &&
           VerifyOffset(verifier, VT_MEMORY_LIST) &&
           verifier.VerifyVector(memory_list()) &&
           verifier.VerifyVectorOfTables(memory_list()) &&
           VerifyOffset(verifier, VT_BLOBS) &&
           verifier.VerifyVector(blobs()) &&
           verifier.VerifyVectorOfTables(blobs()) &&
           verifier.EndTable();
  }
};

struct HModelBuilder {
  typedef HModel Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_version(const hisdk::hmodel::Version *version) {
    fbb_.AddStruct(HModel::VT_VERSION, version);
  }
  void add_task_list(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::TaskListEntry>>> task_list) {
    fbb_.AddOffset(HModel::VT_TASK_LIST, task_list);
  }
  void add_address_list(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::AddressListEntry>>> address_list) {
    fbb_.AddOffset(HModel::VT_ADDRESS_LIST, address_list);
  }
  void add_memory_list(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::MemoryListEntry>>> memory_list) {
    fbb_.AddOffset(HModel::VT_MEMORY_LIST, memory_list);
  }
  void add_blobs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::Blob>>> blobs) {
    fbb_.AddOffset(HModel::VT_BLOBS, blobs);
  }
  explicit HModelBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<HModel> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<HModel>(end);
    fbb_.Required(o, HModel::VT_VERSION);
    return o;
  }
};

inline flatbuffers::Offset<HModel> CreateHModel(
    flatbuffers::FlatBufferBuilder &_fbb,
    const hisdk::hmodel::Version *version = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::TaskListEntry>>> task_list = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::AddressListEntry>>> address_list = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::MemoryListEntry>>> memory_list = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<hisdk::hmodel::Blob>>> blobs = 0) {
  HModelBuilder builder_(_fbb);
  builder_.add_blobs(blobs);
  builder_.add_memory_list(memory_list);
  builder_.add_address_list(address_list);
  builder_.add_task_list(task_list);
  builder_.add_version(version);
  return builder_.Finish();
}

inline flatbuffers::Offset<HModel> CreateHModelDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const hisdk::hmodel::Version *version = 0,
    const std::vector<flatbuffers::Offset<hisdk::hmodel::TaskListEntry>> *task_list = nullptr,
    const std::vector<flatbuffers::Offset<hisdk::hmodel::AddressListEntry>> *address_list = nullptr,
    const std::vector<flatbuffers::Offset<hisdk::hmodel::MemoryListEntry>> *memory_list = nullptr,
    const std::vector<flatbuffers::Offset<hisdk::hmodel::Blob>> *blobs = nullptr) {
  auto task_list__ = task_list ? _fbb.CreateVector<flatbuffers::Offset<hisdk::hmodel::TaskListEntry>>(*task_list) : 0;
  auto address_list__ = address_list ? _fbb.CreateVector<flatbuffers::Offset<hisdk::hmodel::AddressListEntry>>(*address_list) : 0;
  auto memory_list__ = memory_list ? _fbb.CreateVector<flatbuffers::Offset<hisdk::hmodel::MemoryListEntry>>(*memory_list) : 0;
  auto blobs__ = blobs ? _fbb.CreateVector<flatbuffers::Offset<hisdk::hmodel::Blob>>(*blobs) : 0;
  return hisdk::hmodel::CreateHModel(
      _fbb,
      version,
      task_list__,
      address_list__,
      memory_list__,
      blobs__);
}

inline const hisdk::hmodel::HModel *GetHModel(const void *buf) {
  return flatbuffers::GetRoot<hisdk::hmodel::HModel>(buf);
}

inline const hisdk::hmodel::HModel *GetSizePrefixedHModel(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<hisdk::hmodel::HModel>(buf);
}

inline bool VerifyHModelBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<hisdk::hmodel::HModel>(nullptr);
}

inline bool VerifySizePrefixedHModelBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<hisdk::hmodel::HModel>(nullptr);
}

inline void FinishHModelBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<hisdk::hmodel::HModel> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedHModelBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<hisdk::hmodel::HModel> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace hmodel
}  // namespace hisdk

#endif  // FLATBUFFERS_GENERATED_HMODEL_HISDK_HMODEL_H_
