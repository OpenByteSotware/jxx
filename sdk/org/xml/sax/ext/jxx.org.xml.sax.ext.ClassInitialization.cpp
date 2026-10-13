#include "org/xml/sax/ext/jxx.org.xml.sax.ext.ClassInitialization.h"
#include <mutex>
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Attributes2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Attributes2Impl.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.DeclHandler.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.DefaultHandler2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.EntityResolver2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.LexicalHandler.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Locator2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Locator2Impl.h"
namespace jxx::org::xml::sax::ext {
void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::InterfaceBase<Attributes2, ::jxx::org::xml::sax::Attributes>::Class();
        (void)::jxx::lang::ClassBase<Attributes2Impl, ::jxx::org::xml::sax::helpers::AttributesImpl, Attributes2>::Class();
        (void)::jxx::lang::InterfaceBase<DeclHandler>::Class();
        (void)::jxx::lang::InterfaceBase<EntityResolver2, ::jxx::org::xml::sax::EntityResolver>::Class();
        (void)::jxx::lang::InterfaceBase<LexicalHandler>::Class();
        (void)::jxx::lang::InterfaceBase<Locator2, ::jxx::org::xml::sax::Locator>::Class();
        (void)::jxx::lang::ClassBase<DefaultHandler2, ::jxx::org::xml::sax::helpers::DefaultHandler, DeclHandler, EntityResolver2, LexicalHandler>::Class();
        (void)::jxx::lang::ClassBase<Locator2Impl, ::jxx::org::xml::sax::helpers::LocatorImpl, Locator2>::Class();
    });
}
}
