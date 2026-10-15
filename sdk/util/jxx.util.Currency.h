#pragma once

#include "lang/jxx_types.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Locale.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class Currency final
    : public ::jxx::lang::ClassBase<
          Currency,
          ::jxx::lang::Object,
          ::jxx::io::SerializableI> {
private:
    ::jxx::Ptr<::jxx::lang::String> code_;
    ::jxx::Ptr<::jxx::lang::String> symbol_;
    ::jxx::lang::jint numericCode_ = 0;
    ::jxx::lang::jint defaultFractionDigits_ = 2;

public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        Currency, JxxSuper, ::jxx::io::SerializableI>;
    Currency(const ::jxx::Ptr<::jxx::lang::String>& code,
             const ::jxx::Ptr<::jxx::lang::String>& symbol,
             ::jxx::lang::jint numericCode,
             ::jxx::lang::jint defaultFractionDigits);

    static ::jxx::Ptr<Currency> getInstance(const ::jxx::Ptr<::jxx::lang::String>& code);
    static ::jxx::Ptr<Currency> getInstance(const ::jxx::Ptr<Locale>& locale);

    ::jxx::Ptr<::jxx::lang::String> getCurrencyCode() const;
    ::jxx::lang::jint getNumericCode() const;
    ::jxx::lang::jint getDefaultFractionDigits() const;
    ::jxx::Ptr<::jxx::lang::String> getSymbol() const;
    ::jxx::Ptr<::jxx::lang::String> getSymbol(const ::jxx::Ptr<Locale>& locale) const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayName() const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayName(const ::jxx::Ptr<Locale>& locale) const;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override { if(!out)throw ::jxx::lang::NullPointerException();out->writeObject(code_);out->writeObject(symbol_);out->writeInt(numericCode_);out->writeInt(defaultFractionDigits_); }
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override { if(!in)throw ::jxx::lang::NullPointerException();code_=::jxx::CAST<::jxx::lang::String>(in->readObject());symbol_=::jxx::CAST<::jxx::lang::String>(in->readObject());numericCode_=in->readInt();defaultFractionDigits_=in->readInt(); }
    void readObjectNoData() override { code_=::jxx::NEW<::jxx::lang::String>("");symbol_=code_;numericCode_=0;defaultFractionDigits_=2; }
};

} // namespace jxx::util
