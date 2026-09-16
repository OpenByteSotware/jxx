#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "jxx_types.h"
#include "jxx.lang.CharSequence.h"

namespace jxx::lang {

    /**
     * Character-output interface with whole-sequence, subsequence,
     * and single-character append operations.
     */
    class Appendable : public jxx::lang::InterfaceBase<Appendable> {
    public:
        virtual ~Appendable() = default;

        virtual jxx::Ptr<Appendable> append(const jxx::Ptr<CharSequence>& csq) = 0;
        virtual jxx::Ptr<Appendable> append(const jxx::Ptr<CharSequence>& csq, jint start, jint end) = 0;
        virtual jxx::Ptr<Appendable> append(jchar c) = 0;
    };

} // namespace jxx::lang