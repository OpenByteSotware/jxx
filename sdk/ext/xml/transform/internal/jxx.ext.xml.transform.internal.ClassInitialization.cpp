#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.ClassInitialization.h"

#include <mutex>

#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.IdentityTransformer.h"
#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.IdentityTransformerFactory.h"

namespace jxx::ext::xml::transform::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            IdentityTransformer,
            ::jxx::ext::xml::transform::Transformer>::Class();
        (void)::jxx::lang::ClassBase<
            IdentityTransformerFactory,
            ::jxx::ext::xml::transform::TransformerFactory>::Class();
    });
}

} // namespace jxx::ext::xml::transform::internal
