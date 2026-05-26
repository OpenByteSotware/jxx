#pragma once

#include "jxx_types.h"
#include "jxx.lang.CharSequence.h"

namespace jxx::lang {

    /**
     * Java-like java.lang.Appendable.
     *
     * Java signature:
     *   Appendable append(CharSequence csq)
     *   Appendable append(CharSequence csq, int start, int end)
     *   Appendable append(char c)
     *
     * JXX rule: reference returns use jxx::Ptr<T>.
     */
    struct Appendable {
        virtual ~Appendable() = default;

        virtual jxx::Ptr<Appendable> append(const jxx::Ptr<CharSequence> csq) = 0;
        virtual jxx::Ptr<Appendable> append(const jxx::Ptr<CharSequence> csq, jint start, jint end) = 0;
        virtual jxx::Ptr<Appendable> append(jchar c) = 0;
    };

} // namespace jxx::lang