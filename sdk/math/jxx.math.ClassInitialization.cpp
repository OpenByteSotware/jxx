#include "math/jxx.math.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.String.h"
#include "math/jxx.math.BigDecimal.h"
#include "math/jxx.math.BigInteger.h"

namespace jxx::math {
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
        registerType<BigDecimal>();
        registerType<BigInteger>();

        ::jxx::io::serialization::registerSerializableClass<BigDecimal>(
            1,
            [] {
                return ::jxx::NEW<BigDecimal>(
                    ::jxx::NEW<::jxx::lang::String>("0"));
            });
        ::jxx::io::serialization::registerSerializableClass<BigInteger>(
            1,
            [] { return ::jxx::NEW<BigInteger>(::jxx::lang::jlong{0}); });
    });
}

} // namespace jxx::math
