#pragma once

#include <cstdint>
#include <unordered_map>

#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.DataOutputStream.h"
#include "io/jxx.io.ObjectOutput.h"
#include "io/jxx.io.ObjectStreamConstants.h"

namespace jxx::lang { class Object; class String; }

namespace jxx::io {
class ObjectOutputStream : public ::jxx::lang::ClassBase<ObjectOutputStream, OutputStream, ObjectOutput, ObjectStreamConstants> {
public:
    using JxxSuper = OutputStream;
    using Super = ::jxx::lang::ClassBase<ObjectOutputStream, JxxSuper, ObjectOutput, ObjectStreamConstants>;
    explicit ObjectOutputStream(const ::jxx::Ptr<OutputStream>& output);
    ~ObjectOutputStream() override;
    void write(::jxx::lang::jint value) override;
    void write(const ::jxx::lang::ByteArray& buffer) override;
    void write(const ::jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset, ::jxx::lang::jint length) override;
    void flush() override;
    void close() override;
    void writeBoolean(::jxx::lang::jbool value) override;
    void writeByte(::jxx::lang::jint value) override;
    void writeShort(::jxx::lang::jint value) override;
    void writeChar(::jxx::lang::jint value) override;
    void writeInt(::jxx::lang::jint value) override;
    void writeLong(::jxx::lang::jlong value) override;
    void writeFloat(::jxx::lang::jfloat value) override;
    void writeDouble(::jxx::lang::jdouble value) override;
    void writeBytes(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    void writeChars(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    void writeUTF(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    void writeObject(const ::jxx::Ptr<::jxx::lang::Object>& object) override;
    virtual void writeUnshared(const ::jxx::Ptr<::jxx::lang::Object>& object);
    virtual void reset();
protected:
    ObjectOutputStream();
private:
    using Handle = std::uint64_t;
    void writeStreamHeader_();
    void writeObject0_(const ::jxx::Ptr<::jxx::lang::Object>& object);
    void writeString_(const ::jxx::Ptr<::jxx::lang::String>& value, Handle handle);
    void writeRawUtf8_(const std::string& value);
    void writeHandle_(Handle value);
    void writeClassDescriptor_(const std::string& binaryName, ::jxx::lang::jlong serialVersionUID);
    Handle assignHandle_(const ::jxx::Ptr<::jxx::lang::Object>& object);
    std::unordered_map<const ::jxx::lang::Object*, Handle> handles_;
    Handle nextHandle_ = 1U;
    std::unordered_map<std::string, Handle> descriptorHandles_;
    Handle nextDescriptorHandle_ = 1U;
    ::jxx::Ptr<DataOutputStream> dataOutput_;
};
} // namespace jxx::io
