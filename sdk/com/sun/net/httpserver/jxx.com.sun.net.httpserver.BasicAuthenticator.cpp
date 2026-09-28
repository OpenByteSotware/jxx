#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.BasicAuthenticator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Headers.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpExchange.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpPrincipal.h"
#include "lang/jxx.lang.Exceptions.h"

#include <array>
#include <string>

namespace jxx::com::sun::net::httpserver {
namespace {

bool decodeBase64(const std::string& text, std::string& output)
{
    static const std::string alphabet =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::array<int, 256> values{};
    values.fill(-1);
    for (std::size_t index = 0; index < alphabet.size(); ++index) {
        values[static_cast<unsigned char>(alphabet[index])] = static_cast<int>(index);
    }

    output.clear();
    if (text.empty() || (text.size() % 4U) != 0U) return false;

    const auto firstPadding = text.find('=');
    const auto payloadLength = firstPadding == std::string::npos ? text.size() : firstPadding;
    const auto paddingLength = text.size() - payloadLength;
    if (paddingLength > 2U) return false;
    for (std::size_t index = payloadLength; index < text.size(); ++index) {
        if (text[index] != '=') return false;
    }
    if (paddingLength == 1U && (payloadLength % 4U) != 3U) return false;
    if (paddingLength == 2U && (payloadLength % 4U) != 2U) return false;
    if (paddingLength == 0U && (payloadLength % 4U) != 0U) return false;

    for (std::size_t block = 0; block < text.size(); block += 4U) {
        const bool finalBlock = block + 4U == text.size();
        const unsigned char first = static_cast<unsigned char>(text[block]);
        const unsigned char second = static_cast<unsigned char>(text[block + 1U]);
        const unsigned char third = static_cast<unsigned char>(text[block + 2U]);
        const unsigned char fourth = static_cast<unsigned char>(text[block + 3U]);
        if (values[first] < 0 || values[second] < 0) return false;
        if (third == '=' && (!finalBlock || fourth != '=')) return false;
        if (fourth == '=' && !finalBlock) return false;
        if (third != '=' && values[third] < 0) return false;
        if (fourth != '=' && values[fourth] < 0) return false;

        const unsigned int value =
            (static_cast<unsigned int>(values[first]) << 18U) |
            (static_cast<unsigned int>(values[second]) << 12U) |
            (third == '=' ? 0U : static_cast<unsigned int>(values[third]) << 6U) |
            (fourth == '=' ? 0U : static_cast<unsigned int>(values[fourth]));
        output.push_back(static_cast<char>((value >> 16U) & 0xffU));
        if (third != '=') output.push_back(static_cast<char>((value >> 8U) & 0xffU));
        if (fourth != '=') output.push_back(static_cast<char>(value & 0xffU));
    }
    return true;
}

std::string quotedRealm(const std::string& realm)
{
    std::string output;
    output.reserve(realm.size() + 2U);
    output.push_back('"');
    for (const auto character : realm) {
        if (character == '"' || character == '\\') output.push_back('\\');
        output.push_back(character);
    }
    output.push_back('"');
    return output;
}

} // namespace

BasicAuthenticator::BasicAuthenticator(
    const ::jxx::Ptr<::jxx::lang::String>& realm)
    : Super(), realm_(realm)
{
    if (realm_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::String>
BasicAuthenticator::getRealm() const
{
    return realm_;
}

::jxx::Ptr<Authenticator::Result>
BasicAuthenticator::authenticate(
    const ::jxx::Ptr<HttpExchange>& exchange)
{
    if (exchange == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    auto authorization = exchange->getRequestHeaders()->getFirst(
        ::jxx::NEW<::jxx::lang::String>("Authorization"));

    const std::string challenge =
        "Basic realm=" + quotedRealm(realm_->utf8());

    if (authorization == nullptr) {
        exchange->getResponseHeaders()->set(
            ::jxx::NEW<::jxx::lang::String>("WWW-Authenticate"),
            ::jxx::NEW<::jxx::lang::String>(challenge.c_str()));
        return ::jxx::NEW<Authenticator::Retry>(401);
    }

    const std::string header = authorization->utf8();
    if (header.size() < 6 ||
        !((header[0] == 'B' || header[0] == 'b') &&
          (header[1] == 'A' || header[1] == 'a') &&
          (header[2] == 'S' || header[2] == 's') &&
          (header[3] == 'I' || header[3] == 'i') &&
          (header[4] == 'C' || header[4] == 'c') &&
          header[5] == ' ')) {
        return ::jxx::NEW<Authenticator::Failure>(401);
    }

    std::string decoded;
    if (!decodeBase64(header.substr(6), decoded)) {
        return ::jxx::NEW<Authenticator::Failure>(401);
    }

    const auto separator = decoded.find(':');
    if (separator == std::string::npos) {
        return ::jxx::NEW<Authenticator::Failure>(401);
    }

    auto username = ::jxx::NEW<::jxx::lang::String>(
        decoded.substr(0, separator).c_str());
    auto password = ::jxx::NEW<::jxx::lang::String>(
        decoded.substr(separator + 1).c_str());

    if (!checkCredentials(username, password)) {
        return ::jxx::NEW<Authenticator::Failure>(401);
    }

    return ::jxx::NEW<Authenticator::Success>(
        ::jxx::NEW<HttpPrincipal>(username, realm_));
}

} // namespace jxx::com::sun::net::httpserver
