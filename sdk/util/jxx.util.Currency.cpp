#include "util/jxx.util.Currency.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.StringPool.h"

#include <mutex>
#include <string>
#include <unordered_map>

namespace jxx::util {
namespace {
    struct CurrencyRow { const char* country; const char* code; const char* symbol; int numericCode; int fractionDigits; };

    static const CurrencyRow ROWS[] = {
        {"BR", "BRL", "R$", 986, 2}, {"CA", "CAD", "$", 124, 2}, {"CN", "CNY", "¥", 156, 2}, {"DE", "EUR", "€", 978, 2},
        {"ES", "EUR", "€", 978, 2}, {"FR", "EUR", "€", 978, 2}, {"GB", "GBP", "£", 826, 2}, {"IT", "EUR", "€", 978, 2},
        {"JP", "JPY", "¥", 392, 0}, {"KR", "KRW", "₩", 410, 0}, {"RU", "RUB", "₽", 643, 2}, {"US", "USD", "$", 840, 2}
    };

    std::unordered_map<std::string, ::jxx::Ptr<Currency>>& cache() {
        static std::unordered_map<std::string, ::jxx::Ptr<Currency>> c;
        return c;
    }

    std::mutex& cacheMutex() {
        static std::mutex m;
        return m;
    }
}

Currency::Currency(const ::jxx::Ptr<::jxx::lang::String>& code,
                   const ::jxx::Ptr<::jxx::lang::String>& symbol,
                   ::jxx::lang::jint numericCode,
                   ::jxx::lang::jint defaultFractionDigits)
    : code_(code), symbol_(symbol), numericCode_(numericCode),
      defaultFractionDigits_(defaultFractionDigits) {}

::jxx::Ptr<Currency> Currency::getInstance(const ::jxx::Ptr<::jxx::lang::String>& code) {
    if (!code) return ::jxx::Ptr<Currency>();

    std::string key = code->utf8();
    std::lock_guard<std::mutex> lock(cacheMutex());
    auto& c = cache();
    auto it = c.find(key);
    if (it != c.end()) return it->second;

    for (const auto& row : ROWS) {
        if (key == row.code) {
            auto cur = ::jxx::NEW<Currency>(
                StringPool::intern(row.code), StringPool::intern(row.symbol),
                static_cast<::jxx::lang::jint>(row.numericCode),
                static_cast<::jxx::lang::jint>(row.fractionDigits));
            c.emplace(key, cur);
            return cur;
        }
    }
    return ::jxx::Ptr<Currency>();
}

::jxx::Ptr<Currency> Currency::getInstance(const ::jxx::Ptr<Locale>& locale) {
    if (!locale) return ::jxx::Ptr<Currency>();
    std::string country = locale->getCountry() ? locale->getCountry()->utf8() : "";
    for (const auto& row : ROWS) {
        if (country == row.country) {
            return getInstance(StringPool::intern(row.code));
        }
    }
    return ::jxx::Ptr<Currency>();
}

::jxx::Ptr<::jxx::lang::String> Currency::getCurrencyCode() const { return code_; }
::jxx::lang::jint Currency::getNumericCode() const { return numericCode_; }
::jxx::lang::jint Currency::getDefaultFractionDigits() const { return defaultFractionDigits_; }
::jxx::Ptr<::jxx::lang::String> Currency::getSymbol() const { return symbol_; }
::jxx::Ptr<::jxx::lang::String> Currency::getSymbol(const ::jxx::Ptr<Locale>& locale) const {
    (void)locale;
    return symbol_;
}
::jxx::Ptr<::jxx::lang::String> Currency::getDisplayName() const { return code_; }
::jxx::Ptr<::jxx::lang::String> Currency::getDisplayName(const ::jxx::Ptr<Locale>& locale) const {
    (void)locale;
    return code_;
}
::jxx::Ptr<::jxx::lang::String> Currency::toString() const { return code_; }

} // namespace jxx::util
