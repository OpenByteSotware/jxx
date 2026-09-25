#include "ext/xml/transform/jxx.ext.xml.transform.TransformerFactory.h"
#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.IdentityTransformerFactory.h"
namespace jxx::ext::xml::transform {
::jxx::Ptr<TransformerFactory> TransformerFactory::newInstance(){return ::jxx::NEW<::jxx::ext::xml::transform::internal::IdentityTransformerFactory>();}
}
