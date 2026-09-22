#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "com/google/gson/jxx.com.google.gson.FieldNamingPolicy.h"
#include "com/google/gson/jxx.com.google.gson.InstanceCreator.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapterFactory.h"
#include "lang/jxx.lang.Class.h"

namespace com::google::gson {

class Gson;

class GsonBuilder final
    : public ::jxx::lang::ClassBase<GsonBuilder, ::jxx::lang::Object> {
public:
    GsonBuilder() = default;

    ::jxx::Ptr<GsonBuilder> serializeNulls();
    ::jxx::Ptr<GsonBuilder> disableHtmlEscaping();
    ::jxx::Ptr<GsonBuilder> setPrettyPrinting();
    ::jxx::Ptr<GsonBuilder> setLenient();
    ::jxx::Ptr<GsonBuilder> setFieldNamingPolicy(FieldNamingPolicy policy);
    ::jxx::Ptr<GsonBuilder> setFieldNamingStrategy(
        const ::jxx::Ptr<FieldNamingStrategy>& strategy);
    ::jxx::Ptr<GsonBuilder> registerTypeAdapter(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
        const ::jxx::Ptr<TypeAdapter>& adapter);
    ::jxx::Ptr<GsonBuilder> registerInstanceCreator(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
        const ::jxx::Ptr<InstanceCreator>& creator);
    ::jxx::Ptr<GsonBuilder> registerTypeAdapterFactory(
        const ::jxx::Ptr<TypeAdapterFactory>& factory);

    ::jxx::Ptr<Gson> create() const;

private:
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
