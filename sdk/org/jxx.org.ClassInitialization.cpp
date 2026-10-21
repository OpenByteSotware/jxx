#include "org/jxx.org.ClassInitialization.h"
#include "org/w3c/jxx.org.w3c.ClassInitialization.h"
#include "org/xml/jxx.org.xml.ClassInitialization.h"

namespace jxx::org
{
void initialize()
{
    ::jxx::org::w3c::initialize();
    ::jxx::org::xml::initialize();
}

} // namespace jxx::org
