#pragma once

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::text {

class Format
    : public ::jxx::lang::ClassBase<
          Format,
          ::jxx::lang::Object,
          ::jxx::lang::Cloneable,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        Format, JxxSuper, ::jxx::lang::Cloneable, ::jxx::io::SerializableI>;

    ~Format() override = 0;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override {
        (void)out;
    }
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override {
        (void)in;
    }
    void readObjectNoData() override {}

protected:
    Format() = default;
};

inline Format::~Format() = default;

} // namespace jxx::text
