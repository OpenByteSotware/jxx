#pragma once
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::security::cert {
class Certificate : public ::jxx::lang::ClassBase<Certificate, ::jxx::lang::Object, ::jxx::io::SerializableI> {
public:
    explicit Certificate(const ::jxx::Ptr<::jxx::lang::String>& type);
    ~Certificate() override = default;
    ::jxx::Ptr<::jxx::lang::String> getType() const;
    virtual ::jxx::lang::ByteArray getEncoded() const = 0;
private:
    ::jxx::Ptr<::jxx::lang::String> type_;
};
}
