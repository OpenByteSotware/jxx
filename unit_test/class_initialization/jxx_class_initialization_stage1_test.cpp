#include <gtest/gtest.h>

#include "util/jxx.util.ClassInitialization.h"
#include "util/jxx.util.Date.h"
#include "util/zip/jxx.util.zip.ClassInitialization.h"
#include "util/zip/jxx.util.zip.GZIPInputStream.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.ClassInitialization.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicBoolean.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ClassInitialization.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.LockSupport.h"
#include "nio/channels/spi/jxx.nio.channels.spi.ClassInitialization.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
#include "nio/file/attribute/jxx.nio.file.attribute.ClassInitialization.h"
#include "nio/file/attribute/jxx.nio.file.attribute.FileTime.h"
#include "security/cert/jxx.security.cert.ClassInitialization.h"
#include "security/cert/jxx.security.cert.Certificate.h"
#include "ext/crypto/jxx.ext.crypto.ClassInitialization.h"
#include "ext/crypto/jxx.ext.crypto.Cipher.h"
#include "ext/net/jxx.ext.net.ClassInitialization.h"
#include "ext/net/jxx.ext.net.ServerSocketFactory.h"
#include "ext/security/auth/jxx.ext.security.auth.ClassInitialization.h"
#include "ext/security/auth/jxx.ext.security.auth.Subject.h"
#include "ext/security/auth/x500/jxx.ext.security.auth.x500.ClassInitialization.h"
#include "ext/security/auth/x500/jxx.ext.security.auth.x500.X500Principal.h"
#include "ext/xml/bind/jxx.ext.xml.bind.ClassInitialization.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBContext.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.ClassInitialization.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlRootElement.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.ClassInitialization.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlAdapter.h"
#include "ext/xml/namespace/jxx.ext.xml.namespace.ClassInitialization.h"
#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.ClassInitialization.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "ext/xml/transform/jxx.ext.xml.transform.ClassInitialization.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerFactory.h"
#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.ClassInitialization.h"
#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.DOMSource.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.ClassInitialization.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamSource.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.ClassInitialization.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathFactory.h"

TEST(ClassInitializationStage1Test, PublicParityTypesHaveMetadata) {
    EXPECT_NO_THROW(::jxx::util::initializeClasses());
    EXPECT_NO_THROW(::jxx::util::zip::initializeClasses());
    EXPECT_NO_THROW(::jxx::util::concurrent::atomic::initializeClasses());
    EXPECT_NO_THROW(::jxx::util::concurrent::locks::initializeClasses());
    EXPECT_NO_THROW(::jxx::nio::channels::spi::initializeClasses());
    EXPECT_NO_THROW(::jxx::nio::file::attribute::initializeClasses());
    EXPECT_NO_THROW(::jxx::security::cert::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::crypto::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::net::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::security::auth::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::security::auth::x500::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::bind::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::bind::annotation::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::bind::annotation::adapters::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::namespace_::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::parsers::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::transform::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::transform::dom::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::transform::stream::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::xpath::initializeClasses());

    EXPECT_NE(nullptr, ::jxx::util::Date::Class());
    EXPECT_NE(nullptr, ::jxx::util::zip::GZIPInputStream::Class());
    EXPECT_NE(nullptr, ::jxx::util::concurrent::atomic::AtomicBoolean::Class());
    EXPECT_NE(nullptr, ::jxx::nio::file::attribute::FileTime::Class());
    EXPECT_NE(nullptr, ::jxx::security::cert::Certificate::Class());
    EXPECT_NE(nullptr, ::jxx::ext::crypto::Cipher::Class());
    EXPECT_NE(nullptr, ::jxx::ext::security::auth::Subject::Class());
    EXPECT_NE(nullptr, ::jxx::ext::security::auth::x500::X500Principal::Class());
    EXPECT_NE(nullptr, ::jxx::ext::xml::namespace_::QName::Class());
    EXPECT_NE(nullptr, ::jxx::ext::xml::transform::dom::DOMSource::Class());
    EXPECT_NE(nullptr, ::jxx::ext::xml::transform::stream::StreamSource::Class());
}
