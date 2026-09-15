#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {

class String;

class Package final : public ClassBase<Package, Object> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<Package, JxxSuper>;

    explicit Package(const jxx::Ptr<String>& name);

    jxx::Ptr<String> getName() const;
    jxx::Ptr<String> toString() const override;

private:
    jxx::Ptr<String> name_;
};

} // namespace jxx::lang
