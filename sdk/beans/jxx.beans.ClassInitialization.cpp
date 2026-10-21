#include "beans/jxx.beans.ClassInitialization.h"

#include <mutex>

#include "beans/jxx.beans.PropertyChangeEvent.h"
#include "beans/jxx.beans.PropertyChangeListener.h"
#include "beans/jxx.beans.PropertyChangeSupport.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::beans {
namespace {

template <typename T>
void registerType()
{
    ::jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<PropertyChangeEvent>();
        registerType<PropertyChangeListener>();
        registerType<PropertyChangeSupport>();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::beans
