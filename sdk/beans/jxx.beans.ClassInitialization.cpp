#include "beans/jxx.beans.ClassInitialization.h"

#include <mutex>

#include "beans/jxx.beans.PropertyChangeEvent.h"
#include "beans/jxx.beans.PropertyChangeListener.h"
#include "beans/jxx.beans.PropertyChangeSupport.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

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

        ::jxx::io::serialization::registerSerializableClass<PropertyChangeEvent>(
            static_cast<::jxx::lang::jlong>(7042693688939648123LL),
            [] {
                return ::jxx::NEW<PropertyChangeEvent>(
                    ::jxx::NEW<::jxx::lang::Object>(),
                    ::jxx::NEW<::jxx::lang::String>(), nullptr, nullptr);
            });
        ::jxx::io::serialization::registerSerializableClass<PropertyChangeSupport>(
            static_cast<::jxx::lang::jlong>(6401253773779951803LL),
            [] { return ::jxx::NEW<PropertyChangeSupport>(::jxx::NEW<::jxx::lang::Object>()); });
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::beans
