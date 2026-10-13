#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.ClassInitialization.h"

#include <mutex>

#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilder.h"
#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilderFactory.h"
#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiSAXParser.h"

namespace jxx::ext::xml::parsers::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            PugiDocumentBuilder,
            ::jxx::ext::xml::parsers::DocumentBuilder>::Class();
        (void)::jxx::lang::ClassBase<
            PugiDocumentBuilderFactory,
            ::jxx::ext::xml::parsers::DocumentBuilderFactory>::Class();
        (void)::jxx::lang::ClassBase<
            PugiSAXParser,
            ::jxx::ext::xml::parsers::SAXParser>::Class();
    });
}

} // namespace jxx::ext::xml::parsers::internal
