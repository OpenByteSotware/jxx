#include "util/jxx.util.ClassInitialization.Detail.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "util/jxx.util.Currency.h"
#include "util/jxx.util.ListResourceBundle.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.LocaleBuilder.h"
#include "util/jxx.util.PluralRules.h"
#include "util/jxx.util.PropertyResourceBundle.h"
#include "util/jxx.util.ResourceBundle.h"

namespace jxx::util::class_initialization_detail {
namespace {
template <typename T>
void registerType() { ::jxx::lang::class_info_detail::registerClassInfo<T>(); }
}
void initializeLocaleResources() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<Currency>();
        ::jxx::io::serialization::registerSerializableClass<Currency>(1, [] { return ::jxx::NEW<Currency>(::jxx::NEW<::jxx::lang::String>("XXX"), ::jxx::NEW<::jxx::lang::String>("XXX"), 999, 2); });
        registerType<ListResourceBundle>();
        registerType<Locale>();
        ::jxx::io::serialization::registerSerializableClass<Locale>(1, [] {
                    return ::jxx::NEW<Locale>(::jxx::NEW<::jxx::lang::String>(""));
                });
        registerType<LocaleBuilder>();
        registerType<PluralRules>();
        registerType<PropertyResourceBundle>();
        registerType<ResourceBundle>();
    });
}
}
