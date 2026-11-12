#include <gtest/gtest.h>

#include "ext/crypto/jxx.ext.crypto.Cipher.h"
#include "ext/security/auth/jxx.ext.security.auth.Subject.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamSource.h"
#include "lang/jxx.lang.String.h"

TEST(MissingJava8TypesTest, CipherMetadataAndConstants) {
    EXPECT_NE(nullptr, ::jxx::ext::crypto::Cipher::Class());
    EXPECT_EQ(1, ::jxx::ext::crypto::Cipher::ENCRYPT_MODE);
    EXPECT_EQ(2, ::jxx::ext::crypto::Cipher::DECRYPT_MODE);
}

TEST(MissingJava8TypesTest, SubjectDefaultsAndReadOnlyState) {
    auto subject = ::jxx::NEW<::jxx::ext::security::auth::Subject>();
    EXPECT_FALSE(subject->isReadOnly());
    EXPECT_TRUE(subject->getPrincipals()->isEmpty());
    subject->setReadOnly();
    EXPECT_TRUE(subject->isReadOnly());
}

TEST(MissingJava8TypesTest, StreamSourceJava8Surface) {
    auto source = ::jxx::NEW<::jxx::ext::xml::transform::stream::StreamSource>();
    EXPECT_TRUE(source->isEmpty());
    auto id = ::jxx::NEW<::jxx::lang::String>("file:/tmp/input.xml");
    source->setSystemId(id);
    EXPECT_EQ(id, source->getSystemId());
    EXPECT_FALSE(source->isEmpty());
    EXPECT_NE(nullptr, ::jxx::ext::xml::transform::stream::StreamSource::FEATURE());
}
