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
        values[static_cast<unsigned char>(alphabet[index])] =
            static_cast<int>(index);
    }

    output.clear();
    int accumulator = 0;
    int bits = -8;
    for (unsigned char character : text) {
        if (character == '=') {
            break;
        }
        const int value = values[character];
        if (value < 0) {
            return false;
        }
        accumulator = (accumulator << 6) | value;
        bits += 6;
        if (bits >= 0) {
            output.push_back(
                static_cast<char>((accumulator >> bits) & 0xff));
            bits -= 8;
        }
    }
    return true;
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
        "Basic realm=\"" + realm_->utf8() + "\"";

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
