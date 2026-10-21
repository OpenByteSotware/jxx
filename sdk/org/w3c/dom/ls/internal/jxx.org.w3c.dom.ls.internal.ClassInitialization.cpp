#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.ClassInitialization.h"
#include <mutex>
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMConfigurationImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSInputImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSOutputImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSParserImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSSerializerImpl.h"
namespace jxx::org::w3c::dom::ls::internal {
void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<DOMConfigurationImpl, ::jxx::lang::Object, ::jxx::org::w3c::dom::DOMConfiguration>::Class();
        (void)::jxx::lang::ClassBase<DOMImplementationLSImpl, ::jxx::lang::Object, ::jxx::org::w3c::dom::ls::DOMImplementationLS>::Class();
        (void)::jxx::lang::ClassBase<LSInputImpl, ::jxx::lang::Object, ::jxx::org::w3c::dom::ls::LSInput>::Class();
        (void)::jxx::lang::ClassBase<LSOutputImpl, ::jxx::lang::Object, ::jxx::org::w3c::dom::ls::LSOutput>::Class();
        (void)::jxx::lang::ClassBase<LSParserImpl, ::jxx::lang::Object, ::jxx::org::w3c::dom::ls::LSParser>::Class();
        (void)::jxx::lang::ClassBase<LSSerializerImpl, ::jxx::lang::Object, ::jxx::org::w3c::dom::ls::LSSerializer>::Class();
    });
}

void initialize()
{
    initializeClasses();
}

}
