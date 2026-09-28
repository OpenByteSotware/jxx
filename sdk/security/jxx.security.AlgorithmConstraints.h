#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "security/jxx.security.AlgorithmParameters.h"
#include "security/jxx.security.CryptoPrimitive.h"
#include "security/jxx.security.Key.h"
#include "util/jxx.util.Set.h"

namespace jxx::security {

class AlgorithmConstraints
    : public ::jxx::lang::InterfaceBase<AlgorithmConstraints> {
public:
    using PrimitiveSet = ::jxx::util::Set<CryptoPrimitive>;

    ~AlgorithmConstraints() override = default;

    virtual ::jxx::lang::jbool permits(
        const ::jxx::Ptr<PrimitiveSet>& primitives,
        const ::jxx::Ptr<::jxx::lang::String>& algorithm,
        const ::jxx::Ptr<AlgorithmParameters>& parameters) = 0;

    virtual ::jxx::lang::jbool permits(
        const ::jxx::Ptr<PrimitiveSet>& primitives,
        const ::jxx::Ptr<Key>& key) = 0;

    virtual ::jxx::lang::jbool permits(
        const ::jxx::Ptr<PrimitiveSet>& primitives,
        const ::jxx::Ptr<::jxx::lang::String>& algorithm,
        const ::jxx::Ptr<Key>& key,
        const ::jxx::Ptr<AlgorithmParameters>& parameters) = 0;
};

} // namespace jxx::security
