#include "ext/imageio/jxx.ext.imageio.ClassInitialization.h"
#include "ext/imageio/jxx.ext.imageio.IIOImage.h"
#include "ext/imageio/jxx.ext.imageio.ImageWriteParam.h"
#include "ext/imageio/jxx.ext.imageio.ImageWriter.h"
#include <mutex>
namespace jxx::ext::imageio { void initializeClasses(){ static std::once_flag flag; std::call_once(flag,[]{ (void)IIOImage::Class(); (void)ImageWriteParam::Class(); (void)ImageWriter::Class(); }); } }
