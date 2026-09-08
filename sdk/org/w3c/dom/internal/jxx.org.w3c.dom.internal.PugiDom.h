#pragma once
#include <memory>
#include <string>
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
namespace jxx::org::w3c::dom::internal {
jxx::Ptr<Document> parseDocument(const std::string& xml,jxx::lang::jbool ignoreComments,jxx::lang::jbool coalescing,std::string& error,jxx::lang::jint& offset);
jxx::Ptr<Document> loadDocument(const std::string& path,jxx::lang::jbool ignoreComments,jxx::lang::jbool coalescing,std::string& error,jxx::lang::jint& offset);
jxx::Ptr<Document> newDocument();
jxx::Ptr<DOMImplementation> implementation();
}
