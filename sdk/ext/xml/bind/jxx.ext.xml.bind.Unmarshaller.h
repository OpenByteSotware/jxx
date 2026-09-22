#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::io { class File; class InputStream; class Reader; }
namespace jxx::org::w3c::dom { class Node; }

namespace jxx::ext::xml::bind {

class Unmarshaller
    : public ::jxx::lang::InterfaceBase<Unmarshaller> {
public:
    ~Unmarshaller() override = default;

    virtual ::jxx::Ptr<::jxx::lang::Object> unmarshal(
        const ::jxx::Ptr<::jxx::io::File>& file) = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> unmarshal(
        const ::jxx::Ptr<::jxx::io::InputStream>& input) = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> unmarshal(
        const ::jxx::Ptr<::jxx::io::Reader>& reader) = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> unmarshal(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) = 0;
};

} // namespace jxx::ext::xml::bind
