// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: messages_robocup_ssl_wrapper.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "messages_robocup_ssl_wrapper.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* SSL_WrapperPacket_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SSL_WrapperPacket_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_messages_5frobocup_5fssl_5fwrapper_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_messages_5frobocup_5fssl_5fwrapper_2eproto() {
  protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "messages_robocup_ssl_wrapper.proto");
  GOOGLE_CHECK(file != NULL);
  SSL_WrapperPacket_descriptor_ = file->message_type(0);
  static const int SSL_WrapperPacket_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_WrapperPacket, detection_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_WrapperPacket, geometry_),
  };
  SSL_WrapperPacket_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      SSL_WrapperPacket_descriptor_,
      SSL_WrapperPacket::default_instance_,
      SSL_WrapperPacket_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_WrapperPacket, _has_bits_[0]),
      -1,
      -1,
      sizeof(SSL_WrapperPacket),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_WrapperPacket, _internal_metadata_),
      -1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_messages_5frobocup_5fssl_5fwrapper_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      SSL_WrapperPacket_descriptor_, &SSL_WrapperPacket::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_messages_5frobocup_5fssl_5fwrapper_2eproto() {
  delete SSL_WrapperPacket::default_instance_;
  delete SSL_WrapperPacket_reflection_;
}

void protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::protobuf_AddDesc_messages_5frobocup_5fssl_5fdetection_2eproto();
  ::protobuf_AddDesc_messages_5frobocup_5fssl_5fgeometry_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\"messages_robocup_ssl_wrapper.proto\032$me"
    "ssages_robocup_ssl_detection.proto\032#mess"
    "ages_robocup_ssl_geometry.proto\"`\n\021SSL_W"
    "rapperPacket\022&\n\tdetection\030\001 \001(\0132\023.SSL_De"
    "tectionFrame\022#\n\010geometry\030\002 \001(\0132\021.SSL_Geo"
    "metryData", 209);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "messages_robocup_ssl_wrapper.proto", &protobuf_RegisterTypes);
  SSL_WrapperPacket::default_instance_ = new SSL_WrapperPacket();
  SSL_WrapperPacket::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_messages_5frobocup_5fssl_5fwrapper_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_messages_5frobocup_5fssl_5fwrapper_2eproto {
  StaticDescriptorInitializer_messages_5frobocup_5fssl_5fwrapper_2eproto() {
    protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_2eproto();
  }
} static_descriptor_initializer_messages_5frobocup_5fssl_5fwrapper_2eproto_;

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int SSL_WrapperPacket::kDetectionFieldNumber;
const int SSL_WrapperPacket::kGeometryFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

SSL_WrapperPacket::SSL_WrapperPacket()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:SSL_WrapperPacket)
}

void SSL_WrapperPacket::InitAsDefaultInstance() {
  detection_ = const_cast< ::SSL_DetectionFrame*>(&::SSL_DetectionFrame::default_instance());
  geometry_ = const_cast< ::SSL_GeometryData*>(&::SSL_GeometryData::default_instance());
}

SSL_WrapperPacket::SSL_WrapperPacket(const SSL_WrapperPacket& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:SSL_WrapperPacket)
}

void SSL_WrapperPacket::SharedCtor() {
  _cached_size_ = 0;
  detection_ = NULL;
  geometry_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

SSL_WrapperPacket::~SSL_WrapperPacket() {
  // @@protoc_insertion_point(destructor:SSL_WrapperPacket)
  SharedDtor();
}

void SSL_WrapperPacket::SharedDtor() {
  if (this != default_instance_) {
    delete detection_;
    delete geometry_;
  }
}

void SSL_WrapperPacket::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SSL_WrapperPacket::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SSL_WrapperPacket_descriptor_;
}

const SSL_WrapperPacket& SSL_WrapperPacket::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_2eproto();
  return *default_instance_;
}

SSL_WrapperPacket* SSL_WrapperPacket::default_instance_ = NULL;

SSL_WrapperPacket* SSL_WrapperPacket::New(::google::protobuf::Arena* arena) const {
  SSL_WrapperPacket* n = new SSL_WrapperPacket;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void SSL_WrapperPacket::Clear() {
// @@protoc_insertion_point(message_clear_start:SSL_WrapperPacket)
  if (_has_bits_[0 / 32] & 3u) {
    if (has_detection()) {
      if (detection_ != NULL) detection_->::SSL_DetectionFrame::Clear();
    }
    if (has_geometry()) {
      if (geometry_ != NULL) geometry_->::SSL_GeometryData::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  if (_internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->Clear();
  }
}

bool SSL_WrapperPacket::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:SSL_WrapperPacket)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .SSL_DetectionFrame detection = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_detection()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_geometry;
        break;
      }

      // optional .SSL_GeometryData geometry = 2;
      case 2: {
        if (tag == 18) {
         parse_geometry:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_geometry()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:SSL_WrapperPacket)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:SSL_WrapperPacket)
  return false;
#undef DO_
}

void SSL_WrapperPacket::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:SSL_WrapperPacket)
  // optional .SSL_DetectionFrame detection = 1;
  if (has_detection()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, *this->detection_, output);
  }

  // optional .SSL_GeometryData geometry = 2;
  if (has_geometry()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, *this->geometry_, output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:SSL_WrapperPacket)
}

::google::protobuf::uint8* SSL_WrapperPacket::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:SSL_WrapperPacket)
  // optional .SSL_DetectionFrame detection = 1;
  if (has_detection()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        1, *this->detection_, false, target);
  }

  // optional .SSL_GeometryData geometry = 2;
  if (has_geometry()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        2, *this->geometry_, false, target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:SSL_WrapperPacket)
  return target;
}

int SSL_WrapperPacket::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:SSL_WrapperPacket)
  int total_size = 0;

  if (_has_bits_[0 / 32] & 3u) {
    // optional .SSL_DetectionFrame detection = 1;
    if (has_detection()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->detection_);
    }

    // optional .SSL_GeometryData geometry = 2;
    if (has_geometry()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->geometry_);
    }

  }
  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SSL_WrapperPacket::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:SSL_WrapperPacket)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  const SSL_WrapperPacket* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const SSL_WrapperPacket>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:SSL_WrapperPacket)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:SSL_WrapperPacket)
    MergeFrom(*source);
  }
}

void SSL_WrapperPacket::MergeFrom(const SSL_WrapperPacket& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:SSL_WrapperPacket)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_detection()) {
      mutable_detection()->::SSL_DetectionFrame::MergeFrom(from.detection());
    }
    if (from.has_geometry()) {
      mutable_geometry()->::SSL_GeometryData::MergeFrom(from.geometry());
    }
  }
  if (from._internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->MergeFrom(from.unknown_fields());
  }
}

void SSL_WrapperPacket::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:SSL_WrapperPacket)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SSL_WrapperPacket::CopyFrom(const SSL_WrapperPacket& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:SSL_WrapperPacket)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SSL_WrapperPacket::IsInitialized() const {

  if (has_detection()) {
    if (!this->detection_->IsInitialized()) return false;
  }
  if (has_geometry()) {
    if (!this->geometry_->IsInitialized()) return false;
  }
  return true;
}

void SSL_WrapperPacket::Swap(SSL_WrapperPacket* other) {
  if (other == this) return;
  InternalSwap(other);
}
void SSL_WrapperPacket::InternalSwap(SSL_WrapperPacket* other) {
  std::swap(detection_, other->detection_);
  std::swap(geometry_, other->geometry_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata SSL_WrapperPacket::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SSL_WrapperPacket_descriptor_;
  metadata.reflection = SSL_WrapperPacket_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// SSL_WrapperPacket

// optional .SSL_DetectionFrame detection = 1;
bool SSL_WrapperPacket::has_detection() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void SSL_WrapperPacket::set_has_detection() {
  _has_bits_[0] |= 0x00000001u;
}
void SSL_WrapperPacket::clear_has_detection() {
  _has_bits_[0] &= ~0x00000001u;
}
void SSL_WrapperPacket::clear_detection() {
  if (detection_ != NULL) detection_->::SSL_DetectionFrame::Clear();
  clear_has_detection();
}
const ::SSL_DetectionFrame& SSL_WrapperPacket::detection() const {
  // @@protoc_insertion_point(field_get:SSL_WrapperPacket.detection)
  return detection_ != NULL ? *detection_ : *default_instance_->detection_;
}
::SSL_DetectionFrame* SSL_WrapperPacket::mutable_detection() {
  set_has_detection();
  if (detection_ == NULL) {
    detection_ = new ::SSL_DetectionFrame;
  }
  // @@protoc_insertion_point(field_mutable:SSL_WrapperPacket.detection)
  return detection_;
}
::SSL_DetectionFrame* SSL_WrapperPacket::release_detection() {
  // @@protoc_insertion_point(field_release:SSL_WrapperPacket.detection)
  clear_has_detection();
  ::SSL_DetectionFrame* temp = detection_;
  detection_ = NULL;
  return temp;
}
void SSL_WrapperPacket::set_allocated_detection(::SSL_DetectionFrame* detection) {
  delete detection_;
  detection_ = detection;
  if (detection) {
    set_has_detection();
  } else {
    clear_has_detection();
  }
  // @@protoc_insertion_point(field_set_allocated:SSL_WrapperPacket.detection)
}

// optional .SSL_GeometryData geometry = 2;
bool SSL_WrapperPacket::has_geometry() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
void SSL_WrapperPacket::set_has_geometry() {
  _has_bits_[0] |= 0x00000002u;
}
void SSL_WrapperPacket::clear_has_geometry() {
  _has_bits_[0] &= ~0x00000002u;
}
void SSL_WrapperPacket::clear_geometry() {
  if (geometry_ != NULL) geometry_->::SSL_GeometryData::Clear();
  clear_has_geometry();
}
const ::SSL_GeometryData& SSL_WrapperPacket::geometry() const {
  // @@protoc_insertion_point(field_get:SSL_WrapperPacket.geometry)
  return geometry_ != NULL ? *geometry_ : *default_instance_->geometry_;
}
::SSL_GeometryData* SSL_WrapperPacket::mutable_geometry() {
  set_has_geometry();
  if (geometry_ == NULL) {
    geometry_ = new ::SSL_GeometryData;
  }
  // @@protoc_insertion_point(field_mutable:SSL_WrapperPacket.geometry)
  return geometry_;
}
::SSL_GeometryData* SSL_WrapperPacket::release_geometry() {
  // @@protoc_insertion_point(field_release:SSL_WrapperPacket.geometry)
  clear_has_geometry();
  ::SSL_GeometryData* temp = geometry_;
  geometry_ = NULL;
  return temp;
}
void SSL_WrapperPacket::set_allocated_geometry(::SSL_GeometryData* geometry) {
  delete geometry_;
  geometry_ = geometry;
  if (geometry) {
    set_has_geometry();
  } else {
    clear_has_geometry();
  }
  // @@protoc_insertion_point(field_set_allocated:SSL_WrapperPacket.geometry)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
