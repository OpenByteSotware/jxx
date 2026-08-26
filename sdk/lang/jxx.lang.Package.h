#pragma once

#include "jxx_types.h"
#include "jxx.lang.ClassInfo.h"
#include "jxx.lang.String.h"

namespace jxx::lang {

/**
 * Minimal java.lang.Package.
 * Java has more metadata; this provides getName()/toString() for parity.
 */
class Package final : public Object, public jxx::lang::ClassInfo<Package> {
public:
    explicit Package(const jxx::Ptr<String> name);

    jxx::Ptr<String> getName() const;

private:
    jxx::Ptr<String> name_;
};

} // namespace jxx::lang