#include "org/xml/sax/internal/jxx.org.xml.sax.internal.ClassInitialization.h"

#include <mutex>

#include "org/xml/sax/internal/jxx.org.xml.sax.internal.PugiXMLReader.h"

namespace jxx::org::xml::sax::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            PugiXMLReader,
            ::jxx::lang::Object,
            ::jxx::org::xml::sax::XMLReader>::Class();
    });
}

} // namespace jxx::org::xml::sax::internal
