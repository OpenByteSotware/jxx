#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Class.h"
#include "com/google/gson/jxx.com.google.gson.FieldNamingStrategy.h"
#include "com/google/gson/jxx.com.google.gson.InstanceCreator.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapterFactory.h"

#include <string>

namespace jxx::lang { class String; }
namespace com::google::gson { class JsonElement; class TypeAdapter; }

namespace com::google::gson {

class Gson final
    : public ::jxx::lang::ClassBase<Gson, ::jxx::lang::Object> {
public:
    Gson() = default;
    Gson(
        ::jxx::lang::jbool serializeNulls,
        ::jxx::lang::jbool htmlSafe,
        ::jxx::lang::jbool prettyPrinting,
        ::jxx::lang::jbool lenient,
        const ::jxx::Ptr<FieldNamingStrategy>& fieldNamingStrategy = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>>& adapterTypes = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TypeAdapter>, 1U>>& adapters = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>>& creatorTypes = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<InstanceCreator>, 1U>>& creators = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TypeAdapterFactory>, 1U>>& factories = nullptr);

    ::jxx::Ptr<JsonElement> fromJson(
        const ::jxx::Ptr<::jxx::lang::String>& json) const;

    ::jxx::Ptr<::jxx::lang::String> toJson(
        const ::jxx::Ptr<JsonElement>& element) const;

    ::jxx::Ptr<::jxx::lang::Object> fromJson(
        const ::jxx::Ptr<::jxx::lang::String>& json,
        const ::jxx::Ptr<TypeAdapter>& adapter) const;

    ::jxx::Ptr<::jxx::lang::String> toJson(
        const ::jxx::Ptr<::jxx::lang::Object>& value,
        const ::jxx::Ptr<TypeAdapter>& adapter) const;

    ::jxx::lang::jbool serializeNulls() const noexcept;
    ::jxx::lang::jbool htmlSafe() const noexcept;
    ::jxx::lang::jbool prettyPrinting() const noexcept;
    ::jxx::lang::jbool lenient() const noexcept;
    ::jxx::Ptr<FieldNamingStrategy> fieldNamingStrategy() const;
    ::jxx::Ptr<TypeAdapter> getAdapter(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type) const;
    ::jxx::Ptr<::jxx::lang::Object> createInstance(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type) const;

private:
    static std::string formatTree_(
        const ::jxx::Ptr<JsonElement>& element,
        ::jxx::lang::jbool serializeNulls,
        ::jxx::lang::jbool htmlSafe,
        ::jxx::lang::jbool prettyPrinting,
        ::jxx::lang::jint depth);

    ::jxx::lang::jbool serializeNulls_ = false;
    ::jxx::lang::jbool htmlSafe_ = true;
    ::jxx::lang::jbool prettyPrinting_ = false;
    ::jxx::lang::jbool lenient_ = false;
    ::jxx::Ptr<FieldNamingStrategy> fieldNamingStrategy_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>> adapterTypes_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TypeAdapter>, 1U>> adapters_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>> creatorTypes_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<InstanceCreator>, 1U>> creators_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TypeAdapterFactory>, 1U>> factories_;
};

} // namespace com::google::gson
