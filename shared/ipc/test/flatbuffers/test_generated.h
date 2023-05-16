// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TEST_JVM_TOOLBOX_TEST_H_
#define FLATBUFFERS_GENERATED_TEST_JVM_TOOLBOX_TEST_H_

#include "../../../includes/flatbuffers/flatbuffers.h"

namespace jvm_toolbox {
namespace test {

struct basic_table;
struct basic_tableBuilder;

struct basic_table FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef basic_tableBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_A = 4,
    VT_B = 6,
    VT_C = 8
  };
  int16_t a() const {
    return GetField<int16_t>(VT_A, 0);
  }
  int16_t b() const {
    return GetField<int16_t>(VT_B, 0);
  }
  int16_t c() const {
    return GetField<int16_t>(VT_C, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int16_t>(verifier, VT_A) &&
           VerifyField<int16_t>(verifier, VT_B) &&
           VerifyField<int16_t>(verifier, VT_C) &&
           verifier.EndTable();
  }
};

struct basic_tableBuilder {
  typedef basic_table Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_a(int16_t a) {
    fbb_.AddElement<int16_t>(basic_table::VT_A, a, 0);
  }
  void add_b(int16_t b) {
    fbb_.AddElement<int16_t>(basic_table::VT_B, b, 0);
  }
  void add_c(int16_t c) {
    fbb_.AddElement<int16_t>(basic_table::VT_C, c, 0);
  }
  explicit basic_tableBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<basic_table> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<basic_table>(end);
    return o;
  }
};

inline flatbuffers::Offset<basic_table> Createbasic_table(
    flatbuffers::FlatBufferBuilder &_fbb,
    int16_t a = 0,
    int16_t b = 0,
    int16_t c = 0) {
  basic_tableBuilder builder_(_fbb);
  builder_.add_c(c);
  builder_.add_b(b);
  builder_.add_a(a);
  return builder_.Finish();
}

}  // namespace test
}  // namespace jvm_toolbox

#endif  // FLATBUFFERS_GENERATED_TEST_JVM_TOOLBOX_TEST_H_