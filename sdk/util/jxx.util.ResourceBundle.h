#pragma once

#include <functional>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.Locale.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class ResourceBundle : public jxx::lang::Object {
public:
    using BundleFactory = std::function<jxx::Ptr<ResourceBundle>()>;

    static jxx::Ptr<ResourceBundle> getBundle(
        const jxx::Ptr<jxx::lang::String>& baseName);

    static jxx::Ptr<ResourceBundle> getBundle(
        const jxx::Ptr<jxx::lang::String>& baseName,
        const jxx::Ptr<Locale>& locale);

    static void clearCache();

    // JXX registration hook for compiled or property-backed bundle providers.
    static void registerBundle(
        const jxx::Ptr<jxx::lang::String>& bundleName,
        const BundleFactory& factory);

    virtual jxx::Ptr<jxx::lang::String> getString(
        const jxx::Ptr<jxx::lang::String>& key) = 0;

    jxx::Ptr<Locale> getLocale() const;
    jxx::Ptr<jxx::lang::String> getBaseBundleName() const;

protected:
    ResourceBundle() = default;

private:
    jxx::Ptr<Locale> locale_;
    jxx::Ptr<jxx::lang::String> baseBundleName_;
};

} // namespace jxx::util
