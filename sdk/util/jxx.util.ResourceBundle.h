#pragma once

#include <functional>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.Enumeration.h"
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

    static void registerBundle(
        const jxx::Ptr<jxx::lang::String>& bundleName,
        const BundleFactory& factory);

    jxx::Ptr<jxx::lang::Object> getObject(
        const jxx::Ptr<jxx::lang::String>& key);

    jxx::Ptr<jxx::lang::String> getString(
        const jxx::Ptr<jxx::lang::String>& key);

    jxx::Ptr<Enumeration<jxx::lang::String>> getKeys();

    jxx::Ptr<Locale> getLocale() const;
    jxx::Ptr<jxx::lang::String> getBaseBundleName() const;

protected:
    ResourceBundle() = default;

    virtual jxx::Ptr<jxx::lang::Object> handleGetObject(
        const jxx::Ptr<jxx::lang::String>& key) = 0;

    virtual jxx::Ptr<Enumeration<jxx::lang::String>>
    getLocalKeys() = 0;

    void setParent(const jxx::Ptr<ResourceBundle>& parent);
    jxx::Ptr<ResourceBundle> getParent() const;

private:
    jxx::Ptr<Locale> locale_;
    jxx::Ptr<jxx::lang::String> baseBundleName_;
    jxx::Ptr<ResourceBundle> parent_;
};

} // namespace jxx::util
