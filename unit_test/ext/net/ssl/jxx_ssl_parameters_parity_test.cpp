#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.String.h"

namespace {
::jxx::Ptr<::jxx::lang::String> text(const char* value) {
    return ::jxx::NEW<::jxx::lang::String>(value);
}
}

TEST(SSLParametersParityTest, CipherSuiteArraysAreDefensivelyCopied) {
    using Parameters = ::jxx::ext::net::ssl::SSLParameters;
    const auto source = ::jxx::NEW<Parameters::StringArray>(1);
    (*source)[0] = text("first");
    auto parameters = ::jxx::NEW<Parameters>();
    parameters->setCipherSuites(source);
    (*source)[0] = text("changed");
    auto firstRead = parameters->getCipherSuites();
    ASSERT_NE(nullptr, firstRead);
    EXPECT_EQ("first", (*firstRead)[0]->utf8());
    (*firstRead)[0] = text("changed-again");
    EXPECT_EQ("first", (*parameters->getCipherSuites())[0]->utf8());
}

TEST(SSLParametersParityTest, ProtocolArraysAreDefensivelyCopied) {
    using Parameters = ::jxx::ext::net::ssl::SSLParameters;
    const auto source = ::jxx::NEW<Parameters::StringArray>(1);
    (*source)[0] = text("TLSv1.2");
    auto parameters = ::jxx::NEW<Parameters>();
    parameters->setProtocols(source);
    (*source)[0] = text("changed");
    EXPECT_EQ("TLSv1.2", (*parameters->getProtocols())[0]->utf8());
}

TEST(SSLParametersParityTest, NeedAndWantClientAuthAreMutuallyExclusive) {
    auto parameters = ::jxx::NEW<::jxx::ext::net::ssl::SSLParameters>();
    parameters->setWantClientAuth(true);
    EXPECT_TRUE(parameters->getWantClientAuth());
    EXPECT_FALSE(parameters->getNeedClientAuth());
    parameters->setNeedClientAuth(true);
    EXPECT_TRUE(parameters->getNeedClientAuth());
    EXPECT_FALSE(parameters->getWantClientAuth());
}
