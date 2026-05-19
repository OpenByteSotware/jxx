#pragma once

#include "jxx_types.h"

namespace jxx::util {
class Formatter;

/** Java 8: java.util.Formattable (interface) */
struct Formattable {
    virtual ~Formattable() = default;

    virtual void formatTo(jxx::Ptr<Formatter> formatter, jxx::lang::jint flags, jxx::lang::jint width, jxx::lang::jint precision) = 0;
};

} // namespace jxx::util
