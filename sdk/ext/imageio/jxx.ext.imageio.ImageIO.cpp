#include "ext/imageio/jxx.ext.imageio.ImageIO.h"
#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegImageWriter.h"
#include "ext/imageio/stream/jxx.ext.imageio.stream.MemoryCacheImageOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Iterator.h"
#include <cctype>
#include <memory>
namespace jxx::ext::imageio{namespace{class SingleWriterIterator final:public ::jxx::util::Iterator<ImageWriter>{public:explicit SingleWriterIterator(const ::jxx::Ptr<ImageWriter>&w):writer_(w){}bool hasNext()override{return writer_!=nullptr;}::jxx::Ptr<ImageWriter> next()override{auto value=writer_;writer_=nullptr;return value;}private: ::jxx::Ptr<ImageWriter> writer_;};}::jxx::Ptr<::jxx::util::Iterator<ImageWriter>> ImageIO::getImageWritersByFormatName(const ::jxx::Ptr<::jxx::lang::String>&name){if(!name)return std::make_shared<SingleWriterIterator>(nullptr);auto text=name->utf8();for(auto&c:text)c=static_cast<char>(::tolower(static_cast<unsigned char>(c)));return std::make_shared<SingleWriterIterator>((text=="jpeg"||text=="jpg")?::jxx::CAST<ImageWriter>(::jxx::NEW<::jxx::ext::imageio::internal::JpegImageWriter>()):nullptr);}::jxx::Ptr<stream::ImageOutputStream> ImageIO::createImageOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>&output){return ::jxx::NEW<stream::MemoryCacheImageOutputStream>(output);}}
