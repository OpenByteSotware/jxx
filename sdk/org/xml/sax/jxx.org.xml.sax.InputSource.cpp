#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"

namespace jxx::org::xml::sax {

InputSource::InputSource() = default;

InputSource::InputSource(
    const ::jxx::Ptr<::jxx::io::InputStream>& byteStream)
    : byteStream_(byteStream) {
}

InputSource::InputSource(
    const ::jxx::Ptr<::jxx::io::Reader>& characterStream)
    : characterStream_(characterStream) {
}

InputSource::InputSource(
    const ::jxx::Ptr<::jxx::lang::String>& systemId)
    : systemId_(systemId) {
}

InputSource::~InputSource() = default;

::jxx::Ptr<::jxx::io::InputStream> InputSource::getByteStream() const {
    return byteStream_;
}

void InputSource::setByteStream(
    const ::jxx::Ptr<::jxx::io::InputStream>& byteStream) {
    byteStream_ = byteStream;
}

::jxx::Ptr<::jxx::io::Reader> InputSource::getCharacterStream() const {
    return characterStream_;
}

void InputSource::setCharacterStream(
    const ::jxx::Ptr<::jxx::io::Reader>& characterStream) {
    characterStream_ = characterStream;
}

::jxx::Ptr<::jxx::lang::String> InputSource::getEncoding() const {
    return encoding_;
}

void InputSource::setEncoding(
    const ::jxx::Ptr<::jxx::lang::String>& encoding) {
    encoding_ = encoding;
}

::jxx::Ptr<::jxx::lang::String> InputSource::getPublicId() const {
    return publicId_;
}

void InputSource::setPublicId(
    const ::jxx::Ptr<::jxx::lang::String>& publicId) {
    publicId_ = publicId;
}

::jxx::Ptr<::jxx::lang::String> InputSource::getSystemId() const {
    return systemId_;
}

void InputSource::setSystemId(
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    systemId_ = systemId;
}

} // namespace jxx::org::xml::sax
