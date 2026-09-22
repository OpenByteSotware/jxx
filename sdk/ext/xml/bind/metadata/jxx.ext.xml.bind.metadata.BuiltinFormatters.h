#pragma once

#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueFormatter.h"

namespace jxx::ext::xml::bind::metadata {

class StringFormatter final
    : public ::jxx::lang::ClassBase<
          StringFormatter, ::jxx::lang::Object, ValueFormatter> {
public:
    ::jxx::Ptr<::jxx::lang::String> format(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
};

class IntegerFormatter final
    : public ::jxx::lang::ClassBase<
          IntegerFormatter, ::jxx::lang::Object, ValueFormatter> {
public:
    ::jxx::Ptr<::jxx::lang::String> format(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
};

class LongFormatter final
    : public ::jxx::lang::ClassBase<
          LongFormatter, ::jxx::lang::Object, ValueFormatter> {
public:
    ::jxx::Ptr<::jxx::lang::String> format(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
};

class DoubleFormatter final
    : public ::jxx::lang::ClassBase<
          DoubleFormatter, ::jxx::lang::Object, ValueFormatter> {
public:
    ::jxx::Ptr<::jxx::lang::String> format(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
};

class BooleanFormatter final
    : public ::jxx::lang::ClassBase<
          BooleanFormatter, ::jxx::lang::Object, ValueFormatter> {
public:
    ::jxx::Ptr<::jxx::lang::String> format(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
};

} // namespace jxx::ext::xml::bind::metadata
