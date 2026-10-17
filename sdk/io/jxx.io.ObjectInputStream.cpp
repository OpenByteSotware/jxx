#include "io/jxx.io.ObjectInputStream.h"

#include "io/jxx.io.InvalidClassException.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "io/jxx.io.ObjectStreamFormat.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.StreamCorruptedException.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.TimeZone.h"

namespace jxx::io {
namespace { using Tag = serialization::Tag; }
ObjectInputStream::ObjectInputStream(const ::jxx::Ptr<InputStream>& input) : Super(input) { ::jxx::initialize(); readStreamHeader_(); }
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
void ObjectInputStream::setLimits(const ObjectInputLimits& limits) {
    if (limits.maxObjects == 0U || limits.maxStringCodeUnits == 0U ||
        limits.maxArrayElements == 0U || limits.maxGraphDepth == 0U ||
        limits.maxClassNameBytes == 0U || limits.maxClassDescriptors == 0U)
        throw ::jxx::lang::IllegalArgumentException("Object input limits must be positive");
    limits_ = limits;
}
const ObjectInputLimits& ObjectInputStream::getLimits() const noexcept { return limits_; }
void ObjectInputStream::setClassFilter(ObjectInputClassFilter filter) {
    if (!filter) {
        throw ::jxx::lang::IllegalArgumentException(
            "Object input class filter must not be empty");
    }
    classFilter_ = std::move(filter);
}
void ObjectInputStream::clearClassFilter() { classFilter_ = {}; }
bool ObjectInputStream::hasClassFilter() const noexcept {
    return static_cast<bool>(classFilter_);
}

void ObjectInputStream::validateNewHandle_(Handle handle) {
    if (handles_.find(handle) != handles_.end())
        throw StreamCorruptedException("Duplicate object handle");
    if (++objectCount_ > limits_.maxObjects)
        throw StreamCorruptedException("JXX object count exceeds limit");
}
void ObjectInputStream::enterGraph_() {
    if (++graphDepth_ > limits_.maxGraphDepth) {
        --graphDepth_;
        throw StreamCorruptedException("JXX object graph depth exceeds limit");
    }
}
void ObjectInputStream::leaveGraph_() noexcept { if (graphDepth_ != 0U) --graphDepth_; }
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
std::uint64_t ObjectInputStream::readLength_() {
    const auto value = readLong();
    if (value < 0) throw StreamCorruptedException("Invalid serialized length");
    return static_cast<std::uint64_t>(value);
}
ObjectInputStream::ClassDescriptor ObjectInputStream::readClassDescriptor_() {
    const auto tag = static_cast<Tag>(readUnsignedByte());
    if (tag == Tag::ClassDescriptorRef) {
        const auto found = descriptors_.find(readHandle_());
        if (found == descriptors_.end())
            throw StreamCorruptedException("Unknown class descriptor handle");
        return found->second;
    }
    if (tag != Tag::ClassDescriptor)
        throw StreamCorruptedException("Missing JXX class descriptor");
    const auto handle = readHandle_();
    ClassDescriptor descriptor{readRawUtf8_(), readLong()};
    const auto flags = readUnsignedShort();
    const auto fieldCount = readInt();
    if ((flags & 0x0001) == 0)
        throw StreamCorruptedException("Class descriptor is not serializable");
    if (fieldCount != 0)
        throw StreamCorruptedException("JXX v1 reflected fields are unsupported");
    if (static_cast<Tag>(readUnsignedByte()) != Tag::EndClassDescriptor)
        throw StreamCorruptedException("Missing JXX class descriptor terminator");
    if (descriptors_.size() >= limits_.maxClassDescriptors)
        throw StreamCorruptedException("JXX class descriptor count exceeds limit");
    if (!descriptors_.emplace(handle, descriptor).second)
        throw StreamCorruptedException("Duplicate class descriptor handle");
    return descriptor;
}
std::string ObjectInputStream::readRawUtf8_() {
    const auto length = readInt();
    if (length < 0 || static_cast<std::uint64_t>(length) > limits_.maxClassNameBytes) throw StreamCorruptedException("Invalid metadata length");
    std::string value; value.reserve(static_cast<std::size_t>(length));
    for (::jxx::lang::jint i = 0; i < length; ++i) value.push_back(static_cast<char>(readUnsignedByte()));
    return value;
}
void ObjectInputStream::registerHandle_(
    Handle handle, const ::jxx::Ptr<::jxx::lang::Object>& object) {
    validateNewHandle_(handle);
    handles_.emplace(handle, object);
}
void ObjectInputStream::requireEndArray_() {
    if (static_cast<Tag>(readUnsignedByte()) != Tag::EndArray)
        throw StreamCorruptedException("Missing JXX array terminator");
}
::jxx::Ptr<::jxx::lang::Object> ObjectInputStream::readArray_(Handle handle) {
    using Kind = serialization::ArrayElementKind;
    const auto kind = static_cast<Kind>(readUnsignedByte());
    const auto rank = readUnsignedByte();
    if (rank == 0)
        throw StreamCorruptedException("Invalid JXX array rank");
    const auto rawLength = readLength_();
    if (rawLength > limits_.maxArrayElements) throw StreamCorruptedException("Array length exceeds limit");
    const auto length = static_cast<::jxx::lang::jint>(rawLength);
    if (kind == Kind::TypedObject) {
        const auto componentName = readRawUtf8_();
        if (classFilter_ && !classFilter_(componentName))
            throw InvalidClassException(jxx::NEW<::jxx::lang::String>(componentName),
                jxx::NEW<::jxx::lang::String>("Array component rejected by JXX object input class filter"));
        const auto* codec = serialization::ArrayCodecRegistry::findByWireType(componentName, rank);
        if (codec == nullptr) throw InvalidClassException(jxx::NEW<::jxx::lang::String>(componentName), jxx::NEW<::jxx::lang::String>("No typed reference-array codec is registered"));
        auto array = codec->create(length);
        registerHandle_(handle, array);
        for (::jxx::lang::jint i = 0; i < length; ++i) codec->set(array, i, readObject0_());
        requireEndArray_();
        return array;
    }
    if (rank != 1 && rank != 2)
        throw StreamCorruptedException("Unsupported built-in JXX array rank");
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
    const auto length = readLength_();
    if (length > limits_.maxStringCodeUnits) throw StreamCorruptedException("String length exceeds limit");
    auto chars = jxx::NEW<::jxx::lang::CharArrayType>(static_cast<std::uint32_t>(length));
    for (std::uint64_t i = 0; i < length; ++i) (*chars)[static_cast<::jxx::lang::jint>(i)] = readChar();
    auto value = jxx::NEW<::jxx::lang::String>(chars);
    registerHandle_(handle, value);
    return value;
}
::jxx::Ptr<::jxx::lang::Object> ObjectInputStream::readObject0_() {
    enterGraph_();
    struct GraphExit final { ObjectInputStream* stream; ~GraphExit() { stream->leaveGraph_(); } } graphExit{this};
    const auto tag = static_cast<Tag>(readUnsignedByte());
    if (tag == Tag::Null) return nullptr;
    if (tag == Tag::Reset) { handles_.clear(); descriptors_.clear(); objectCount_ = 0U; return readObject0_(); }
    if (tag == Tag::Reference) {
        const auto found = handles_.find(readHandle_());
        if (found == handles_.end()) throw StreamCorruptedException("Unknown object handle");
        return found->second;
    }
    if (tag == Tag::NewClass) {
        const auto handle = readHandle_();
        const auto binaryName = readRawUtf8_();
        if (classFilter_ && !classFilter_(binaryName))
            throw InvalidClassException(jxx::NEW<::jxx::lang::String>(binaryName),
                jxx::NEW<::jxx::lang::String>("Rejected by JXX object input class filter"));
        auto type = ::jxx::lang::ClassAny::forName(jxx::NEW<::jxx::lang::String>(binaryName));
        registerHandle_(handle, type);
        return type;
    }
    if (tag == Tag::NewEnum) {
        const auto handle = readHandle_();
        const auto descriptor = readClassDescriptor_();
        if (classFilter_ && !classFilter_(descriptor.binaryName))
            throw InvalidClassException(jxx::NEW<::jxx::lang::String>(descriptor.binaryName),
                jxx::NEW<::jxx::lang::String>("Rejected by JXX object input class filter"));
        auto type = ::jxx::lang::ClassAny::forName(jxx::NEW<::jxx::lang::String>(descriptor.binaryName));
        if (!type->isEnum()) throw InvalidClassException(type->getName(), jxx::NEW<::jxx::lang::String>("Class descriptor is not an enum"));
        auto value = type->resolveEnumConstant(jxx::NEW<::jxx::lang::String>(readRawUtf8_()));
        registerHandle_(handle, value);
        return value;
    }
    if (tag == Tag::NewString) return readString_(readHandle_());
    if (tag == Tag::NewTimeZone) {
        const auto handle = readHandle_();
        const auto idObject = readObject0_();
        const auto id = std::dynamic_pointer_cast<::jxx::lang::String>(idObject);
        if (id == nullptr)
            throw StreamCorruptedException("TimeZone identifier is not a String");
        auto value = ::jxx::util::TimeZone::getTimeZone(id);
        registerHandle_(handle, value);
        return value;
    }
    if (tag == Tag::NewArray) return readArray_(readHandle_());
    if (tag != Tag::NewObject) throw StreamCorruptedException("Unsupported JXX object stream tag");
    const auto handle = readHandle_();
    const auto descriptor = readClassDescriptor_();
    const auto className = jxx::NEW<::jxx::lang::String>(descriptor.binaryName);
    if (classFilter_ && !classFilter_(descriptor.binaryName))
        throw InvalidClassException(className,
            jxx::NEW<::jxx::lang::String>("Rejected by JXX object input class filter"));
    const auto classMetadata = serialization::SerializationClassRegistry::resolve(
        descriptor.binaryName);
    if (descriptor.serialVersionUID != classMetadata.serialVersionUID)
        throw InvalidClassException(className,
            jxx::NEW<::jxx::lang::String>("JXX serialVersionUID mismatch"));
    const auto runtimeClass = ::jxx::lang::ClassAny::forName(className);
    auto object = classMetadata.deserializationFactory();
    if (object == nullptr || !runtimeClass->isInstance(object))
        throw InvalidClassException(className,
            jxx::NEW<::jxx::lang::String>("Deserialization factory returned an incompatible object"));
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
