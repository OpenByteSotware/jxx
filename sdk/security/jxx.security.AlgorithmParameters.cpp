#include "security/jxx.security.AlgorithmParameters.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::security {

AlgorithmParameters::AlgorithmParameters(
    const ::jxx::Ptr<::jxx::lang::String>& algorithm)
    : algorithm_(algorithm) {
    if (algorithm_ == nullptr)
        throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<::jxx::lang::String>
AlgorithmParameters::getAlgorithm() const {
    return algorithm_;
}

} // namespace jxx::security
