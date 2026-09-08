#pragma once

#include <string>

#include "lang/jxx_types.h"

namespace jxx::org::w3c::dom {
class Document;
}

namespace jxx::org::w3c::dom::internal {

::jxx::Ptr<::jxx::org::w3c::dom::Document> parseDocument(
    const std::string& xml,
    ::jxx::lang::jbool ignoreComments,
    ::jxx::lang::jbool coalescing,
    std::string& error,
    ::jxx::lang::jint& offset);

::jxx::Ptr<::jxx::org::w3c::dom::Document> loadDocument(
    const std::string& path,
    ::jxx::lang::jbool ignoreComments,
    ::jxx::lang::jbool coalescing,
    std::string& error,
    ::jxx::lang::jint& offset);

::jxx::Ptr<::jxx::org::w3c::dom::Document> newDocument();

} // namespace jxx::org::w3c::dom::internal
