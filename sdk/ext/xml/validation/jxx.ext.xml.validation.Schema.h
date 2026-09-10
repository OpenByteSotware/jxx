#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::ext::xml::validation {

class Schema
    : public ::jxx::lang::ClassBase<
          Schema,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Schema, JxxSuper>;

    ~Schema() override;

protected:
    Schema();
};

} // namespace jxx::ext::xml::validation
