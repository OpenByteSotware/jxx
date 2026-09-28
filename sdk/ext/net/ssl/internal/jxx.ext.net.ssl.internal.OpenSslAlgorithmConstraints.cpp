#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslAlgorithmConstraints.h"

#include "util/jxx.util.HashSet.h"

namespace jxx::ext::net::ssl::internal {

::jxx::lang::jbool permitsAlgorithm(
    const ::jxx::Ptr<::jxx::security::AlgorithmConstraints>& constraints,
    const ::jxx::Ptr<::jxx::security::CryptoPrimitive>& primitive,
    const ::jxx::Ptr<::jxx::lang::String>& algorithm) {
    if (constraints == nullptr) return true;
    const auto primitives =
        ::jxx::NEW<::jxx::util::HashSet<::jxx::security::CryptoPrimitive>>();
    primitives->add(primitive);
    return constraints->permits(primitives, algorithm, nullptr);
}

std::vector<std::string> filterAlgorithms(
    const ::jxx::Ptr<::jxx::security::AlgorithmConstraints>& constraints,
    const ::jxx::Ptr<::jxx::security::CryptoPrimitive>& primitive,
    const std::vector<std::string>& algorithms) {
    std::vector<std::string> permitted;
    permitted.reserve(algorithms.size());
    for (const auto& algorithm : algorithms) {
        if (permitsAlgorithm(
                constraints,
                primitive,
                ::jxx::NEW<::jxx::lang::String>(algorithm)))
            permitted.push_back(algorithm);
    }
    return permitted;
}

} // namespace jxx::ext::net::ssl::internal
