#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSignatureAlgorithms.h"

#include <string>
#include <vector>

#include <openssl/objects.h>

#include "lang/jxx.lang.String.h"

namespace jxx::ext::net::ssl::internal {
namespace {

using StringArray = ::jxx::ext::net::ssl::SSLSession::StringArray;

std::string normalizedName(int signatureNid, int hashNid, int signatureHashNid) {
    if (signatureHashNid != NID_undef) {
        const char* combined = OBJ_nid2sn(signatureHashNid);
        if (combined != nullptr) return combined;
    }
    const char* signature = OBJ_nid2sn(signatureNid);
    const char* hash = OBJ_nid2sn(hashNid);
    if (signature == nullptr) return "UNKNOWN";
    if (hash == nullptr || hashNid == NID_undef) return signature;
    return std::string(hash) + "with" + signature;
}

::jxx::Ptr<StringArray> toArray(const std::vector<std::string>& values) {
    const auto result = ::jxx::NEW<StringArray>(
        static_cast<::jxx::lang::jint>(values.size()));
    for (::jxx::lang::jint index = 0;
         index < static_cast<::jxx::lang::jint>(values.size());
         ++index)
        (*result)[index] = ::jxx::NEW<::jxx::lang::String>(
            values[static_cast<std::size_t>(index)]);
    return result;
}

void appendUnique(std::vector<std::string>& values, const std::string& value) {
    for (const auto& existing : values)
        if (existing == value) return;
    values.push_back(value);
}

} // namespace

::jxx::Ptr<StringArray> localSupportedSignatureAlgorithms(SSL* ssl) {
    std::vector<std::string> values;
    if (ssl == nullptr) return toArray(values);
    const int count = SSL_get_shared_sigalgs(
        ssl, 0, nullptr, nullptr, nullptr, nullptr, nullptr);
    for (int index = 0; index < count; ++index) {
        int signatureNid = NID_undef;
        int hashNid = NID_undef;
        int signatureHashNid = NID_undef;
        if (SSL_get_shared_sigalgs(
                ssl, index, &signatureNid, &hashNid,
                &signatureHashNid, nullptr, nullptr) > 0)
            appendUnique(values,
                normalizedName(signatureNid, hashNid, signatureHashNid));
    }
    return toArray(values);
}

::jxx::Ptr<StringArray> peerSupportedSignatureAlgorithms(SSL* ssl) {
    std::vector<std::string> values;
    if (ssl == nullptr) return toArray(values);
    const int count = SSL_get_sigalgs(
        ssl, 0, nullptr, nullptr, nullptr, nullptr, nullptr);
    for (int index = 0; index < count; ++index) {
        int signatureNid = NID_undef;
        int hashNid = NID_undef;
        int signatureHashNid = NID_undef;
        if (SSL_get_sigalgs(
                ssl, index, &signatureNid, &hashNid,
                &signatureHashNid, nullptr, nullptr) > 0)
            appendUnique(values,
                normalizedName(signatureNid, hashNid, signatureHashNid));
    }
    return toArray(values);
}

} // namespace jxx::ext::net::ssl::internal
