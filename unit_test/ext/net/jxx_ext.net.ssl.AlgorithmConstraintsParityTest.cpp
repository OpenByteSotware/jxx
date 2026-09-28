#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "security/jxx.security.AlgorithmConstraints.h"
namespace {
class AllowAllConstraints final
    : public ::jxx::lang::InterfaceBase<
          AllowAllConstraints,
          ::jxx::security::AlgorithmConstraints> {
public:
    using PrimitiveSet = ::jxx::security::AlgorithmConstraints::PrimitiveSet;
    ::jxx::lang::jbool permits(
        const ::jxx::Ptr<PrimitiveSet>&,
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<::jxx::security::AlgorithmParameters>&) override { return true; }
    ::jxx::lang::jbool permits(
        const ::jxx::Ptr<PrimitiveSet>&,
        const ::jxx::Ptr<::jxx::security::Key>&) override { return true; }
    ::jxx::lang::jbool permits(
        const ::jxx::Ptr<PrimitiveSet>&,
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<::jxx::security::Key>&,
        const ::jxx::Ptr<::jxx::security::AlgorithmParameters>&) override { return true; }
};
TEST(AlgorithmConstraintsParityTest, SSLParametersRoundTripsIdentity) {
    const auto parameters = ::jxx::NEW<::jxx::ext::net::ssl::SSLParameters>();
    EXPECT_EQ(parameters->getAlgorithmConstraints(), nullptr);
    const auto constraints = ::jxx::NEW<AllowAllConstraints>();
    parameters->setAlgorithmConstraints(constraints);
    EXPECT_EQ(parameters->getAlgorithmConstraints(), constraints);
}
}
