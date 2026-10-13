#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.ClassInitialization.h"

#include <mutex>

#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamResult.h"

namespace jxx::ext::xml::transform::stream {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            StreamResult,
            ::jxx::lang::Object,
            ::jxx::ext::xml::transform::Result>::Class();
    });
}

} // namespace jxx::ext::xml::transform::stream
