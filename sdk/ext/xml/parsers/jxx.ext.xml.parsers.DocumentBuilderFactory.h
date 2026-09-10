#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::ext::xml::validation {
class Schema;
}

namespace jxx::lang {
class ClassLoader;
class Object;
class String;
}

namespace jxx::ext::xml::parsers {

class DocumentBuilder;

class DocumentBuilderFactory
    : public ::jxx::lang::ClassBase<
          DocumentBuilderFactory,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<DocumentBuilderFactory, JxxSuper>;

    virtual ~DocumentBuilderFactory();

    static ::jxx::Ptr<DocumentBuilderFactory> newInstance();
    static ::jxx::Ptr<DocumentBuilderFactory> newInstance(
        const ::jxx::Ptr<::jxx::lang::String>& factoryClassName,
        const ::jxx::Ptr<::jxx::lang::ClassLoader>& classLoader);

    virtual ::jxx::Ptr<DocumentBuilder> newDocumentBuilder() = 0;
    virtual void setAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> getAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& name) = 0;
    virtual void setFeature(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        ::jxx::lang::jbool value) = 0;
    virtual ::jxx::lang::jbool getFeature(
        const ::jxx::Ptr<::jxx::lang::String>& name) = 0;

    void setNamespaceAware(::jxx::lang::jbool value);
    ::jxx::lang::jbool isNamespaceAware() const;
    void setValidating(::jxx::lang::jbool value);
    ::jxx::lang::jbool isValidating() const;
    void setIgnoringElementContentWhitespace(::jxx::lang::jbool value);
    ::jxx::lang::jbool isIgnoringElementContentWhitespace() const;
    void setExpandEntityReferences(::jxx::lang::jbool value);
    ::jxx::lang::jbool isExpandEntityReferences() const;
    void setIgnoringComments(::jxx::lang::jbool value);
    ::jxx::lang::jbool isIgnoringComments() const;
    void setCoalescing(::jxx::lang::jbool value);
    ::jxx::lang::jbool isCoalescing() const;
    void setSchema(const ::jxx::Ptr<::jxx::ext::xml::validation::Schema>& schema);
    ::jxx::Ptr<::jxx::ext::xml::validation::Schema> getSchema() const;
    void setXIncludeAware(::jxx::lang::jbool value);
    ::jxx::lang::jbool isXIncludeAware() const;

protected:
    DocumentBuilderFactory();

    ::jxx::lang::jbool namespaceAware_ = false;
    ::jxx::lang::jbool validating_ = false;
    ::jxx::lang::jbool ignoreWhitespace_ = false;
    ::jxx::lang::jbool expandEntities_ = true;
    ::jxx::lang::jbool ignoreComments_ = false;
    ::jxx::lang::jbool coalescing_ = false;
    ::jxx::lang::jbool xinclude_ = false;
    ::jxx::Ptr<::jxx::ext::xml::validation::Schema> schema_;
};

} // namespace jxx::ext::xml::parsers
