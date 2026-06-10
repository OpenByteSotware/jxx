#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"

namespace jxx::lang {
    class String;

// Minimal Charset with Java-8-like surface.
// Supports UTF-8 (default), US-ASCII, ISO-8859-1.
class Charset final : public Object {
public:
    enum class Kind { UTF8, ASCII, ISO_8859_1 };
    explicit Charset(Kind k);
    virtual ~Charset() = default;

    static jxx::Ptr<Charset> defaultCharset();
    static jxx::Ptr<Charset> forName(const jxx::Ptr<String> name);

    jxx::Ptr<String> name() const;

    jxx::lang::ByteArray encode(const jxx::Ptr<String> s) const;
    jxx::Ptr<String> decode(const jxx::lang::ByteArray bytes) const;

    jbool equals(const jxx::Ptr<Object> o) const override;
    jint hashCode() const override;

private:

    Kind kind_;
};

} // namespace jxx::lang
