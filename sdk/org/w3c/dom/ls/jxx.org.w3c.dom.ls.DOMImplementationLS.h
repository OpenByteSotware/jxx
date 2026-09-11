#pragma once

#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::org::w3c::dom::ls {

class LSInput;
class LSOutput;
class LSParser;
class LSSerializer;

class DOMImplementationLS {
public:
    virtual ~DOMImplementationLS() = default;

    enum : ::jxx::lang::jshort {
        MODE_SYNCHRONOUS = 1,
        MODE_ASYNCHRONOUS = 2
    };

    virtual ::jxx::Ptr<LSParser> createLSParser(
        ::jxx::lang::jshort mode,
        const ::jxx::Ptr<::jxx::lang::String>& schemaType) = 0;

    virtual ::jxx::Ptr<LSSerializer> createLSSerializer() = 0;
    virtual ::jxx::Ptr<LSInput> createLSInput() = 0;
    virtual ::jxx::Ptr<LSOutput> createLSOutput() = 0;
};

} // namespace jxx::org::w3c::dom::ls
