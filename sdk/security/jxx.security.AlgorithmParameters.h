#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::security {

class AlgorithmParameters
    : public ::jxx::lang::ClassBase<
          AlgorithmParameters,
          ::jxx::lang::Object> {
public:
    explicit AlgorithmParameters(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm);

    ::jxx::Ptr<::jxx::lang::String> getAlgorithm() const;

private:
    ::jxx::Ptr<::jxx::lang::String> algorithm_;
};

} // namespace jxx::security
