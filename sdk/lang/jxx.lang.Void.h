#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {

class ClassAny;

class Void final
    : public ::jxx::lang::ClassBase<
          Void,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Void, JxxSuper>;

    static const ::jxx::Ptr<ClassAny> TYPE;

    Void(const Void&) = delete;
    Void& operator=(const Void&) = delete;
    Void(Void&&) = delete;
    Void& operator=(Void&&) = delete;

private:
    Void() = delete;
};

} // namespace jxx::lang
