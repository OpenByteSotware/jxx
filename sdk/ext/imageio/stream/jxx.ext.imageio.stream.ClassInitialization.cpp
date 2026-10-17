#include "ext/imageio/stream/jxx.ext.imageio.stream.ClassInitialization.h"
#include <mutex>
#include "ext/imageio/stream/jxx.ext.imageio.stream.ImageInputStream.h"
#include "ext/imageio/stream/jxx.ext.imageio.stream.ImageOutputStream.h"
#include "ext/imageio/stream/jxx.ext.imageio.stream.MemoryCacheImageOutputStream.h"
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::ext::imageio::stream {
namespace { template<class T> void reg(){::jxx::lang::class_info_detail::registerClassInfo<T>();} }
void initializeClasses(){static std::once_flag f;std::call_once(f,[]{reg<ImageInputStream>();reg<ImageOutputStream>();reg<MemoryCacheImageOutputStream>();});}
}
