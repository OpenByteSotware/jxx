#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

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

    ::jxx::Ptr<Gson> create() const;

private:
    ::jxx::lang::jbool serializeNulls_ = false;
    ::jxx::lang::jbool htmlSafe_ = true;
    ::jxx::lang::jbool prettyPrinting_ = false;
    ::jxx::lang::jbool lenient_ = false;
};

} // namespace com::google::gson
