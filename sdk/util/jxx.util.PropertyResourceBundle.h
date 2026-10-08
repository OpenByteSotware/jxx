#pragma once

#include <vector>

#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.Properties.h"
#include "util/jxx.util.ResourceBundle.h"

namespace jxx::util {

class PropertyResourceBundle final
    : public ::jxx::lang::ClassBase<
          PropertyResourceBundle,
          ResourceBundle> {
public:
    using JxxSuper = ResourceBundle;
    using Super = ::jxx::lang::ClassBase<PropertyResourceBundle, JxxSuper>;

    explicit PropertyResourceBundle(
        const ::jxx::Ptr<::jxx::io::InputStream>& stream);
    explicit PropertyResourceBundle(
        const ::jxx::Ptr<::jxx::io::Reader>& reader);

protected:
    ::jxx::Ptr<::jxx::lang::Object> handleGetObject(
        const ::jxx::Ptr<::jxx::lang::String>& key) override;
    ::jxx::Ptr<Enumeration<::jxx::lang::String>> getLocalKeys() override;

private:
    ::jxx::Ptr<Properties> properties_;
};

class PropertyResourceBundleKeyEnumeration final
    : public ::jxx::lang::ClassBase<
          PropertyResourceBundleKeyEnumeration,
          ::jxx::lang::Object,
          Enumeration<::jxx::lang::String>> {
public:
    explicit PropertyResourceBundleKeyEnumeration(
        std::vector<::jxx::Ptr<::jxx::lang::String>> keys);

    ::jxx::lang::jbool hasMoreElements() override;
    ::jxx::Ptr<::jxx::lang::String> nextElement() override;

private:
    std::vector<::jxx::Ptr<::jxx::lang::String>> keys_;
    std::size_t index_ = 0U;
};

} // namespace jxx::util
