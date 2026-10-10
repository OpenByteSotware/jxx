#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.ResourceBundle.h"

namespace jxx::util {

class ListResourceBundle
    : public ::jxx::lang::ClassBase<ListResourceBundle, ResourceBundle> {
public:
    using JxxSuper = ResourceBundle;
    using Super = ::jxx::lang::ClassBase<ListResourceBundle, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    using ContentsArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::Object>, 2U>;

    ~ListResourceBundle() override = default;

protected:
    ListResourceBundle() = default;

    virtual ::jxx::Ptr<ContentsArray> getContents() = 0;

    ::jxx::Ptr<::jxx::lang::Object> handleGetObject(
        const ::jxx::Ptr<::jxx::lang::String>& key) override;
    ::jxx::Ptr<Enumeration<::jxx::lang::String>> getLocalKeys() override;

private:
    void loadLookup_();

    std::once_flag loadFlag_;
    std::unordered_map<std::string, ::jxx::Ptr<::jxx::lang::Object>> lookup_;
    std::vector<::jxx::Ptr<::jxx::lang::String>> keys_;
};

class ListResourceBundleKeyEnumeration final
    : public ::jxx::lang::ClassBase<
          ListResourceBundleKeyEnumeration,
          ::jxx::lang::Object,
          Enumeration<::jxx::lang::String>> {
public:
    explicit ListResourceBundleKeyEnumeration(
        std::vector<::jxx::Ptr<::jxx::lang::String>> keys);

    ::jxx::lang::jbool hasMoreElements() override;
    ::jxx::Ptr<::jxx::lang::String> nextElement() override;

private:
    std::vector<::jxx::Ptr<::jxx::lang::String>> keys_;
    std::size_t index_ = 0U;
};

} // namespace jxx::util
