#include "io/jxx.io.ObjectInputStream.h"

#include "io/jxx.io.InvalidClassException.h"
#include "io/jxx.io.ObjectStreamFormat.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.StreamCorruptedException.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::io {
namespace { using Tag = serialization::Tag; }
ObjectInputStream::ObjectInputStream(const ::jxx::Ptr<InputStream>& input) : Super(input) { readStreamHeader_(); }
ObjectInputStream::ObjectInputStream() : Super(::jxx::Ptr<InputStream>{}) {}
ObjectInputStream::~ObjectInputStream() = default;

::jxx::lang::jint ObjectInputStream::read() {
    return JxxSuper::read();
}
::jxx::lang::jint ObjectInputStream::read(
    const ::jxx::lang::ByteArray& buffer) {
    return JxxSuper::read(buffer);
}
::jxx::lang::jint ObjectInputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    return JxxSuper::read(buffer, offset, length);
}
::jxx::lang::jlong ObjectInputStream::skip(::jxx::lang::jlong count) {
    return JxxSuper::skip(count);
}
::jxx::lang::jint ObjectInputStream::available() {
    return JxxSuper::available();
}
void ObjectInputStream::readStreamHeader_() {
    if (static_cast<std::uint32_t>(readInt()) != serialization::STREAM_MAGIC ||
        readUnsignedByte() != serialization::STREAM_MAJOR_VERSION ||
        readUnsignedByte() != serialization::STREAM_MINOR_VERSION)
        throw StreamCorruptedException("Invalid JXX object stream header");
    (void)readUnsignedShort();
    if (readLong() != 0) throw StreamCorruptedException("Invalid reserved header bytes");
}
ObjectInputStream::Handle ObjectInputStream::readHandle_() {
    const auto value = readLong();
    if (value <= 0) throw StreamCorruptedException("Invalid object handle");
    return static_cast<Handle>(value);
}
std::string ObjectInputStream::readRawUtf8_() {
    const auto length = readInt();
    if (length < 0 || length > 1024 * 1024) throw StreamCorruptedException("Invalid metadata length");
    std::string value; value.reserve(static_cast<std::size_t>(length));
    for (::jxx::lang::jint i = 0; i < length; ++i) value.push_back(static_cast<char>(readUnsignedByte()));
    return value;
}
void ObjectInputStream::registerHandle_(
    Handle handle, const ::jxx::Ptr<::jxx::lang::Object>& object) {
    if (!handles_.emplace(handle, object).second)
        throw StreamCorruptedException("Duplicate object handle");
}
void ObjectInputStream::requireEndArray_() {
    if (static_cast<Tag>(readUnsignedByte()) != Tag::EndArray)
        throw StreamCorruptedException("Missing JXX array terminator");
}
::jxx::Ptr<::jxx::lang::Object> ObjectInputStream::readArray_(Handle handle) {
    using Kind = serialization::ArrayElementKind;
    const auto kind = static_cast<Kind>(readUnsignedByte());
    const auto rank = readUnsignedByte();
    if (rank != 1 && rank != 2)
        throw StreamCorruptedException("Unsupported JXX array rank");
    const auto rawLength = readHandle_();
    if (rawLength > 64U * 1024U * 1024U) throw StreamCorruptedException("Array length exceeds limit");
    const auto length = static_cast<::jxx::lang::jint>(rawLength);
    if (rank == 2) {
#define JXX_READ_RANK2_ARRAY(KIND, TYPE, ROWTYPE) \
        case Kind::KIND: { auto array = jxx::NEW<TYPE>(length); registerHandle_(handle, array); \
            for (::jxx::lang::jint i = 0; i < length; ++i) { auto row = readObject0_(); \
                (*array)[i] = row == nullptr ? nullptr : jxx::CAST<ROWTYPE>(row); } \
            requireEndArray_(); return array; }
        switch (kind) {
            JXX_READ_RANK2_ARRAY(Boolean, ::jxx::lang::BooleanArray2DType, ::jxx::lang::BooleanArrayType)
            JXX_READ_RANK2_ARRAY(Byte, ::jxx::lang::ByteArray2DType, ::jxx::lang::ByteArrayType)
            JXX_READ_RANK2_ARRAY(Char, ::jxx::lang::CharArray2DType, ::jxx::lang::CharArrayType)
            JXX_READ_RANK2_ARRAY(Short, ::jxx::lang::ShortArray2DType, ::jxx::lang::ShortArrayType)
            JXX_READ_RANK2_ARRAY(Int, ::jxx::lang::IntArray2DType, ::jxx::lang::IntArrayType)
            JXX_READ_RANK2_ARRAY(Long, ::jxx::lang::LongArray2DType, ::jxx::lang::LongArrayType)
            JXX_READ_RANK2_ARRAY(Float, ::jxx::lang::FloatArray2DType, ::jxx::lang::FloatArrayType)
            JXX_READ_RANK2_ARRAY(Double, ::jxx::lang::DoubleArray2DType, ::jxx::lang::DoubleArrayType)
            JXX_READ_RANK2_ARRAY(Object, ::jxx::lang::ObjectArray2DType, ::jxx::lang::ObjectArrayType)
            default: throw StreamCorruptedException("Unknown JXX rank-two array element kind");
        }
#undef JXX_READ_RANK2_ARRAY
    }
#define JXX_READ_PRIMITIVE_ARRAY(KIND, TYPE, READ) \
    case Kind::KIND: { auto array = jxx::NEW<TYPE>(length); registerHandle_(handle, array); \
        for (::jxx::lang::jint i = 0; i < length; ++i) (*array)[i] = READ(); \
        requireEndArray_(); return array; }
    switch (kind) {
        JXX_READ_PRIMITIVE_ARRAY(Boolean, ::jxx::lang::BooleanArrayType, readBoolean)
        JXX_READ_PRIMITIVE_ARRAY(Byte, ::jxx::lang::ByteArrayType, readByte)
        JXX_READ_PRIMITIVE_ARRAY(Char, ::jxx::lang::CharArrayType, readChar)
        JXX_READ_PRIMITIVE_ARRAY(Short, ::jxx::lang::ShortArrayType, readShort)
        JXX_READ_PRIMITIVE_ARRAY(Int, ::jxx::lang::IntArrayType, readInt)
        JXX_READ_PRIMITIVE_ARRAY(Long, ::jxx::lang::LongArrayType, readLong)
        JXX_READ_PRIMITIVE_ARRAY(Float, ::jxx::lang::FloatArrayType, readFloat)
        JXX_READ_PRIMITIVE_ARRAY(Double, ::jxx::lang::DoubleArrayType, readDouble)
#undef JXX_READ_PRIMITIVE_ARRAY
        case Kind::Object: {
            auto array = jxx::NEW<::jxx::lang::ObjectArrayType>(length);
            registerHandle_(handle, array);
            for (::jxx::lang::jint i = 0; i < length; ++i) (*array)[i] = readObject0_();
            requireEndArray_(); return array;
        }
        default: throw StreamCorruptedException("Unknown JXX array element kind");
    }
}
::jxx::Ptr<::jxx::lang::String> ObjectInputStream::readString_(Handle handle) {
    const auto length = readHandle_();
    if (length > 64U * 1024U * 1024U) throw StreamCorruptedException("String length exceeds limit");
    auto chars = jxx::NEW<::jxx::lang::CharArrayType>(static_cast<std::uint32_t>(length));
    for (std::uint64_t i = 0; i < length; ++i) (*chars)[static_cast<::jxx::lang::jint>(i)] = readChar();
    auto value = jxx::NEW<::jxx::lang::String>(chars);
    if (!handles_.emplace(handle, value).second) throw StreamCorruptedException("Duplicate object handle");
    return value;
}
::jxx::Ptr<::jxx::lang::Object> ObjectInputStream::readObject0_() {
    const auto tag = static_cast<Tag>(readUnsignedByte());
    if (tag == Tag::Null) return nullptr;
    if (tag == Tag::Reset) { handles_.clear(); return readObject0_(); }
    if (tag == Tag::Reference) {
        const auto found = handles_.find(readHandle_());
        if (found == handles_.end()) throw StreamCorruptedException("Unknown object handle");
        return found->second;
    }
    if (tag == Tag::NewString) return readString_(readHandle_());
    if (tag == Tag::NewArray) return readArray_(readHandle_());
    if (tag != Tag::NewObject) throw StreamCorruptedException("Unsupported JXX object stream tag");
    const auto handle = readHandle_();
    const auto className = jxx::NEW<::jxx::lang::String>(readRawUtf8_());
    const auto serialVersionUID = readLong();
    if (serialVersionUID != 1) throw InvalidClassException(className, jxx::NEW<::jxx::lang::String>("Unsupported JXX serialVersionUID"));
    const auto runtimeClass = ::jxx::lang::ClassAny::forName(className);
    auto object = runtimeClass->newInstance();
    auto serializable = std::dynamic_pointer_cast<SerializableI>(object);
    if (serializable == nullptr) throw InvalidClassException(className, jxx::NEW<::jxx::lang::String>("Class does not implement SerializableI"));
    if (!handles_.emplace(handle, object).second) throw StreamCorruptedException("Duplicate object handle");
    serializable->readObject(jxx::CAST<ObjectInputStream>(thisPtr()));
    if (static_cast<Tag>(readUnsignedByte()) != Tag::EndCustomData ||
        static_cast<Tag>(readUnsignedByte()) != Tag::EndObject)
        throw StreamCorruptedException("Missing JXX object terminator");
    return object;
}
::jxx::Ptr<::jxx::lang::Object> ObjectInputStream::readObject() {
    return synchronized([&]() -> ::jxx::Ptr<::jxx::lang::Object> { return readObject0_(); });
}
::jxx::Ptr<::jxx::lang::Object> ObjectInputStream::readUnshared() {
    return synchronized([&]() -> ::jxx::Ptr<::jxx::lang::Object> {
        auto object = readObject0_();
        if (object != nullptr) {
            for (auto iterator = handles_.begin(); iterator != handles_.end(); ++iterator)
                if (iterator->second == object) { handles_.erase(iterator); break; }
        }
        return object;
    });
}
} // namespace jxx::io
