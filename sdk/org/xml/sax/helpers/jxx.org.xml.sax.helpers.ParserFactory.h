#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::org::xml::sax {
class Parser;
}

namespace jxx::org::xml::sax::helpers {

class ParserFactory final : public ::jxx::lang::Object {
public:
    using JxxSuper = ::jxx::lang::Object;

    static ::jxx::Ptr<::jxx::org::xml::sax::Parser>
    makeParser();

    static ::jxx::Ptr<::jxx::org::xml::sax::Parser>
    makeParser(
        const ::jxx::Ptr<::jxx::lang::String>& className);

private:
    ParserFactory() = delete;
};

} // namespace jxx::org::xml::sax::helpers
