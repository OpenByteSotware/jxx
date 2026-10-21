#include "ext/xml/xpath/jxx.ext.xml.xpath.ClassInitialization.h"

#include <mutex>

#include "ext/xml/xpath/jxx.ext.xml.xpath.XPath.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathConstants.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpression.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpressionException.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathFactory.h"

#include "ext/xml/xpath/internal/jxx.ext.xml.xpath.internal.ClassInitialization.h"
namespace jxx::ext::xml::xpath {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::InterfaceBase<XPath>::Class();
        (void)::jxx::lang::ClassBase<
            XPathConstants,
            ::jxx::lang::Object>::Class();
        (void)::jxx::lang::InterfaceBase<XPathExpression>::Class();
        (void)::jxx::lang::ClassBase<
            XPathExpressionException,
            ::jxx::lang::Exception>::Class();
        (void)::jxx::lang::ClassBase<
            XPathFactory,
            ::jxx::lang::Object>::Class();
    });
}


void initialize()
{
    initializeClasses();
    ::jxx::ext::xml::xpath::internal::initialize();
}

} // namespace jxx::ext::xml::xpath
