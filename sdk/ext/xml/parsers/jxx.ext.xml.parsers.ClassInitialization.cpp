#include "ext/xml/parsers/jxx.ext.xml.parsers.ClassInitialization.h"

#include <mutex>

#include "ext/xml/parsers/jxx.ext.xml.parsers.FactoryConfigurationError.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.ParserConfigurationException.h"

namespace jxx::ext::xml::parsers {

void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)FactoryConfigurationError::Class();
        (void)ParserConfigurationException::Class();
    });
}

} // namespace jxx::ext::xml::parsers
