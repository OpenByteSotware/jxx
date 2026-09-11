#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSOutputImpl.h"

namespace jxx::org::w3c::dom::ls::internal {

LSOutputImpl::LSOutputImpl() = default;
LSOutputImpl::~LSOutputImpl() = default;

::jxx::Ptr<::jxx::io::Writer> LSOutputImpl::getCharacterStream() const { return characterStream_; }
void LSOutputImpl::setCharacterStream(const ::jxx::Ptr<::jxx::io::Writer>& value) { characterStream_ = value; }
::jxx::Ptr<::jxx::io::OutputStream> LSOutputImpl::getByteStream() const { return byteStream_; }
void LSOutputImpl::setByteStream(const ::jxx::Ptr<::jxx::io::OutputStream>& value) { byteStream_ = value; }
::jxx::Ptr<::jxx::lang::String> LSOutputImpl::getSystemId() const { return systemId_; }
void LSOutputImpl::setSystemId(const ::jxx::Ptr<::jxx::lang::String>& value) { systemId_ = value; }
::jxx::Ptr<::jxx::lang::String> LSOutputImpl::getEncoding() const { return encoding_; }
void LSOutputImpl::setEncoding(const ::jxx::Ptr<::jxx::lang::String>& value) { encoding_ = value; }

} // namespace jxx::org::w3c::dom::ls::internal
