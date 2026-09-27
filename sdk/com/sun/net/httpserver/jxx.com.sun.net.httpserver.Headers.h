#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.Map.h"

namespace jxx::com::sun::net::httpserver {

class Headers final
    : public ::jxx::util::HashMap<
          ::jxx::lang::String,
          ::jxx::util::List<::jxx::lang::String>> {
public:
    using ValueList = ::jxx::util::List<::jxx::lang::String>;
    using JxxSuper = ::jxx::util::HashMap<
        ::jxx::lang::String,
        ValueList>;
    using Super = ::jxx::lang::ClassBase<Headers, JxxSuper>;

    Headers();

    void freezeInternal();
    ::jxx::lang::jbool isFrozenInternal() const noexcept;

    void add(
        const ::jxx::Ptr<::jxx::lang::String>& key,
        const ::jxx::Ptr<::jxx::lang::String>& value);

    void set(
        const ::jxx::Ptr<::jxx::lang::String>& key,
        const ::jxx::Ptr<::jxx::lang::String>& value);

    ::jxx::Ptr<::jxx::lang::String> getFirst(
        const ::jxx::Ptr<::jxx::lang::String>& key);

    ::jxx::lang::jbool containsKey(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override;

    ::jxx::Ptr<ValueList> get(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override;

    ::jxx::Ptr<ValueList> put(
        const ::jxx::Ptr<::jxx::lang::String>& key,
        const ::jxx::Ptr<ValueList>& value) override;

    ::jxx::Ptr<ValueList> remove(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override;

    ::jxx::Ptr<::jxx::util::Set<::jxx::util::MapEntry<::jxx::lang::String, ValueList>>> entrySet() override;
    ::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>> keySet() override;
    ::jxx::Ptr<::jxx::util::Collection<ValueList>> values() override;

    void clear() override;

    void putAll(
        const ::jxx::Ptr<
            ::jxx::util::Map<::jxx::lang::String, ValueList>>& source) override;

private:
    void ensureMutable_() const;

    static ::jxx::Ptr<::jxx::lang::String> normalizeKey_(
        const ::jxx::Ptr<::jxx::lang::String>& key);

    static ::jxx::Ptr<::jxx::lang::String> normalizeObjectKey_(
        const ::jxx::Ptr<::jxx::lang::Object>& key);

    static void validateValue_(
        const ::jxx::Ptr<::jxx::lang::String>& value);

    ::jxx::lang::jbool frozen_ = false;

    static void validateList_(
        const ::jxx::Ptr<ValueList>& values);
};

} // namespace jxx::com::sun::net::httpserver
