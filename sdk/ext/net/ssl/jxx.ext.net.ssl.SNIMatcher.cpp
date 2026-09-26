#include "ext/net/ssl/jxx.ext.net.ssl.SNIMatcher.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::ext::net::ssl {
SNIMatcher::SNIMatcher(::jxx::lang::jint type) : type_(type) {
    if (type < 0 || type > 255)
        throw ::jxx::lang::IllegalArgumentException();
}
::jxx::lang::jint SNIMatcher::getType() const { return type_; }
} // namespace jxx::ext::net::ssl
