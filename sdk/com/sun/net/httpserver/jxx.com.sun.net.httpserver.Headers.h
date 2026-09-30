#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.Map.h"

namespace jxx::com::sun::net::httpserver::internal {
class DefaultHttpServer;
}

namespace jxx::com::sun::net::httpserver {

class Headers final
    : public ::jxx::lang::ClassBase<
          Headers,
          ::jxx::lang::Object,
          ::jxx::util::Map<
              ::jxx::lang::String,
              ::jxx::util::List<::jxx::lang::String>>> {
public:
    using ValueList = ::jxx::util::List<::jxx::lang::String>;
    using MapType = ::jxx::util::Map<::jxx::lang::String, ValueList>;
    using StorageType = ::jxx::util::HashMap<::jxx::lang::String, ValueList>;
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Headers, JxxSuper, MapType>;

    Headers();

    void add(
        const ::jxx::Ptr<::jxx::lang::String>& key,
        const ::jxx::Ptr<::jxx::lang::String>& value);
    void set(
        const ::jxx::Ptr<::jxx::lang::String>& key,
        const ::jxx::Ptr<::jxx::lang::String>& value);
    ::jxx::Ptr<::jxx::lang::String> getFirst(
        const ::jxx::Ptr<::jxx::lang::String>& key);

    ::jxx::lang::jint size() override;
    ::jxx::lang::jbool containsKey(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override;
    ::jxx::lang::jbool containsValue(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
    ::jxx::Ptr<ValueList> get(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override;
    ::jxx::Ptr<ValueList> put(
        const ::jxx::Ptr<::jxx::lang::String>& key,
        const ::jxx::Ptr<ValueList>& value) override;
    ::jxx::Ptr<ValueList> remove(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override;
    void putAll(const ::jxx::Ptr<MapType>& source) override;
    void clear() override;
    ::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>> keySet() override;
    ::jxx::Ptr<::jxx::util::Collection<ValueList>> values() override;
    ::jxx::Ptr<::jxx::util::Set<
        ::jxx::util::MapEntry<::jxx::lang::String, ValueList>>> entrySet() override;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;

private:
    friend class ::jxx::com::sun::net::httpserver::internal::DefaultHttpServer;

    void freezeInternal();
    ::jxx::lang::jbool isFrozenInternal() const noexcept;

    void ensureMutable_() const;

    static ::jxx::Ptr<::jxx::lang::String> normalizeKey_(
        const ::jxx::Ptr<::jxx::lang::String>& key);

    static ::jxx::Ptr<::jxx::lang::String> normalizeObjectKey_(
        const ::jxx::Ptr<::jxx::lang::Object>& key);

    static void validateValue_(
        const ::jxx::Ptr<::jxx::lang::String>& value);

    ::jxx::Ptr<StorageType> values_;
    ::jxx::lang::jbool frozen_ = false;

    static void validateList_(
        const ::jxx::Ptr<ValueList>& values);
};

} // namespace jxx::com::sun::net::httpserver
