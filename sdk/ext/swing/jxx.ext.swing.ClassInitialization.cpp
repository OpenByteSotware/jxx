#include "ext/swing/jxx.ext.swing.ClassInitialization.h"

#include <mutex>

#include "ext/swing/jxx.ext.swing.SwingWorker.h"
#include "ext/swing/jxx.ext.swing.ImageIcon.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::swing {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)SwingWorker<::jxx::lang::Object, ::jxx::lang::Object>::JxxClassInfoMarker::Class();
        (void)ImageIcon::Class();
    });
}

} // namespace jxx::ext::swing
