#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::nio {
class CharBuffer;
}

namespace jxx::lang {

class Readable
    : public ::jxx::lang::InterfaceBase<Readable> {
public:
    virtual ~Readable() = default;

    virtual ::jxx::lang::jint read(
        const ::jxx::Ptr<::jxx::nio::CharBuffer>& target) = 0;
};

} // namespace jxx::lang
