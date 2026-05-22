#include "jxx.net.URL.h"

namespace jxx::net {

URL::URL(jxx::Ptr<jxx::lang::String> spec) : spec_(spec) {}

jxx::Ptr<jxx::lang::String> URL::toString() const {
    return spec_ ? spec_ : jxx::NEW<jxx::lang::String>("null");
}

jxx::Ptr<jxx::io::InputStream> URL::openStream() const {
    // Not implemented here; resource streams are typically provided by ClassLoader::getResourceAsStream.
    return nullptr;
}

jxx::Ptr<jxx::lang::String> URL::getSpec() const {
    return spec_;
}

} // namespace jxx::net