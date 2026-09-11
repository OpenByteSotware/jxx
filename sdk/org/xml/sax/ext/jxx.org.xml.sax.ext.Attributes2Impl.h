#pragma once

#include <vector>

#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Attributes2.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributesImpl.h"

namespace jxx::org::xml::sax::ext {

class Attributes2Impl
    : public ::jxx::lang::ClassBase<
          Attributes2Impl,
          ::jxx::org::xml::sax::helpers::AttributesImpl,
          Attributes2> {
public:
    using JxxSuper =
        ::jxx::org::xml::sax::helpers::AttributesImpl;

    using Super = ::jxx::lang::ClassBase<
        Attributes2Impl,
        JxxSuper,
        Attributes2>;

    Attributes2Impl();

    explicit Attributes2Impl(
        const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes);

    ~Attributes2Impl() override;

    ::jxx::lang::jbool isDeclared(
        ::jxx::lang::jint index) const override;

    ::jxx::lang::jbool isDeclared(
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) const override;

    ::jxx::lang::jbool isDeclared(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const override;

    ::jxx::lang::jbool isSpecified(
        ::jxx::lang::jint index) const override;

    ::jxx::lang::jbool isSpecified(
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) const override;

    ::jxx::lang::jbool isSpecified(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const override;

    void setAttributes(
        const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes);

    void addAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
        const ::jxx::Ptr<::jxx::lang::String>& type,
        const ::jxx::Ptr<::jxx::lang::String>& value);

    void removeAttribute(::jxx::lang::jint index);

    void setDeclared(
        ::jxx::lang::jint index,
        ::jxx::lang::jbool value);

    void setSpecified(
        ::jxx::lang::jint index,
        ::jxx::lang::jbool value);

private:
    void checkIndex_(::jxx::lang::jint index) const;
    ::jxx::lang::jint requireIndex_(
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) const;
    ::jxx::lang::jint requireIndex_(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const;

    std::vector<::jxx::lang::jbool> declared_;
    std::vector<::jxx::lang::jbool> specified_;
};

} // namespace jxx::org::xml::sax::ext
