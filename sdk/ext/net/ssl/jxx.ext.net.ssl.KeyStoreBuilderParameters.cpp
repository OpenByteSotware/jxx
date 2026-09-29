#include "ext/net/ssl/jxx.ext.net.ssl.KeyStoreBuilderParameters.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::net::ssl {

KeyStoreBuilderParameters::KeyStoreBuilderParameters(
    const ::jxx::Ptr<::jxx::security::KeyStore::Builder>& builder) {
    if (builder == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    builders_ = ::jxx::NEW<BuilderArray>(1);
    (*builders_)[0] = builder;
}

KeyStoreBuilderParameters::KeyStoreBuilderParameters(
    const ::jxx::Ptr<BuilderArray>& builders) {
    if (builders == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (builders->length == 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    builders_ = ::jxx::NEW<BuilderArray>(builders->length);
    for (::jxx::lang::jint index = 0; index < builders->length; ++index) {
        if ((*builders)[index] == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        (*builders_)[index] = (*builders)[index];
    }
}

::jxx::Ptr<KeyStoreBuilderParameters::BuilderArray>
KeyStoreBuilderParameters::getParameters() const {
    const auto copy = ::jxx::NEW<BuilderArray>(builders_->length);
    for (::jxx::lang::jint index = 0; index < builders_->length; ++index) {
        (*copy)[index] = (*builders_)[index];
    }
    return copy;
}

} // namespace jxx::ext::net::ssl
