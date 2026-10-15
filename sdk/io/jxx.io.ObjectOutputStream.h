#pragma once

#include <cstdint>
#include <unordered_map>

#include "io/jxx.io.DataOutputStream.h"
#include "io/jxx.io.ObjectOutput.h"

namespace jxx::lang { class Object; class String; }

namespace jxx::io {
class ObjectOutputStream : public ::jxx::lang::ClassBase<ObjectOutputStream, DataOutputStream, ObjectOutput> {
public:
    using JxxSuper = DataOutputStream;
    using Super = ::jxx::lang::ClassBase<ObjectOutputStream, JxxSuper, ObjectOutput>;
    explicit ObjectOutputStream(const ::jxx::Ptr<OutputStream>& output);
    ~ObjectOutputStream() override;
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
};
} // namespace jxx::io
