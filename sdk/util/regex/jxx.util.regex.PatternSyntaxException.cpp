#include <sstream>

#include "io/jxx.util.regex.PatternSyntaxException.h"

namespace jxx {
namespace util {
namespace regex {

PatternSyntaxException::PatternSyntaxException(
    jxx::Ptr<jxx::lang::String> desc,
    jxx::Ptr<jxx::lang::String> regex,
    jxx::lang::jint index)
    : desc_(desc), pattern_(regex), index_(index) {
}

jxx::Ptr<jxx::lang::String> PatternSyntaxException::getDescription() {
    return desc_;
}

jxx::Ptr<jxx::lang::String> PatternSyntaxException::getPattern() {
    return pattern_;
}

jxx::lang::jint PatternSyntaxException::getIndex() {
    return index_;
}

jxx::Ptr<jxx::lang::String> PatternSyntaxException::getMessage() const {
    std::ostringstream oss;
    oss << (desc_ == nullptr ? "" : desc_->utf8());
    if (index_ >= 0) {
        oss << " near index " << index_;
    }
    if (pattern_ != nullptr) {
        oss << "\n" << pattern_->utf8();
        if (index_ >= 0) {
            oss << "\n";
            for (jxx::lang::jint i = 0; i < index_; ++i) {
                oss << ' ';
            }
            oss << '^';
        }
    }
    return std::make_shared<jxx::lang::String>(oss.str());
}

} // namespace regex
} // namespace util
} // namespace jxx
