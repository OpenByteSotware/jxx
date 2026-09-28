#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"
namespace { TEST(HttpsPrincipalParityTest, ConvenienceApisRemainDeclared) { using C=::jxx::ext::net::ssl::HttpsURLConnection; EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&C::getPeerPrincipal)>)); EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&C::getLocalPrincipal)>)); } }
