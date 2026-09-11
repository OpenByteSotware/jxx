#pragma once

#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
#include "org/xml/sax/jxx.org.xml.sax.Attributes.h"

namespace jxx::org::xml::sax::helpers {

class AttributesImpl
    : public ::jxx::lang::ClassBase<
          AttributesImpl,
          ::jxx::lang::Object,
          ::jxx::org::xml::sax::Attributes> {
public:
    using JxxSuper = ::jxx::lang::Object;

    using Super = ::jxx::lang::ClassBase<
        AttributesImpl,
        JxxSuper,
        ::jxx::org::xml::sax::Attributes>;

    AttributesImpl();

    explicit AttributesImpl(
        const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes);

    ~AttributesImpl() override;

    ::jxx::lang::jint getLength() const override;

    ::jxx::Ptr<::jxx::lang::String> getURI(
        ::jxx::lang::jint index) const override;

    ::jxx::Ptr<::jxx::lang::String> getLocalName(
        ::jxx::lang::jint index) const override;

    ::jxx::Ptr<::jxx::lang::String> getQName(
        ::jxx::lang::jint index) const override;

    ::jxx::Ptr<::jxx::lang::String> getType(
        ::jxx::lang::jint index) const override;

    ::jxx::Ptr<::jxx::lang::String> getValue(
        ::jxx::lang::jint index) const override;

    ::jxx::lang::jint getIndex(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const override;

    ::jxx::lang::jint getIndex(
        const ::jxx::Ptr<::jxx::lang::String>& qName) const override;

    ::jxx::Ptr<::jxx::lang::String> getType(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const override;

    ::jxx::Ptr<::jxx::lang::String> getType(
        const ::jxx::Ptr<::jxx::lang::String>& qName) const override;

    ::jxx::Ptr<::jxx::lang::String> getValue(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const override;

    ::jxx::Ptr<::jxx::lang::String> getValue(
        const ::jxx::Ptr<::jxx::lang::String>& qName) const override;

    void clear();

    void setAttributes(
        const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes);

    void addAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qName,
        const ::jxx::Ptr<::jxx::lang::String>& type,
        const ::jxx::Ptr<::jxx::lang::String>& value);

    void setAttribute(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qName,
        const ::jxx::Ptr<::jxx::lang::String>& type,
        const ::jxx::Ptr<::jxx::lang::String>& value);

    void removeAttribute(
        ::jxx::lang::jint index);

    void setURI(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<::jxx::lang::String>& uri);

    void setLocalName(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<::jxx::lang::String>& localName);

    void setQName(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<::jxx::lang::String>& qName);

    void setType(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<::jxx::lang::String>& type);

    void setValue(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<::jxx::lang::String>& value);

private:
    struct Entry {
        ::jxx::Ptr<::jxx::lang::String> uri;
        ::jxx::Ptr<::jxx::lang::String> localName;
        ::jxx::Ptr<::jxx::lang::String> qName;
        ::jxx::Ptr<::jxx::lang::String> type;
        ::jxx::Ptr<::jxx::lang::String> value;
    };

    Entry& entryAt_(::jxx::lang::jint index);
    const Entry* entryAtOrNull_(::jxx::lang::jint index) const noexcept;

    std::vector<Entry> entries_;
};

} // namespace jxx::org::xml::sax::helpers
