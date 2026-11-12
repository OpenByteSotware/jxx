#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamSource.h"

namespace jxx::ext::xml::transform::stream {
StreamSource::StreamSource() = default;
StreamSource::StreamSource(const ::jxx::Ptr<::jxx::io::InputStream>& v) : inputStream_(v) {}
StreamSource::StreamSource(const ::jxx::Ptr<::jxx::io::InputStream>& v, const ::jxx::Ptr<::jxx::lang::String>& id) : inputStream_(v), systemId_(id) {}
StreamSource::StreamSource(const ::jxx::Ptr<::jxx::io::Reader>& v) : reader_(v) {}
StreamSource::StreamSource(const ::jxx::Ptr<::jxx::io::Reader>& v, const ::jxx::Ptr<::jxx::lang::String>& id) : reader_(v), systemId_(id) {}
StreamSource::StreamSource(const ::jxx::Ptr<::jxx::lang::String>& id) : systemId_(id) {}
StreamSource::StreamSource(const ::jxx::Ptr<::jxx::io::File>& file) { setSystemId(file); }
void StreamSource::setInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& v) { inputStream_ = v; }
::jxx::Ptr<::jxx::io::InputStream> StreamSource::getInputStream() const { return inputStream_; }
void StreamSource::setReader(const ::jxx::Ptr<::jxx::io::Reader>& v) { reader_ = v; }
::jxx::Ptr<::jxx::io::Reader> StreamSource::getReader() const { return reader_; }
void StreamSource::setPublicId(const ::jxx::Ptr<::jxx::lang::String>& v) { publicId_ = v; }
::jxx::Ptr<::jxx::lang::String> StreamSource::getPublicId() const { return publicId_; }
void StreamSource::setSystemId(const ::jxx::Ptr<::jxx::lang::String>& v) { systemId_ = v; }
::jxx::Ptr<::jxx::lang::String> StreamSource::getSystemId() const { return systemId_; }
void StreamSource::setSystemId(const ::jxx::Ptr<::jxx::io::File>& file) { systemId_ = file == nullptr ? nullptr : file->getAbsolutePath(); }
::jxx::lang::jbool StreamSource::isEmpty() const { return inputStream_ == nullptr && reader_ == nullptr && systemId_ == nullptr; }
::jxx::Ptr<::jxx::lang::String> StreamSource::FEATURE() { static const auto v = ::jxx::NEW<::jxx::lang::String>("http://javax.xml.transform.stream.StreamSource/feature"); return v; }
} // namespace jxx::ext::xml::transform::stream
