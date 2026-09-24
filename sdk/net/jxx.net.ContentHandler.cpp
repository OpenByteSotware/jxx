#include "net/jxx.net.ContentHandler.h"

namespace jxx::net {
jxx::Ptr<jxx::lang::Object> ContentHandler::getContent(
    const jxx::Ptr<URLConnection>& urlc,
    const jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::Class>, 1U>>& classes) {
    (void)classes;
    return getContent(urlc);
}
} // namespace jxx::net
