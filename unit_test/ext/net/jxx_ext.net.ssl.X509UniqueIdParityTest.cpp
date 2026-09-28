#include <gtest/gtest.h>
#include <type_traits>
#include "security/cert/jxx.security.cert.X509Certificate.h"
namespace { TEST(X509UniqueIdParityTest, ApisAreDeclared) { using C=::jxx::security::cert::X509Certificate; EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&C::getIssuerUniqueID)>)); EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&C::getSubjectUniqueID)>)); } }
