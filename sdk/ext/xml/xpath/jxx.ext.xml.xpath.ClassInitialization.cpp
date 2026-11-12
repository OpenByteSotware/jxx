#include "ext/xml/xpath/jxx.ext.xml.xpath.ClassInitialization.h"

#include <mutex>

#include "ext/xml/xpath/jxx.ext.xml.xpath.XPath.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathConstants.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpression.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathException.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpressionException.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathFactory.h"

#include "ext/xml/xpath/internal/jxx.ext.xml.xpath.internal.ClassInitialization.h"
namespace jxx::ext::xml::xpath {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
            ::jxx::lang::class_info_detail::registerClassInfo<XPath>();
            ::jxx::lang::class_info_detail::registerClassInfo<XPathConstants>();
            ::jxx::lang::class_info_detail::registerClassInfo<XPathExpression>();
            ::jxx::lang::class_info_detail::registerClassInfo<XPathFactory>();
        (void)::jxx::lang::InterfaceBase<XPath>::Class();
        (void)::jxx::lang::ClassBase<
            XPathConstants,
            ::jxx::lang::Object>::Class();
        (void)::jxx::lang::InterfaceBase<XPathExpression>::Class();
        ::jxx::lang::class_info_detail::registerClassInfo<XPathException>();
        ::jxx::lang::class_info_detail::registerClassInfo<XPathExpressionException>();
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
