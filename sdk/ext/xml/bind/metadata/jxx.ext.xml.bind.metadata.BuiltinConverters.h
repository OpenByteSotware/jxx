#pragma once

#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueConverter.h"

namespace jxx::ext::xml::bind::metadata {

class StringConverter final
    : public ::jxx::lang::ClassBase<
          StringConverter, ::jxx::lang::Object, ValueConverter> {
public:
    ::jxx::Ptr<::jxx::lang::Object> convert(
        const ::jxx::Ptr<::jxx::lang::String>& text) override;
};

class IntegerConverter final
    : public ::jxx::lang::ClassBase<
          IntegerConverter, ::jxx::lang::Object, ValueConverter> {
public:
    ::jxx::Ptr<::jxx::lang::Object> convert(
        const ::jxx::Ptr<::jxx::lang::String>& text) override;
};

class LongConverter final
    : public ::jxx::lang::ClassBase<
          LongConverter, ::jxx::lang::Object, ValueConverter> {
public:
    ::jxx::Ptr<::jxx::lang::Object> convert(
        const ::jxx::Ptr<::jxx::lang::String>& text) override;
};

class DoubleConverter final
    : public ::jxx::lang::ClassBase<
          DoubleConverter, ::jxx::lang::Object, ValueConverter> {
public:
    ::jxx::Ptr<::jxx::lang::Object> convert(
        const ::jxx::Ptr<::jxx::lang::String>& text) override;
};

class BooleanConverter final
    : public ::jxx::lang::ClassBase<
          BooleanConverter, ::jxx::lang::Object, ValueConverter> {
public:
    ::jxx::Ptr<::jxx::lang::Object> convert(
        const ::jxx::Ptr<::jxx::lang::String>& text) override;
};

} // namespace jxx::ext::xml::bind::metadata
