#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::ext::xml::validation { class Schema; }
namespace jxx::io { class File; class InputStream; }
namespace jxx::lang { class Object; class String; }
namespace jxx::org::xml::sax { class InputSource; class Parser; class XMLReader; }
namespace jxx::org::xml::sax::helpers { class DefaultHandler; }

namespace jxx::ext::xml::parsers {

class SAXParser
    : public ::jxx::lang::ClassBase<SAXParser, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SAXParser, JxxSuper>;

    ~SAXParser() override;

    virtual ::jxx::Ptr<::jxx::org::xml::sax::Parser> getParser() = 0;
    virtual ::jxx::Ptr<::jxx::org::xml::sax::XMLReader> getXMLReader() = 0;
    virtual ::jxx::lang::jbool isNamespaceAware() const = 0;
    virtual ::jxx::lang::jbool isValidating() const = 0;
    virtual void setProperty(const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> getProperty(
        const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;
    virtual ::jxx::Ptr<::jxx::ext::xml::validation::Schema> getSchema() const;
    virtual ::jxx::lang::jbool isXIncludeAware() const;
    virtual void reset();

    void parse(const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input,
        const ::jxx::Ptr<::jxx::org::xml::sax::helpers::DefaultHandler>& handler);
    void parse(const ::jxx::Ptr<::jxx::io::InputStream>& input,
        const ::jxx::Ptr<::jxx::org::xml::sax::helpers::DefaultHandler>& handler);
    void parse(const ::jxx::Ptr<::jxx::io::InputStream>& input,
        const ::jxx::Ptr<::jxx::org::xml::sax::helpers::DefaultHandler>& handler,
        const ::jxx::Ptr<::jxx::lang::String>& systemId);
    void parse(const ::jxx::Ptr<::jxx::io::File>& file,
        const ::jxx::Ptr<::jxx::org::xml::sax::helpers::DefaultHandler>& handler);
    void parse(const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::org::xml::sax::helpers::DefaultHandler>& handler);

protected:
    SAXParser();
};

} // namespace jxx::ext::xml::parsers
