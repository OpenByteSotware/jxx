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
    ::jxx::Ptr<::jxx::lang::Object> readObject() override;
    virtual ::jxx::Ptr<::jxx::lang::Object> readUnshared();
protected:
    ObjectInputStream();
private:
    using Handle = std::uint64_t;
    void readStreamHeader_();
    ::jxx::Ptr<::jxx::lang::Object> readObject0_();
    ::jxx::Ptr<::jxx::lang::String> readString_(Handle handle);
    std::string readRawUtf8_();
    Handle readHandle_();
    std::unordered_map<Handle, ::jxx::Ptr<::jxx::lang::Object>> handles_;
};
} // namespace jxx::io
