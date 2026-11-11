#pragma once

#include <string>

#include "lang/jxx_types.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.Flushable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Enum.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.JxxArray.h"
#include "util/jxx.util.Locale.h"

namespace jxx::lang {
    class String;
}

namespace jxx::util
{
    /**
     * Java-8-style Formatter core for JXX.
     *
     * Public syntax is Java Formatter syntax:
     *   %s %d %08x %.2f %1$s %<s %tF %tT %% %n %a %A
     *
     * This implementation is suitable for String::format(...)
     * and stores formatted output internally.
     */
    class Formatter final
        : public ::jxx::lang::ClassBase<
              Formatter,
              ::jxx::lang::Object,
              ::jxx::io::Closeable,
              ::jxx::io::Flushable>
    {
    public:
        class BigDecimalLayoutForm final
            : public ::jxx::lang::ClassBase<
                  BigDecimalLayoutForm,
                  ::jxx::lang::Enum<BigDecimalLayoutForm>> {
        public:
            using JxxSuper = ::jxx::lang::Enum<BigDecimalLayoutForm>;
            using Super = ::jxx::lang::ClassBase<BigDecimalLayoutForm, JxxSuper>;
            using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

            static const ::jxx::Ptr<BigDecimalLayoutForm> SCIENTIFIC;
            static const ::jxx::Ptr<BigDecimalLayoutForm> DECIMAL_FLOAT;

            static ::jxx::Ptr<::jxx::lang::JxxArray<
                ::jxx::Ptr<BigDecimalLayoutForm>, 1U>> values();
            static ::jxx::Ptr<BigDecimalLayoutForm> valueOf(
                const ::jxx::Ptr<::jxx::lang::String>& name);

            BigDecimalLayoutForm(
                const ::jxx::Ptr<::jxx::lang::String>& name,
                ::jxx::lang::jint ordinal);
        };

        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            Formatter, JxxSuper, ::jxx::io::Closeable, ::jxx::io::Flushable>;

        Formatter();
        explicit Formatter(const ::jxx::Ptr<Locale>& locale);
        ~Formatter() override = default;

    public:
        ::jxx::Ptr<Locale> locale() const;
        void flush() override;
        void close() override;
        ::jxx::lang::jbool closed() const noexcept;

        ::jxx::Ptr<Formatter> format(const ::jxx::Ptr<::jxx::lang::String>& formatString,
            const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>& args);

        ::jxx::Ptr<Formatter> format(const ::jxx::Ptr<Locale>& locale,
            const ::jxx::Ptr<::jxx::lang::String>& formatString,
            const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>& args);

        ::jxx::Ptr<::jxx::lang::String> toString() const override;

    private:
        std::string buffer_;
        ::jxx::Ptr<Locale> locale_;
        ::jxx::lang::jbool closed_ = false;

    private:
        void ensureOpen_() const;
    };
}