#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Comparator.h"

namespace jxx::lang {

class StringCaseInsensitiveComparator final
    : public ClassBase<
          StringCaseInsensitiveComparator,
          Object,
          jxx::util::Comparator<String>> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<
        StringCaseInsensitiveComparator,
        JxxSuper,
        jxx::util::Comparator<String>>;
    using JxxClassInfoMarker = ClassInfo<
        StringCaseInsensitiveComparator,
        JxxSuper,
        jxx::util::Comparator<String>>;

    static jxx::Ptr<ClassAny> Class();

    ::jxx::lang::jint compare(
        const jxx::Ptr<String>& first,
        const jxx::Ptr<String>& second) const override;
};

} // namespace jxx::lang
