#include "util/jxx.util.DecimalFormat.h"
#include "lang/jxx.lang.String.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

namespace jxx::util {
namespace {
    std::string applyGrouping(const std::string& digits, char groupingSeparator) {
        if (digits.empty()) return digits;
        std::string sign;
        std::string body = digits;
        if (!body.empty() && body[0] == '-') {
            sign = "-";
            body.erase(body.begin());
        }
        std::string result;
        int count = 0;
        for (auto it = body.rbegin(); it != body.rend(); ++it) {
            if (count && (count % 3) == 0) result.push_back(groupingSeparator);
            result.push_back(*it);
            ++count;
        }
        std::reverse(result.begin(), result.end());
        return sign + result;
    }
}

DecimalFormat::DecimalFormat() : minFractionDigits_(0), maxFractionDigits_(3) {}

jxx::Ptr<DecimalFormat> DecimalFormat::ofPattern(const jxx::Ptr<jxx::lang::String>& pattern,
                                                 const jxx::Ptr<Locale>& locale) {
    (void)pattern;
    auto fmt = jxx::NEW<DecimalFormat>();
    fmt->locale_ = locale ? locale : Locale::getDefault();

    const std::string country = fmt->locale_->getCountry() ? fmt->locale_->getCountry()->utf8() : "";
    if (country == "FR" || country == "DE" || country == "IT" || country == "ES" || country == "BR" || country == "RU") {
        fmt->decimalSeparator_ = ',';
        fmt->groupingSeparator_ = (country == "FR") ? ' ' : '.';
    } else {
        fmt->decimalSeparator_ = '.';
        fmt->groupingSeparator_ = ',';
    }
    return fmt;
}

jxx::Ptr<jxx::lang::String> DecimalFormat::format(jxx::lang::jlong value) {
    std::string intPart = std::to_string(static_cast<long long>(value));
    if (groupingUsed_) intPart = applyGrouping(intPart, groupingSeparator_);
    return jxx::lang::String::valueOf(intPart.c_str());
}

jxx::Ptr<jxx::lang::String> DecimalFormat::format(jxx::lang::jdouble value) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(maxFractionDigits_) << static_cast<double>(value);
    std::string s = os.str();

    std::size_t dot = s.find('.');
    std::string intPart = (dot == std::string::npos) ? s : s.substr(0, dot);
    std::string fracPart = (dot == std::string::npos) ? std::string() : s.substr(dot + 1);

    while (static_cast<int>(fracPart.size()) > minFractionDigits_ && !fracPart.empty() && fracPart.back() == '0') {
        fracPart.pop_back();
    }

    if (groupingUsed_) intPart = applyGrouping(intPart, groupingSeparator_);
    if (!fracPart.empty()) s = intPart + std::string(1, static_cast<char>(decimalSeparator_)) + fracPart;
    else s = intPart;

    return jxx::lang::String::valueOf(s.c_str());
}

void DecimalFormat::setMinimumFractionDigits(jxx::lang::jint value) { minFractionDigits_ = (value < 0) ? 0 : value; }
void DecimalFormat::setMaximumFractionDigits(jxx::lang::jint value) {
    maxFractionDigits_ = (value < 0) ? 0 : value;
    if (maxFractionDigits_ < minFractionDigits_) minFractionDigits_ = maxFractionDigits_;
}

} // namespace jxx::util
