#pragma once

#include "jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io { class InputStream; }

namespace jxx::net {

/**
 * Minimal java.net.URL representation for ClassLoader resources.
 * This is not a full URL implementation—enough for getResource()/toString().
 */
class URL final : public jxx::lang::Object {
public:
    explicit URL(jxx::Ptr<jxx::lang::String> spec);

    jxx::Ptr<jxx::lang::String> toString() const override;

    // Optional; can return null if you don't have stream impl
    jxx::Ptr<jxx::io::InputStream> openStream() const;

    jxx::Ptr<jxx::lang::String> getSpec() const;

private:
    jxx::Ptr<jxx::lang::String> spec_;
};

} // namespace jxx::net