#include "ext/swing/jxx.ext.swing.ClassInitialization.h"

#include <mutex>

#include "ext/swing/jxx.ext.swing.ImageIcon.h"
#include "ext/swing/jxx.ext.swing.SwingWorker.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::swing {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)SwingWorker<::jxx::lang::Object, ::jxx::lang::Object>::JxxClassInfoMarker::Class();
        (void)ImageIcon::Class();
        ::jxx::io::serialization::registerSerializableClass<ImageIcon>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ImageIcon, 1U, 2U>();
    });
}

} // namespace jxx::ext::swing
