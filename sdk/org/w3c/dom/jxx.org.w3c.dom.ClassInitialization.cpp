#include <mutex>
#include "org/w3c/dom/jxx.org.w3c.dom.ClassInitialization.h"


#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Attr.h"
#include "org/w3c/dom/jxx.org.w3c.dom.CDATASection.h"
#include "org/w3c/dom/jxx.org.w3c.dom.CharacterData.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Comment.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DocumentFragment.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DocumentType.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMError.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMErrorHandler.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementation.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationSource.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMLocator.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMStringList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Entity.h"
#include "org/w3c/dom/jxx.org.w3c.dom.EntityReference.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NamedNodeMap.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NameList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Notation.h"
#include "org/w3c/dom/jxx.org.w3c.dom.ProcessingInstruction.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"
#include "org/w3c/dom/jxx.org.w3c.dom.TypeInfo.h"
#include "org/w3c/dom/jxx.org.w3c.dom.UserDataHandler.h"

namespace jxx::org::w3c::dom {

void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)DOMException::Class();
        (void)Attr::JxxClassInfoMarker::Class();
        (void)CDATASection::JxxClassInfoMarker::Class();
        (void)CharacterData::JxxClassInfoMarker::Class();
        (void)Comment::JxxClassInfoMarker::Class();
        (void)Document::JxxClassInfoMarker::Class();
        (void)DocumentFragment::JxxClassInfoMarker::Class();
        (void)DocumentType::JxxClassInfoMarker::Class();
        (void)DOMConfiguration::JxxClassInfoMarker::Class();
        (void)DOMError::JxxClassInfoMarker::Class();
        (void)DOMErrorHandler::JxxClassInfoMarker::Class();
        (void)DOMImplementation::JxxClassInfoMarker::Class();
        (void)DOMImplementationList::JxxClassInfoMarker::Class();
        (void)DOMImplementationSource::JxxClassInfoMarker::Class();
        (void)DOMLocator::JxxClassInfoMarker::Class();
        (void)DOMStringList::JxxClassInfoMarker::Class();
        (void)Element::JxxClassInfoMarker::Class();
        (void)Entity::JxxClassInfoMarker::Class();
        (void)EntityReference::JxxClassInfoMarker::Class();
        (void)NamedNodeMap::JxxClassInfoMarker::Class();
        (void)NameList::JxxClassInfoMarker::Class();
        (void)Node::JxxClassInfoMarker::Class();
        (void)NodeList::JxxClassInfoMarker::Class();
        (void)Notation::JxxClassInfoMarker::Class();
        (void)ProcessingInstruction::JxxClassInfoMarker::Class();
        (void)Text::JxxClassInfoMarker::Class();
        (void)TypeInfo::JxxClassInfoMarker::Class();
        (void)UserDataHandler::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::org::w3c::dom
