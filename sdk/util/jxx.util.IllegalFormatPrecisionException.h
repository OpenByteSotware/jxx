#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::lang
{

    class ClassAny;
    class Object;

    template <
        typename Derived,
        typename JxxSuper,
        typename... JxxInterfaces>
    class ClassInfo;

} // namespace jxx::lang

namespace jxx::util
{

    class IllegalFormatPrecisionException
        : public IllegalFormatException
    {
    public:
        using JxxSuper =
            IllegalFormatException;

        using JxxClassInfoMarker =
            jxx::lang::ClassInfo<
            IllegalFormatPrecisionException,
            JxxSuper>;

        static jxx::Ptr<jxx::lang::ClassAny>
            Class();

        explicit IllegalFormatPrecisionException(
            jxx::lang::jint precision);

        IllegalFormatPrecisionException(
            const IllegalFormatPrecisionException&) =
            default;

        IllegalFormatPrecisionException(
            IllegalFormatPrecisionException&&) noexcept =
            default;

        IllegalFormatPrecisionException& operator=(
            const IllegalFormatPrecisionException&) =
            default;

        IllegalFormatPrecisionException& operator=(
            IllegalFormatPrecisionException&&) noexcept =
            default;

        ~IllegalFormatPrecisionException() override =
            default;

        jxx::lang::jint getPrecision() const noexcept;

    protected:
        jxx::Ptr<jxx::lang::Object>
            cloneImpl() const override;

        const char*
            typeName() const noexcept override;

    private:
        jxx::lang::jint precision_;
    };

} // namespace jxx::util