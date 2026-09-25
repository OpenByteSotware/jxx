#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::util::zip {

class Checksum : public ::jxx::lang::InterfaceBase<Checksum> {
public:
    using Super = ::jxx::lang::InterfaceBase<Checksum>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    ~Checksum() override = default;

    virtual void update(::jxx::lang::jint value) = 0;

    virtual void update(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) = 0;

    virtual ::jxx::lang::jlong getValue() const noexcept = 0;
    virtual void reset() noexcept = 0;
};

} // namespace jxx::util::zip
