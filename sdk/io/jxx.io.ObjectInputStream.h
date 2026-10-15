#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include "io/jxx.io.DataInputStream.h"
#include "io/jxx.io.ObjectInput.h"

namespace jxx::lang { class Object; class String; }
namespace jxx::io {
class ObjectInputStream : public ::jxx::lang::ClassBase<ObjectInputStream, DataInputStream, ObjectInput> {
public:
    using JxxSuper = DataInputStream;
    using Super = ::jxx::lang::ClassBase<ObjectInputStream, JxxSuper, ObjectInput>;
    explicit ObjectInputStream(const ::jxx::Ptr<InputStream>& input);
    ~ObjectInputStream() override;
    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer) override;
    ::jxx::lang::jint read(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;
    ::jxx::lang::jlong skip(::jxx::lang::jlong count) override;
    ::jxx::lang::jint available() override;
    ::jxx::Ptr<::jxx::lang::Object> readObject() override;
    virtual ::jxx::Ptr<::jxx::lang::Object> readUnshared();
protected:
    ObjectInputStream();
private:
    using Handle = std::uint64_t;
    struct ClassDescriptor { std::string binaryName; ::jxx::lang::jlong serialVersionUID; };
    void readStreamHeader_();
    ::jxx::Ptr<::jxx::lang::Object> readObject0_();
    ::jxx::Ptr<::jxx::lang::String> readString_(Handle handle);
    ::jxx::Ptr<::jxx::lang::Object> readArray_(Handle handle);
    void registerHandle_(Handle handle, const ::jxx::Ptr<::jxx::lang::Object>& object);
    void requireEndArray_();
    std::string readRawUtf8_();
    Handle readHandle_();
    ClassDescriptor readClassDescriptor_();
    std::unordered_map<Handle, ::jxx::Ptr<::jxx::lang::Object>> handles_;
    std::unordered_map<Handle, ClassDescriptor> descriptors_;
};
} // namespace jxx::io
