#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"

namespace jxx::lang {

/**
 * Minimal java.lang.Package.
 * Java has more metadata; this provides getName()/toString() for parity.
 */
class Package final : public Object {
public:
    explicit Package(jxx::Ptr<String> name);

    jxx::Ptr<String> getName() const;
    jxx::Ptr<String> toString() const override;

private:
    jxx::Ptr<String> name_;
};

} // namespace jxx::lang