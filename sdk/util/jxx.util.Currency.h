#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Locale.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class Currency : public jxx::lang::Object {
private:
    jxx::Ptr<jxx::lang::String> code_;
    jxx::Ptr<jxx::lang::String> symbol_;

public:
    Currency(const jxx::Ptr<jxx::lang::String>& code, const jxx::Ptr<jxx::lang::String>& symbol);

    static jxx::Ptr<Currency> getInstance(const jxx::Ptr<jxx::lang::String>& code);
    static jxx::Ptr<Currency> getInstance(const jxx::Ptr<Locale>& locale);

    jxx::Ptr<jxx::lang::String> getCurrencyCode() const;
    jxx::Ptr<jxx::lang::String> getSymbol() const;
    jxx::Ptr<jxx::lang::String> toString() const override;
};

} // namespace jxx::util
