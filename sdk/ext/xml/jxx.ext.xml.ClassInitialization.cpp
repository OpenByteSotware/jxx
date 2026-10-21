#include "ext/xml/jxx.ext.xml.ClassInitialization.h"
#include "ext/xml/bind/jxx.ext.xml.bind.ClassInitialization.h"
#include "ext/xml/namespace/jxx.ext.xml.namespace.ClassInitialization.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.ClassInitialization.h"
#include "ext/xml/transform/jxx.ext.xml.transform.ClassInitialization.h"
#include "ext/xml/validation/jxx.ext.xml.validation.ClassInitialization.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.ClassInitialization.h"

namespace jxx::ext::xml
{
void initialize()
{
    ::jxx::ext::xml::bind::initialize();
    ::jxx::ext::xml::namespace_::initialize();
    ::jxx::ext::xml::parsers::initialize();
    ::jxx::ext::xml::transform::initialize();
    ::jxx::ext::xml::validation::initialize();
    ::jxx::ext::xml::xpath::initialize();
}

} // namespace jxx::ext::xml
