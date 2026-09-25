#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamResult.h"
namespace jxx::ext::xml::transform::stream {
StreamResult::StreamResult()=default;
StreamResult::StreamResult(const ::jxx::Ptr<::jxx::io::OutputStream>& outputStream):outputStream_(outputStream){}
StreamResult::StreamResult(const ::jxx::Ptr<::jxx::io::Writer>& writer):writer_(writer){}
StreamResult::StreamResult(const ::jxx::Ptr<::jxx::lang::String>& systemId):systemId_(systemId){}
StreamResult::StreamResult(const ::jxx::Ptr<::jxx::io::File>& file){setSystemId(file);}
void StreamResult::setOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& outputStream){outputStream_=outputStream;}
::jxx::Ptr<::jxx::io::OutputStream> StreamResult::getOutputStream()const{return outputStream_;}
void StreamResult::setWriter(const ::jxx::Ptr<::jxx::io::Writer>& writer){writer_=writer;}
::jxx::Ptr<::jxx::io::Writer> StreamResult::getWriter()const{return writer_;}
void StreamResult::setSystemId(const ::jxx::Ptr<::jxx::lang::String>& systemId){systemId_=systemId;}
void StreamResult::setSystemId(const ::jxx::Ptr<::jxx::io::File>& file){systemId_=file==nullptr?nullptr:file->getAbsolutePath();}
::jxx::Ptr<::jxx::lang::String> StreamResult::getSystemId()const{return systemId_;}
::jxx::Ptr<::jxx::lang::String> StreamResult::FEATURE(){static const auto v=::jxx::NEW<::jxx::lang::String>("http://javax.xml.transform.stream.StreamResult/feature");return v;}
}
