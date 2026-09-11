#pragma once

#include "ext/xml/parsers/jxx.ext.xml.parsers.SAXParser.h"

namespace jxx::ext::xml::parsers::internal {

class PugiSAXParser final
    : public ::jxx::ext::xml::parsers::SAXParser {
public:
    PugiSAXParser(
        ::jxx::lang::jbool namespaceAware,
        ::jxx::lang::jbool validating,
        const ::jxx::Ptr<::jxx::ext::xml::validation::Schema>& schema,
        ::jxx::lang::jbool xIncludeAware);

    ~PugiSAXParser() override;

    ::jxx::Ptr<::jxx::org::xml::sax::Parser> getParser() override;
    ::jxx::Ptr<::jxx::org::xml::sax::XMLReader> getXMLReader() override;
    ::jxx::lang::jbool isNamespaceAware() const override;
    ::jxx::lang::jbool isValidating() const override;

    void setProperty(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;

    ::jxx::Ptr<::jxx::lang::Object> getProperty(
        const ::jxx::Ptr<::jxx::lang::String>& name) const override;

    ::jxx::Ptr<::jxx::ext::xml::validation::Schema>
    getSchema() const override;

    ::jxx::lang::jbool isXIncludeAware() const override;
    void reset() override;

private:
    ::jxx::Ptr<::jxx::org::xml::sax::XMLReader> reader_;
    ::jxx::lang::jbool namespaceAware_;
    ::jxx::lang::jbool validating_;
    ::jxx::Ptr<::jxx::ext::xml::validation::Schema> schema_;
    ::jxx::lang::jbool xIncludeAware_;
};

} // namespace jxx::ext::xml::parsers::internal
