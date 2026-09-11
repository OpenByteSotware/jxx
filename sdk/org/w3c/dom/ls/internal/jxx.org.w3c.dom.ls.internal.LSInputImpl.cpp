#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSInputImpl.h"

namespace jxx::org::w3c::dom::ls::internal {

LSInputImpl::LSInputImpl()
    : certifiedText_(false) {
}

LSInputImpl::~LSInputImpl() = default;

::jxx::Ptr<::jxx::io::Reader> LSInputImpl::getCharacterStream() const { return characterStream_; }
void LSInputImpl::setCharacterStream(const ::jxx::Ptr<::jxx::io::Reader>& value) { characterStream_ = value; }
::jxx::Ptr<::jxx::io::InputStream> LSInputImpl::getByteStream() const { return byteStream_; }
void LSInputImpl::setByteStream(const ::jxx::Ptr<::jxx::io::InputStream>& value) { byteStream_ = value; }
::jxx::Ptr<::jxx::lang::String> LSInputImpl::getStringData() const { return stringData_; }
void LSInputImpl::setStringData(const ::jxx::Ptr<::jxx::lang::String>& value) { stringData_ = value; }
::jxx::Ptr<::jxx::lang::String> LSInputImpl::getSystemId() const { return systemId_; }
void LSInputImpl::setSystemId(const ::jxx::Ptr<::jxx::lang::String>& value) { systemId_ = value; }
::jxx::Ptr<::jxx::lang::String> LSInputImpl::getPublicId() const { return publicId_; }
void LSInputImpl::setPublicId(const ::jxx::Ptr<::jxx::lang::String>& value) { publicId_ = value; }
::jxx::Ptr<::jxx::lang::String> LSInputImpl::getBaseURI() const { return baseURI_; }
void LSInputImpl::setBaseURI(const ::jxx::Ptr<::jxx::lang::String>& value) { baseURI_ = value; }
::jxx::Ptr<::jxx::lang::String> LSInputImpl::getEncoding() const { return encoding_; }
void LSInputImpl::setEncoding(const ::jxx::Ptr<::jxx::lang::String>& value) { encoding_ = value; }
::jxx::lang::jbool LSInputImpl::getCertifiedText() const { return certifiedText_; }
void LSInputImpl::setCertifiedText(::jxx::lang::jbool value) { certifiedText_ = value; }

} // namespace jxx::org::w3c::dom::ls::internal
