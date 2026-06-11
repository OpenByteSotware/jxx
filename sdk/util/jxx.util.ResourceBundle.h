#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Locale.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class ResourceBundle : public jxx::lang::Object {
public:
    static jxx::Ptr<ResourceBundle> getBundle(const jxx::Ptr<jxx::lang::String>& baseName,
                                              const jxx::Ptr<Locale>& locale);

    virtual jxx::Ptr<jxx::lang::String> getString(const jxx::Ptr<jxx::lang::String>& key) = 0;
};

} // namespace jxx::util
