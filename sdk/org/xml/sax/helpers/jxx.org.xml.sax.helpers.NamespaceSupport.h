#pragma once

#include <memory>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.Enumeration.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::xml::sax::helpers {

class NamespaceSupport final
    : public ::jxx::lang::ClassBase<
          NamespaceSupport,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<NamespaceSupport, JxxSuper>;
    using StringArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>;

    static ::jxx::Ptr<::jxx::lang::String> XMLNS;
    static ::jxx::Ptr<::jxx::lang::String> NSDECL;

    NamespaceSupport();
    ~NamespaceSupport() override;

    void reset();
    void pushContext();
    void popContext();

    ::jxx::lang::jbool declarePrefix(
        const ::jxx::Ptr<::jxx::lang::String>& prefix,
        const ::jxx::Ptr<::jxx::lang::String>& uri);

    ::jxx::Ptr<StringArray> processName(
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
        const ::jxx::Ptr<StringArray>& parts,
        ::jxx::lang::jbool isAttribute);

    ::jxx::Ptr<::jxx::lang::String> getURI(
        const ::jxx::Ptr<::jxx::lang::String>& prefix) const;

    ::jxx::Ptr<::jxx::lang::String> getPrefix(
        const ::jxx::Ptr<::jxx::lang::String>& uri) const;

    ::jxx::Ptr<::jxx::util::Enumeration<::jxx::lang::String>>
    getPrefixes() const;

    ::jxx::Ptr<::jxx::util::Enumeration<::jxx::lang::String>>
    getPrefixes(const ::jxx::Ptr<::jxx::lang::String>& uri) const;

    ::jxx::Ptr<::jxx::util::Enumeration<::jxx::lang::String>>
    getDeclaredPrefixes() const;

    void setNamespaceDeclUris(::jxx::lang::jbool value);
    ::jxx::lang::jbool isNamespaceDeclUris() const;

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

} // namespace jxx::org::xml::sax::helpers
