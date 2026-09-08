#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::lang { class String; }
namespace jxx::io { class InputStream; class Reader; }
namespace jxx::org::xml::sax { class InputSource : public jxx::lang::ClassBase<InputSource,jxx::lang::Object> { public: InputSource(); explicit InputSource(const jxx::Ptr<jxx::io::InputStream>& stream); explicit InputSource(const jxx::Ptr<jxx::lang::String>& systemId); jxx::Ptr<jxx::io::InputStream> getByteStream()const; void setByteStream(const jxx::Ptr<jxx::io::InputStream>& stream); jxx::Ptr<jxx::lang::String> getSystemId()const; void setSystemId(const jxx::Ptr<jxx::lang::String>& id); jxx::Ptr<jxx::lang::String> getPublicId()const; void setPublicId(const jxx::Ptr<jxx::lang::String>& id); jxx::Ptr<jxx::lang::String> getEncoding()const; void setEncoding(const jxx::Ptr<jxx::lang::String>& encoding); private: jxx::Ptr<jxx::io::InputStream> stream_; jxx::Ptr<jxx::lang::String> systemId_,publicId_,encoding_; }; }
