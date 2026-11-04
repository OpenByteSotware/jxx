#include "lang/ref/jxx.lang.ref.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/ref/jxx.lang.ref.PhantomReference.h"
#include "lang/ref/jxx.lang.ref.Reference.h"
#include "lang/ref/jxx.lang.ref.ReferenceQueue.h"
#include "lang/ref/jxx.lang.ref.SoftReference.h"
#include "lang/ref/jxx.lang.ref.WeakReference.h"

namespace jxx::lang::ref {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        using ObjectType = ::jxx::lang::Object;
        (void)Reference<ObjectType>::Class();
        (void)ReferenceQueue<ObjectType>::Class();
        (void)SoftReference<ObjectType>::Class();
        (void)WeakReference<ObjectType>::Class();
        (void)PhantomReference<ObjectType>::Class();
    });
}

void initialize()
{
    initializeClasses();
}

} // namespace jxx::lang::ref
