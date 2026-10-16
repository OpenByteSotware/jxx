#include "io/jxx.io.ObjectOutputStream.h"

#include <limits>
#include "io/jxx.io.NotSerializableException.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "jxx.initialize.h"
#include "io/jxx.io.ObjectStreamFormat.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.EnumValue.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::io {
namespace { using Tag = serialization::Tag; }

ObjectOutputStream::ObjectOutputStream(const ::jxx::Ptr<OutputStream>& output) : Super(output) {
    ::jxx::initialize();
    writeStreamHeader_();
}
ObjectOutputStream::ObjectOutputStream() : Super(::jxx::Ptr<OutputStream>{}) {}
ObjectOutputStream::~ObjectOutputStream() = default;

void ObjectOutputStream::writeStreamHeader_() {
    writeInt(static_cast<::jxx::lang::jint>(serialization::STREAM_MAGIC));
    writeByte(serialization::STREAM_MAJOR_VERSION);
    writeByte(serialization::STREAM_MINOR_VERSION);
    writeShort(serialization::STREAM_FLAGS);
    writeLong(0);
}
void ObjectOutputStream::writeHandle_(Handle value) { writeLong(static_cast<::jxx::lang::jlong>(value)); }
ObjectOutputStream::Handle ObjectOutputStream::assignHandle_(const ::jxx::Ptr<::jxx::lang::Object>& object) {
    const auto handle = nextHandle_++;
    handles_.emplace(object.get(), handle);
    return handle;
}
void ObjectOutputStream::writeRawUtf8_(const std::string& value) {
    if (value.size() > static_cast<std::size_t>(std::numeric_limits<::jxx::lang::jint>::max()))
        throw NotSerializableException("Metadata string is too large");
    writeInt(static_cast<::jxx::lang::jint>(value.size()));
    for (const unsigned char byte : value) writeByte(byte);
}
void ObjectOutputStream::writeClassDescriptor_(const std::string& binaryName, ::jxx::lang::jlong serialVersionUID) {
    const auto found = descriptorHandles_.find(binaryName);
    if (found != descriptorHandles_.end()) {
        writeByte(static_cast<::jxx::lang::jint>(Tag::ClassDescriptorRef));
        writeHandle_(found->second);
        return;
    }
    const auto handle = nextDescriptorHandle_++;
    descriptorHandles_.emplace(binaryName, handle);
    writeByte(static_cast<::jxx::lang::jint>(Tag::ClassDescriptor));
    writeHandle_(handle);
    writeRawUtf8_(binaryName);
    writeLong(serialVersionUID);
    writeShort(0x0003); // Serializable | CustomWriteObject
    writeInt(0); // no reflected/default fields in JXX v1
    writeByte(static_cast<::jxx::lang::jint>(Tag::EndClassDescriptor));
}
void ObjectOutputStream::writeString_(const ::jxx::Ptr<::jxx::lang::String>& value, Handle handle) {
    writeByte(static_cast<::jxx::lang::jint>(Tag::NewString));
    writeHandle_(handle);
    writeHandle_(static_cast<Handle>(value->length()));
    for (::jxx::lang::jint i = 0; i < value->length(); ++i) writeChar(value->charAt(i));
}
void ObjectOutputStream::writeObject0_(const ::jxx::Ptr<::jxx::lang::Object>& object) {
    if (object == nullptr) { writeByte(static_cast<::jxx::lang::jint>(Tag::Null)); return; }
    const auto found = handles_.find(object.get());
    if (found != handles_.end()) {
        writeByte(static_cast<::jxx::lang::jint>(Tag::Reference));
        writeHandle_(found->second);
        return;
    }
    const auto handle = assignHandle_(object);
    if (const auto* codec = serialization::ArrayCodecRegistry::findByType(std::type_index(typeid(*object)))) {
        writeByte(static_cast<::jxx::lang::jint>(Tag::NewArray));
        writeHandle_(handle);
        writeByte(static_cast<::jxx::lang::jint>(serialization::ArrayElementKind::TypedObject));
        writeByte(static_cast<::jxx::lang::jint>(codec->rank));
        writeHandle_(static_cast<Handle>(codec->length(object)));
        writeRawUtf8_(codec->componentBinaryName);
        for (::jxx::lang::jint i = 0; i < codec->length(object); ++i) writeObject0_(codec->get(object, i));
        writeByte(static_cast<::jxx::lang::jint>(Tag::EndArray));
        return;
    }
    const auto writeRankTwoArrayHeader = [&](serialization::ArrayElementKind kind,
                                             ::jxx::lang::jint length) {
        writeByte(static_cast<::jxx::lang::jint>(Tag::NewArray));
        writeHandle_(handle);
        writeByte(static_cast<::jxx::lang::jint>(kind));
        writeByte(2); // rank
        writeHandle_(static_cast<Handle>(length));
    };
#define JXX_WRITE_RANK2_ARRAY(TYPE, KIND) \
    if (const auto array = std::dynamic_pointer_cast<TYPE>(object)) { \
        writeRankTwoArrayHeader(serialization::ArrayElementKind::KIND, array->length); \
        for (::jxx::lang::jint i = 0; i < array->length; ++i) { \
            writeObject0_(::jxx::CAST<::jxx::lang::Object>(array->row(i))); \
        } \
        writeByte(static_cast<::jxx::lang::jint>(Tag::EndArray)); \
        return; \
    }
    JXX_WRITE_RANK2_ARRAY(::jxx::lang::BooleanArray2DType, Boolean)
    JXX_WRITE_RANK2_ARRAY(::jxx::lang::ByteArray2DType, Byte)
    JXX_WRITE_RANK2_ARRAY(::jxx::lang::CharArray2DType, Char)
    JXX_WRITE_RANK2_ARRAY(::jxx::lang::ShortArray2DType, Short)
    JXX_WRITE_RANK2_ARRAY(::jxx::lang::IntArray2DType, Int)
    JXX_WRITE_RANK2_ARRAY(::jxx::lang::LongArray2DType, Long)
    JXX_WRITE_RANK2_ARRAY(::jxx::lang::FloatArray2DType, Float)
    JXX_WRITE_RANK2_ARRAY(::jxx::lang::DoubleArray2DType, Double)
    JXX_WRITE_RANK2_ARRAY(::jxx::lang::ObjectArray2DType, Object)
#undef JXX_WRITE_RANK2_ARRAY
    const auto writeArrayHeader = [&](serialization::ArrayElementKind kind,
                                      ::jxx::lang::jint length) {
        writeByte(static_cast<::jxx::lang::jint>(Tag::NewArray));
        writeHandle_(handle);
        writeByte(static_cast<::jxx::lang::jint>(kind));
        writeByte(1); // rank
        writeHandle_(static_cast<Handle>(length));
    };
#define JXX_WRITE_PRIMITIVE_ARRAY(TYPE, KIND, WRITE) \
    if (const auto array = std::dynamic_pointer_cast<TYPE>(object)) { \
        writeArrayHeader(serialization::ArrayElementKind::KIND, array->length); \
        for (::jxx::lang::jint i = 0; i < array->length; ++i) WRITE((*array)[i]); \
        writeByte(static_cast<::jxx::lang::jint>(Tag::EndArray)); return; \
    }
    JXX_WRITE_PRIMITIVE_ARRAY(::jxx::lang::BooleanArrayType, Boolean, writeBoolean)
    JXX_WRITE_PRIMITIVE_ARRAY(::jxx::lang::ByteArrayType, Byte, writeByte)
    JXX_WRITE_PRIMITIVE_ARRAY(::jxx::lang::CharArrayType, Char, writeChar)
    JXX_WRITE_PRIMITIVE_ARRAY(::jxx::lang::ShortArrayType, Short, writeShort)
    JXX_WRITE_PRIMITIVE_ARRAY(::jxx::lang::IntArrayType, Int, writeInt)
    JXX_WRITE_PRIMITIVE_ARRAY(::jxx::lang::LongArrayType, Long, writeLong)
    JXX_WRITE_PRIMITIVE_ARRAY(::jxx::lang::FloatArrayType, Float, writeFloat)
    JXX_WRITE_PRIMITIVE_ARRAY(::jxx::lang::DoubleArrayType, Double, writeDouble)
#undef JXX_WRITE_PRIMITIVE_ARRAY
    if (const auto array = std::dynamic_pointer_cast<::jxx::lang::ObjectArrayType>(object)) {
        writeArrayHeader(serialization::ArrayElementKind::Object, array->length);
        for (::jxx::lang::jint i = 0; i < array->length; ++i) writeObject0_((*array)[i]);
        writeByte(static_cast<::jxx::lang::jint>(Tag::EndArray));
        return;
    }
    if (const auto classValue = std::dynamic_pointer_cast<::jxx::lang::ClassAny>(object)) {
        writeByte(static_cast<::jxx::lang::jint>(Tag::NewClass));
        writeHandle_(handle);
        writeRawUtf8_(classValue->getName()->utf8());
        return;
    }
    if (const auto enumValue = std::dynamic_pointer_cast<::jxx::lang::EnumValue>(object)) {
        writeByte(static_cast<::jxx::lang::jint>(Tag::NewEnum));
        writeHandle_(handle);
        writeClassDescriptor_(enumValue->enumDeclaringClass()->getName()->utf8(), 1);
        writeRawUtf8_(enumValue->enumName()->utf8());
        return;
    }
    if (const auto stringValue = std::dynamic_pointer_cast<::jxx::lang::String>(object)) {
        writeString_(stringValue, handle);
        return;
    }
    const auto serializable = std::dynamic_pointer_cast<SerializableI>(object);
    if (serializable == nullptr) throw NotSerializableException(object->getClassName());
    const auto runtimeClass = object->getClass();
    writeByte(static_cast<::jxx::lang::jint>(Tag::NewObject));
    writeHandle_(handle);
    const auto classMetadata = serialization::SerializationClassRegistry::resolve(
        runtimeClass->getName()->utf8());
    writeClassDescriptor_(runtimeClass->getName()->utf8(),
        classMetadata.serialVersionUID);
    serializable->writeObject(jxx::CAST<ObjectOutputStream>(thisPtr()));
    writeByte(static_cast<::jxx::lang::jint>(Tag::EndCustomData));
    writeByte(static_cast<::jxx::lang::jint>(Tag::EndObject));
}
void ObjectOutputStream::writeObject(const ::jxx::Ptr<::jxx::lang::Object>& object) {
    synchronized([&] { writeObject0_(object); });
}
void ObjectOutputStream::writeUnshared(const ::jxx::Ptr<::jxx::lang::Object>& object) {
    synchronized([&] {
        if (object != nullptr) handles_.erase(object.get());
        writeObject0_(object);
        if (object != nullptr) handles_.erase(object.get());
    });
}
void ObjectOutputStream::reset() {
    synchronized([&] {
        writeByte(static_cast<::jxx::lang::jint>(Tag::Reset));
        handles_.clear();
        descriptorHandles_.clear();
        nextHandle_ = serialization::FIRST_HANDLE;
        nextDescriptorHandle_ = serialization::FIRST_HANDLE;
    });
}
} // namespace jxx::io
