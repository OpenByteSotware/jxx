#pragma once

#include <string>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.util.Locale.h"

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
        : public jxx::lang::Object
    {
    public:
        Formatter();
        explicit Formatter(const jxx::Ptr<Locale> locale);
        ~Formatter() override = default;

    public:
        jxx::Ptr<Locale> locale() const noexcept;
        void flush();
        void close();
        jxx::lang::jbool closed() const noexcept;

        jxx::Ptr<Formatter> format(const jxx::Ptr<jxx::lang::String> formatString,
            const jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args);

        jxx::Ptr<Formatter> format(const jxx::Ptr<Locale> locale,
            const jxx::Ptr<jxx::lang::String> formatString,
            const jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args);

        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        std::string buffer_;
        jxx::Ptr<Locale> locale_;
        jxx::lang::jbool closed_ = false;

    private:
        void ensureOpen_() const;
    };
}