#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"

#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilderFactory.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.FactoryConfigurationError.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::parsers {

DocumentBuilderFactory::DocumentBuilderFactory() = default;
DocumentBuilderFactory::~DocumentBuilderFactory() = default;

::jxx::Ptr<DocumentBuilderFactory> DocumentBuilderFactory::newInstance() {
    return ::jxx::NEW<internal::PugiDocumentBuilderFactory>();
}

::jxx::Ptr<DocumentBuilderFactory> DocumentBuilderFactory::newInstance(
    const ::jxx::Ptr<::jxx::lang::String>& factoryClassName,
    const ::jxx::Ptr<::jxx::lang::ClassLoader>& classLoader) {
    (void)classLoader;
    if (factoryClassName == nullptr || factoryClassName->length() == 0) {
        return newInstance();
    }
    throw FactoryConfigurationError(
        ::jxx::NEW<::jxx::lang::String>(
            "Custom factory providers are not supported"));
}

void DocumentBuilderFactory::setNamespaceAware(::jxx::lang::jbool value) { namespaceAware_ = value; }
::jxx::lang::jbool DocumentBuilderFactory::isNamespaceAware() const { return namespaceAware_; }
void DocumentBuilderFactory::setValidating(::jxx::lang::jbool value) { validating_ = value; }
::jxx::lang::jbool DocumentBuilderFactory::isValidating() const { return validating_; }
void DocumentBuilderFactory::setIgnoringElementContentWhitespace(::jxx::lang::jbool value) { ignoreWhitespace_ = value; }
::jxx::lang::jbool DocumentBuilderFactory::isIgnoringElementContentWhitespace() const { return ignoreWhitespace_; }
void DocumentBuilderFactory::setExpandEntityReferences(::jxx::lang::jbool value) { expandEntities_ = value; }
::jxx::lang::jbool DocumentBuilderFactory::isExpandEntityReferences() const { return expandEntities_; }
void DocumentBuilderFactory::setIgnoringComments(::jxx::lang::jbool value) { ignoreComments_ = value; }
::jxx::lang::jbool DocumentBuilderFactory::isIgnoringComments() const { return ignoreComments_; }
void DocumentBuilderFactory::setCoalescing(::jxx::lang::jbool value) { coalescing_ = value; }
::jxx::lang::jbool DocumentBuilderFactory::isCoalescing() const { return coalescing_; }
void DocumentBuilderFactory::setSchema(const ::jxx::Ptr<::jxx::ext::xml::validation::Schema>& schema) { schema_ = schema; }
::jxx::Ptr<::jxx::ext::xml::validation::Schema> DocumentBuilderFactory::getSchema() const { return schema_; }
void DocumentBuilderFactory::setXIncludeAware(::jxx::lang::jbool value) { xinclude_ = value; }
::jxx::lang::jbool DocumentBuilderFactory::isXIncludeAware() const { return xinclude_; }

} // namespace jxx::ext::xml::parsers
