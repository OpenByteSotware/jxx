#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Throwable.h"
namespace jxx::lang {
class Error : public Throwable {
public:
    using Throwable::Throwable;
    protected:
    JXX_OBJECT_CLONE(Error)

    const char* typeName() const noexcept override { return "Error"; }
};
}
