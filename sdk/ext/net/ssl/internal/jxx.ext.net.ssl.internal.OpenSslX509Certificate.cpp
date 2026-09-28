#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"

#include <ctime>
#include <string>

#include <openssl/asn1.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPublicKey.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.X509Principal.h"
#include "ext/security/auth/x500/jxx.ext.security.auth.x500.X500Principal.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "math/jxx.math.BigInteger.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.HashSet.h"

namespace jxx::ext::net::ssl::internal {
namespace {

X509* decodeCertificate(const ::jxx::lang::ByteArray& encoded) {
    if (encoded == nullptr)
        throw ::jxx::lang::NullPointerException();
    const unsigned char* cursor =
        reinterpret_cast<const unsigned char*>(&(*encoded)[0]);
    X509* certificate = d2i_X509(nullptr, &cursor, encoded->length);
    if (certificate == nullptr)
        throw ::jxx::io::IOException("Invalid DER X.509 certificate");
    return certificate;
}

::jxx::lang::ByteArray copyBytes(const unsigned char* data, int length) {
    if (data == nullptr || length < 0) return nullptr;
    const auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    for (int index = 0; index < length; ++index)
        (*result)[static_cast<::jxx::lang::jint>(index)] =
            static_cast<::jxx::lang::jbyte>(data[index]);
    return result;
}

::jxx::lang::jlong epochMillis(const ASN1_TIME* value) {
    std::tm time{};
    if (value == nullptr || ASN1_TIME_to_tm(value, &time) != 1)
        throw ::jxx::io::IOException("Invalid X.509 validity time");
#ifdef _WIN32
    const std::time_t epoch = _mkgmtime(&time);
#else
    const std::time_t epoch = timegm(&time);
#endif
    if (epoch == static_cast<std::time_t>(-1))
        throw ::jxx::io::IOException("X.509 validity time is out of range");
    return static_cast<::jxx::lang::jlong>(epoch) * 1000;
}

::jxx::Ptr<::jxx::security::Principal> distinguishedName(
    const ::jxx::lang::ByteArray& encoded,
    bool issuer) {
    X509* certificate = decodeCertificate(encoded);
    X509_NAME* name = issuer
        ? X509_get_issuer_name(certificate)
        : X509_get_subject_name(certificate);
    BIO* memory = BIO_new(BIO_s_mem());
    if (name == nullptr || memory == nullptr) {
        if (memory != nullptr) BIO_free(memory);
        X509_free(certificate);
        throw ::jxx::io::IOException(
            "Unable to read X.509 distinguished name");
    }
    const unsigned long flags =
        XN_FLAG_RFC2253 & ~ASN1_STRFLGS_ESC_MSB;
    if (X509_NAME_print_ex(memory, name, 0, flags) < 0) {
        BIO_free(memory);
        X509_free(certificate);
        throw ::jxx::io::IOException(
            "Unable to format X.509 distinguished name");
    }
    char* data = nullptr;
    const long length = BIO_get_mem_data(memory, &data);
    const std::string value = data == nullptr || length <= 0
        ? std::string()
        : std::string(data, static_cast<std::size_t>(length));
    BIO_free(memory);
    X509_free(certificate);
    return ::jxx::NEW<X509Principal>(
        ::jxx::NEW<::jxx::lang::String>(value));
}

::jxx::lang::BooleanArray uniqueId(
    const ::jxx::lang::ByteArray& encoded,
    bool issuer) {
    X509* certificate = decodeCertificate(encoded);
    const ASN1_BIT_STRING* value = nullptr;
    if (issuer)
        X509_get0_uids(certificate, &value, nullptr);
    else
        X509_get0_uids(certificate, nullptr, &value);
    if (value == nullptr) {
        X509_free(certificate);
        return nullptr;
    }
    const int unusedBits = value->flags & 0x7;
    const int bitCount = value->length * 8 - unusedBits;
    const auto result =
        ::jxx::NEW<::jxx::lang::BooleanArrayType>(bitCount);
    for (int bit = 0; bit < bitCount; ++bit)
        (*result)[static_cast<::jxx::lang::jint>(bit)] =
            ASN1_BIT_STRING_get_bit(value, bit) != 0;
    X509_free(certificate);
    return result;
}

::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>> extensionOids(
    const ::jxx::lang::ByteArray& encoded,
    bool critical) {
    X509* certificate = decodeCertificate(encoded);
    const auto result =
        ::jxx::NEW<::jxx::util::HashSet<::jxx::lang::String>>();
    const int count = X509_get_ext_count(certificate);
    for (int index = 0; index < count; ++index) {
        X509_EXTENSION* extension = X509_get_ext(certificate, index);
        if ((X509_EXTENSION_get_critical(extension) != 0) != critical)
            continue;
        char buffer[128]{};
        if (OBJ_obj2txt(buffer, sizeof(buffer),
                X509_EXTENSION_get_object(extension), 1) > 0)
            result->add(::jxx::NEW<::jxx::lang::String>(buffer));
    }
    X509_free(certificate);
    return result->isEmpty() ? nullptr : result;
}

} // namespace

OpenSslX509Certificate::OpenSslX509Certificate(
    const ::jxx::lang::ByteArray& encoded)
    : Super(::jxx::NEW<::jxx::lang::String>("X.509"))
    , encoded_(encoded) {
    if (encoded_ == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::lang::ByteArray OpenSslX509Certificate::getEncoded() const {
    const auto copy = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(encoded_->length);
    for (::jxx::lang::jint index = 0; index < encoded_->length; ++index)
        (*copy)[index] = (*encoded_)[index];
    return copy;
}

void OpenSslX509Certificate::checkValidity() const {
    X509* certificate = decodeCertificate(encoded_);
    const bool valid =
        X509_cmp_current_time(X509_get0_notBefore(certificate)) <= 0 &&
        X509_cmp_current_time(X509_get0_notAfter(certificate)) >= 0;
    X509_free(certificate);
    if (!valid)
        throw ::jxx::io::IOException(
            "X.509 certificate is not currently valid");
}

::jxx::lang::jint OpenSslX509Certificate::getVersion() const {
    X509* certificate = decodeCertificate(encoded_);
    const auto result = static_cast<::jxx::lang::jint>(
        X509_get_version(certificate) + 1);
    X509_free(certificate);
    return result;
}

::jxx::Ptr<::jxx::math::BigInteger>
OpenSslX509Certificate::getSerialNumber() const {
    X509* certificate = decodeCertificate(encoded_);
    BIGNUM* number = ASN1_INTEGER_to_BN(
        X509_get_serialNumber(certificate), nullptr);
    X509_free(certificate);
    if (number == nullptr)
        throw ::jxx::io::IOException(
            "Unable to decode X.509 serial number");
    char* decimal = BN_bn2dec(number);
    BN_free(number);
    if (decimal == nullptr)
        throw ::jxx::io::IOException(
            "Unable to format X.509 serial number");
    const auto result = ::jxx::NEW<::jxx::math::BigInteger>(
        ::jxx::NEW<::jxx::lang::String>(decimal));
    OPENSSL_free(decimal);
    return result;
}

::jxx::Ptr<::jxx::util::Date>
OpenSslX509Certificate::getNotBefore() const {
    X509* certificate = decodeCertificate(encoded_);
    const auto result = epochMillis(X509_get0_notBefore(certificate));
    X509_free(certificate);
    return ::jxx::NEW<::jxx::util::Date>(result);
}

::jxx::Ptr<::jxx::util::Date>
OpenSslX509Certificate::getNotAfter() const {
    X509* certificate = decodeCertificate(encoded_);
    const auto result = epochMillis(X509_get0_notAfter(certificate));
    X509_free(certificate);
    return ::jxx::NEW<::jxx::util::Date>(result);
}

::jxx::lang::ByteArray OpenSslX509Certificate::getTBSCertificate() const {
    X509* certificate = decodeCertificate(encoded_);
    const int length = i2d_re_X509_tbs(certificate, nullptr);
    if (length <= 0) {
        X509_free(certificate);
        throw ::jxx::io::IOException("Unable to encode TBSCertificate");
    }
    const auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    unsigned char* cursor =
        reinterpret_cast<unsigned char*>(&(*result)[0]);
    const int written = i2d_re_X509_tbs(certificate, &cursor);
    X509_free(certificate);
    if (written != length)
        throw ::jxx::io::IOException("Unable to encode TBSCertificate");
    return result;
}

::jxx::lang::ByteArray OpenSslX509Certificate::getSignature() const {
    X509* certificate = decodeCertificate(encoded_);
    const ASN1_BIT_STRING* signature = nullptr;
    const X509_ALGOR* algorithm = nullptr;
    X509_get0_signature(&signature, &algorithm, certificate);
    const auto result = signature == nullptr
        ? nullptr
        : copyBytes(signature->data, signature->length);
    X509_free(certificate);
    return result;
}

::jxx::Ptr<::jxx::lang::String>
OpenSslX509Certificate::getSigAlgName() const {
    X509* certificate = decodeCertificate(encoded_);
    const int nid = X509_get_signature_nid(certificate);
    const char* name = OBJ_nid2ln(nid);
    if (name == nullptr) name = OBJ_nid2sn(nid);
    const auto result = ::jxx::NEW<::jxx::lang::String>(
        name == nullptr ? "UNKNOWN" : name);
    X509_free(certificate);
    return result;
}

::jxx::Ptr<::jxx::lang::String>
OpenSslX509Certificate::getSigAlgOID() const {
    X509* certificate = decodeCertificate(encoded_);
    const ASN1_BIT_STRING* signature = nullptr;
    const X509_ALGOR* algorithm = nullptr;
    X509_get0_signature(&signature, &algorithm, certificate);
    const ASN1_OBJECT* object = nullptr;
    X509_ALGOR_get0(&object, nullptr, nullptr, algorithm);
    char buffer[128]{};
    const int length = object == nullptr
        ? 0
        : OBJ_obj2txt(buffer, sizeof(buffer), object, 1);
    X509_free(certificate);
    if (length <= 0)
        throw ::jxx::io::IOException(
            "Unable to read X.509 signature OID");
    return ::jxx::NEW<::jxx::lang::String>(buffer);
}

::jxx::lang::ByteArray OpenSslX509Certificate::getSigAlgParams() const {
    X509* certificate = decodeCertificate(encoded_);
    const ASN1_BIT_STRING* signature = nullptr;
    const X509_ALGOR* algorithm = nullptr;
    X509_get0_signature(&signature, &algorithm, certificate);
    const ASN1_OBJECT* object = nullptr;
    int parameterType = V_ASN1_UNDEF;
    const void* parameterValue = nullptr;
    X509_ALGOR_get0(&object, &parameterType, &parameterValue, algorithm);
    if (parameterType == V_ASN1_UNDEF || parameterType == V_ASN1_NULL) {
        X509_free(certificate);
        return nullptr;
    }
    ASN1_TYPE* parameter = ASN1_TYPE_new();
    if (parameter == nullptr) {
        X509_free(certificate);
        throw ::jxx::io::IOException(
            "Unable to allocate X.509 signature parameters");
    }
    parameter->type = parameterType;
    parameter->value.ptr = const_cast<char*>(
        static_cast<const char*>(parameterValue));
    const int length = i2d_ASN1_TYPE(parameter, nullptr);
    if (length <= 0) {
        parameter->value.ptr = nullptr;
        ASN1_TYPE_free(parameter);
        X509_free(certificate);
        return nullptr;
    }
    const auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    unsigned char* cursor =
        reinterpret_cast<unsigned char*>(&(*result)[0]);
    i2d_ASN1_TYPE(parameter, &cursor);
    parameter->value.ptr = nullptr;
    ASN1_TYPE_free(parameter);
    X509_free(certificate);
    return result;
}

::jxx::Ptr<::jxx::security::PublicKey>
OpenSslX509Certificate::getPublicKey() const {
    X509* certificate = decodeCertificate(encoded_);
    EVP_PKEY* key = X509_get_pubkey(certificate);
    X509_free(certificate);
    if (key == nullptr)
        throw ::jxx::io::IOException(
            "X.509 certificate has no public key");
    const int length = i2d_PUBKEY(key, nullptr);
    const char* algorithm = OBJ_nid2sn(EVP_PKEY_base_id(key));
    if (length <= 0) {
        EVP_PKEY_free(key);
        throw ::jxx::io::IOException(
            "Unable to encode X.509 public key");
    }
    const auto encoded = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    unsigned char* cursor =
        reinterpret_cast<unsigned char*>(&(*encoded)[0]);
    if (i2d_PUBKEY(key, &cursor) != length) {
        EVP_PKEY_free(key);
        throw ::jxx::io::IOException(
            "Unable to encode X.509 public key");
    }
    EVP_PKEY_free(key);
    return ::jxx::NEW<OpenSslPublicKey>(
        ::jxx::NEW<::jxx::lang::String>(
            algorithm == nullptr ? "UNKNOWN" : algorithm),
        encoded);
}

::jxx::lang::BooleanArray
OpenSslX509Certificate::getIssuerUniqueID() const {
    return uniqueId(encoded_, true);
}

::jxx::lang::BooleanArray
OpenSslX509Certificate::getSubjectUniqueID() const {
    return uniqueId(encoded_, false);
}

::jxx::lang::BooleanArray OpenSslX509Certificate::getKeyUsage() const {
    X509* certificate = decodeCertificate(encoded_);
    ASN1_BIT_STRING* usage = static_cast<ASN1_BIT_STRING*>(
        X509_get_ext_d2i(certificate, NID_key_usage, nullptr, nullptr));
    X509_free(certificate);
    if (usage == nullptr) return nullptr;
    const auto result = ::jxx::NEW<::jxx::lang::BooleanArrayType>(9);
    for (::jxx::lang::jint bit = 0; bit < 9; ++bit)
        (*result)[bit] = ASN1_BIT_STRING_get_bit(usage, bit) != 0;
    ASN1_BIT_STRING_free(usage);
    return result;
}

::jxx::Ptr<::jxx::util::List<::jxx::lang::String>>
OpenSslX509Certificate::getExtendedKeyUsage() const {
    X509* certificate = decodeCertificate(encoded_);
    EXTENDED_KEY_USAGE* usage = static_cast<EXTENDED_KEY_USAGE*>(
        X509_get_ext_d2i(certificate, NID_ext_key_usage, nullptr, nullptr));
    X509_free(certificate);
    if (usage == nullptr) return nullptr;
    const auto result =
        ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>();
    const int count = sk_ASN1_OBJECT_num(usage);
    for (int index = 0; index < count; ++index) {
        char buffer[128]{};
        if (OBJ_obj2txt(buffer, sizeof(buffer),
                sk_ASN1_OBJECT_value(usage, index), 1) > 0)
            result->add(::jxx::NEW<::jxx::lang::String>(buffer));
    }
    EXTENDED_KEY_USAGE_free(usage);
    return result;
}

::jxx::lang::jint OpenSslX509Certificate::getBasicConstraints() const {
    X509* certificate = decodeCertificate(encoded_);
    BASIC_CONSTRAINTS* constraints = static_cast<BASIC_CONSTRAINTS*>(
        X509_get_ext_d2i(
            certificate, NID_basic_constraints, nullptr, nullptr));
    X509_free(certificate);
    if (constraints == nullptr) return -1;
    ::jxx::lang::jint result = -1;
    if (constraints->ca) {
        result = constraints->pathlen == nullptr
            ? 2147483647
            : static_cast<::jxx::lang::jint>(
                ASN1_INTEGER_get(constraints->pathlen));
    }
    BASIC_CONSTRAINTS_free(constraints);
    return result;
}

::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>>
OpenSslX509Certificate::getCriticalExtensionOIDs() const {
    return extensionOids(encoded_, true);
}

::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>>
OpenSslX509Certificate::getNonCriticalExtensionOIDs() const {
    return extensionOids(encoded_, false);
}

::jxx::lang::ByteArray OpenSslX509Certificate::getExtensionValue(
    const ::jxx::Ptr<::jxx::lang::String>& oid) const {
    if (oid == nullptr) throw ::jxx::lang::NullPointerException();
    X509* certificate = decodeCertificate(encoded_);
    ASN1_OBJECT* object = OBJ_txt2obj(oid->utf8().c_str(), 1);
    if (object == nullptr) {
        X509_free(certificate);
        return nullptr;
    }
    const int index = X509_get_ext_by_OBJ(certificate, object, -1);
    ASN1_OBJECT_free(object);
    if (index < 0) {
        X509_free(certificate);
        return nullptr;
    }
    X509_EXTENSION* extension = X509_get_ext(certificate, index);
    ASN1_OCTET_STRING* value = X509_EXTENSION_get_data(extension);
    const int length = i2d_ASN1_OCTET_STRING(value, nullptr);
    if (length <= 0) {
        X509_free(certificate);
        return nullptr;
    }
    const auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    unsigned char* cursor =
        reinterpret_cast<unsigned char*>(&(*result)[0]);
    i2d_ASN1_OCTET_STRING(value, &cursor);
    X509_free(certificate);
    return result;
}

::jxx::lang::jbool
OpenSslX509Certificate::hasUnsupportedCriticalExtension() const {
    X509* certificate = decodeCertificate(encoded_);
    bool unsupported = false;
    const int extensionCount = X509_get_ext_count(certificate);
    for (int index = 0; index < extensionCount; ++index) {
        X509_EXTENSION* extension = X509_get_ext(certificate, index);
        if (extension == nullptr || X509_EXTENSION_get_critical(extension) != 1)
            continue;
        ASN1_OBJECT* object = X509_EXTENSION_get_object(extension);
        const int nid = object == nullptr ? NID_undef : OBJ_obj2nid(object);
        if (nid == NID_undef) {
            unsupported = true;
            break;
        }
        const unsigned long flags = X509_supported_extension(extension);
        if (flags == 0UL) {
            unsupported = true;
            break;
        }
    }
    X509_free(certificate);
    return unsupported;
}

::jxx::Ptr<::jxx::security::Principal>
OpenSslX509Certificate::getIssuerDN() const {
    return distinguishedName(encoded_, true);
}

::jxx::Ptr<::jxx::security::Principal>
OpenSslX509Certificate::getSubjectDN() const {
    return distinguishedName(encoded_, false);
}

::jxx::Ptr<::jxx::ext::security::auth::x500::X500Principal>
OpenSslX509Certificate::getIssuerX500Principal() const {
    const auto principal = getIssuerDN();
    return ::jxx::NEW<
        ::jxx::ext::security::auth::x500::X500Principal>(
            principal->getName());
}

::jxx::Ptr<::jxx::ext::security::auth::x500::X500Principal>
OpenSslX509Certificate::getSubjectX500Principal() const {
    const auto principal = getSubjectDN();
    return ::jxx::NEW<
        ::jxx::ext::security::auth::x500::X500Principal>(
            principal->getName());
}

void OpenSslX509Certificate::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) {
    if (output == nullptr) throw ::jxx::lang::NullPointerException();
    output->writeInt(encoded_->length);
    output->write(encoded_);
}

void OpenSslX509Certificate::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) {
    if (input == nullptr) throw ::jxx::lang::NullPointerException();
    const auto length = input->readInt();
    if (length <= 0)
        throw ::jxx::lang::IllegalStateException(
            "invalid serialized X.509 certificate length");
    const auto encoded = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    input->readFully(encoded);
    X509* certificate = decodeCertificate(encoded);
    X509_free(certificate);
    encoded_ = encoded;
}

void OpenSslX509Certificate::readObjectNoData() {
    throw ::jxx::lang::IllegalStateException(
        "X.509 certificate requires serialized data");
}

} // namespace jxx::ext::net::ssl::internal
