#include <mutex>
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.ClassInitialization.h"
#include "lang/jxx.lang.ClassInfo.h"

#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.DOMImplementationLS.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSInput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSLoadEvent.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSOutput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParser.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParserFilter.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSProgressEvent.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSResourceResolver.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializerFilter.h"

namespace jxx::org::w3c::dom::ls {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::org::w3c::dom::ls::LSException::JxxClassInfoMarker::Class();
        (void)DOMImplementationLS::JxxClassInfoMarker::Class();
        (void)LSInput::JxxClassInfoMarker::Class();
        (void)LSLoadEvent::JxxClassInfoMarker::Class();
        (void)LSOutput::JxxClassInfoMarker::Class();
        (void)LSParser::JxxClassInfoMarker::Class();
        (void)LSParserFilter::JxxClassInfoMarker::Class();
        (void)LSProgressEvent::JxxClassInfoMarker::Class();
        (void)LSResourceResolver::JxxClassInfoMarker::Class();
        (void)LSSerializer::JxxClassInfoMarker::Class();
        (void)LSSerializerFilter::JxxClassInfoMarker::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::org::w3c::dom::ls
