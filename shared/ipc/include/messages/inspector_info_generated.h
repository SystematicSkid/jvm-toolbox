// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_INSPECTORINFO_JVM_TOOLBOX_FLATBUFFERS_INSPECTOR_H_
#define FLATBUFFERS_GENERATED_INSPECTORINFO_JVM_TOOLBOX_FLATBUFFERS_INSPECTOR_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 &&
              FLATBUFFERS_VERSION_MINOR == 5 &&
              FLATBUFFERS_VERSION_REVISION == 9,
             "Non-compatible flatbuffers version included");

namespace jvm_toolbox_flatbuffers {
namespace inspector {

struct OnInspectorLoaded;
struct OnInspectorLoadedBuilder;

struct OnInspectorLoaded FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef OnInspectorLoadedBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_INSPECTOR_INTERFACE = 4
  };
  const ::flatbuffers::String *inspector_interface() const {
    return GetPointer<const ::flatbuffers::String *>(VT_INSPECTOR_INTERFACE);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_INSPECTOR_INTERFACE) &&
           verifier.VerifyString(inspector_interface()) &&
           verifier.EndTable();
  }
};

struct OnInspectorLoadedBuilder {
  typedef OnInspectorLoaded Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_inspector_interface(::flatbuffers::Offset<::flatbuffers::String> inspector_interface) {
    fbb_.AddOffset(OnInspectorLoaded::VT_INSPECTOR_INTERFACE, inspector_interface);
  }
  explicit OnInspectorLoadedBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<OnInspectorLoaded> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<OnInspectorLoaded>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<OnInspectorLoaded> CreateOnInspectorLoaded(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> inspector_interface = 0) {
  OnInspectorLoadedBuilder builder_(_fbb);
  builder_.add_inspector_interface(inspector_interface);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<OnInspectorLoaded> CreateOnInspectorLoadedDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *inspector_interface = nullptr) {
  auto inspector_interface__ = inspector_interface ? _fbb.CreateString(inspector_interface) : 0;
  return jvm_toolbox_flatbuffers::inspector::CreateOnInspectorLoaded(
      _fbb,
      inspector_interface__);
}

}  // namespace inspector
}  // namespace jvm_toolbox_flatbuffers

#endif  // FLATBUFFERS_GENERATED_INSPECTORINFO_JVM_TOOLBOX_FLATBUFFERS_INSPECTOR_H_