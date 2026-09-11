#pragma once

#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "org/xml/sax/jxx.org.xml.sax.AttributeList.h"

namespace jxx::org::xml::sax::helpers {

class AttributeListImpl
    : public ::jxx::lang::ClassBase<
          AttributeListImpl,
          ::jxx::lang::Object,
          ::jxx::org::xml::sax::AttributeList> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        AttributeListImpl,
        JxxSuper,
        ::jxx::org::xml::sax::AttributeList>;

    AttributeListImpl();

    explicit AttributeListImpl(
        const ::jxx::Ptr<::jxx::org::xml::sax::AttributeList>& attributes);

    ~AttributeListImpl() override;

    ::jxx::lang::jint getLength() const override;

    ::jxx::Ptr<::jxx::lang::String> getName(
        ::jxx::lang::jint index) const override;

    ::jxx::Ptr<::jxx::lang::String> getType(
        ::jxx::lang::jint index) const override;

    ::jxx::Ptr<::jxx::lang::String> getValue(
        ::jxx::lang::jint index) const override;

    ::jxx::Ptr<::jxx::lang::String> getType(
        const ::jxx::Ptr<::jxx::lang::String>& name) const override;

    ::jxx::Ptr<::jxx::lang::String> getValue(
        const ::jxx::Ptr<::jxx::lang::String>& name) const override;

    void setAttributeList(
        const ::jxx::Ptr<::jxx::org::xml::sax::AttributeList>& attributes);

    void addAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& type,
        const ::jxx::Ptr<::jxx::lang::String>& value);

    void removeAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& name);

    void clear();

private:
    struct Entry {
        ::jxx::Ptr<::jxx::lang::String> name;
        ::jxx::Ptr<::jxx::lang::String> type;
        ::jxx::Ptr<::jxx::lang::String> value;
    };

    const Entry* entryAt_(::jxx::lang::jint index) const noexcept;
    std::vector<Entry> entries_;
};

} // namespace jxx::org::xml::sax::helpers
